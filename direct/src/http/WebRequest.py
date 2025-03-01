import types
import direct
from pandac.PandaModules import HttpRequest
from direct.directnotify.DirectNotifyGlobal import directNotify
from direct.task.TaskManagerGlobal import taskMgr
from direct.task import Task
from LandingPage import LandingPage
from direct.showbase import ElementTree as ET

notify = directNotify.newCategory('WebRequestDispatcher')

class WebRequest(object):
    """
    Pointer to a single web request (maps to an open HTTP socket).
    An instance of this class maps to a single client waiting for a response.

    connection is an instance of libdirect.HttpRequest
    """
    notify = directNotify.newCategory('WebRequest')

    def __init__(self,connection):
        self.connection = connection

    def logRequest(self):
        self.notify.info('[from %s]\n%s' % (self.connection.GetSourceAddress(), self.connection.GetRawRequest()))

    def getURI(self):
        return self.connection.GetRequestURL()

    def getRequestType(self):
        return self.connection.GetRequestType()

    def dictFromGET(self):
        result = {}
        for pair in self.connection.GetRequestOptionString().split('&'):
            arg = pair.split('=',1)
            if len(arg) > 1:
                result[arg[0]] = arg[1]
        return result

    def respondHTTP(self,status,body):
        status = str(status)
        msg = u"HTTP/1.0 %s\r\nContent-Type: text/html\r\n\r\n%s" % (status,body)
        self.connection.SendThisResponse(encodedUtf8(msg))

    def respond(self,body):
        self.respondHTTP("200 OK",body)

    def respondXML(self,body):
        msg = u"HTTP/1.0 200 OK\r\nContent-Type: text/xml\r\n\r\n%s" % body
        self.connection.SendThisResponse(encodedUtf8(msg))

    def respondCustom(self,contentType,body):
        msg = "HTTP/1.0 200 OK\r\nContent-Type: %s" % contentType

        if contentType in ["text/css",]:
            msg += "\nCache-Control: max-age=313977290\nExpires: Tue, 02 May 2017 04:08:44 GMT\n"

        msg += "\r\n\r\n%s" % (body)
        self.connection.SendThisResponse(msg)

    def timeout(self):
        resp = "<html><body>Error 504: Request timed out</body></html>\r\n"
        self.respondHTTP("504 Gateway Timeout",resp)

    def getSourceAddress(self):
        return self.connection.GetSourceAddress()


# --------------------------------------------------------------------------------
    
class SkinningReplyTo:
    def __init__(self, replyTo, dispatcher, uri, doSkin):
        self._replyTo = replyTo
        self._dispatcher = dispatcher
        self._uri = uri
        self._doSkin = doSkin
        self._headTag = ET.Element('head')
        self._bodyTag = ET.Element('body')

    def respondHTTP(self,status,body):
        if self._doSkin:
            body = self._dispatcher.landingPage.skin(
                body, self._uri, headTag=self._headTag, bodyTag=self._bodyTag)
        self._replyTo.respondHTTP(status, body)

    def respond(self, response):
        self.respondHTTP("200 OK", response)

    # provides access to head and body tags of landing page
    def getHeadTag(self):
        return self._headTag
    def getBodyTag(self):
        return self._bodyTag

    def __getattr__(self, attrName):
        if attrName in self.__dict__:
            return self.__dict__[attrName]
        if hasattr(self.__class__, attrName):
            return getattr(self.__class__, attrName)
        # pass-through to replyTo object which this object is a proxy to
        return getattr(self._replyTo, attrName)

class WebRequestDispatcher(object):
    """
    Request dispatcher for HTTP requests.
    Contains registration and dispatching functionality.

    Single-state class--multiple instances share all data.
    This is because we're wrapping a singleton webserver.

    How to use:
    
    w = WebRequestDispatcher()
    w.listenOnPort(8888)
    def test(replyTo,**kw):
        print 'test got called with these options: %s' % str(kw)
        replyTo.respond('<html><body>Thank you for the yummy arguments: %s' % str(kw))
    w.registerGETHandler('test',test)
    while 1:
        w.poll()

    Browse to http://localhost:8888/test?foo=bar and see the result!
    """

    _shared_state = {}

    listenPort = None

    uriToHandler = {}

    requestTimeout = 10.0
    
    notify = notify

    def __new__(self, *a, **kw):
        obj = object.__new__(self, *a, **kw)
        obj.__dict__ = self._shared_state
        return obj

    def __init__(self, wantLandingPage = True):
        self.enableLandingPage(wantLandingPage)

    def listenOnPort(self,listenPort):
        """
        Start the web server listening if it isn't already.
        Singleton server, so ignore multiple listen requests.
        """
        if self.listenPort is None:
            self.listenPort = listenPort
            HttpRequest.HttpManagerInitialize(listenPort)
            self.notify.info("Listening on port %d" % listenPort)
        else:
            self.notify.warning("Already listening on port %d.  Ignoring request to listen on port %d." % (self.listenPort,listenPort))

    def invalidURI(self,replyTo,**kw):
        resp = "<html><body>Error 404</body></html>\r\n"
        replyTo.respondHTTP("404 Not Found",resp)
        self.notify.warning("%s - %s - 404" % (replyTo.getSourceAddress(),replyTo.getURI()))

    # access to head and body tags of landing page
    # only for 'returnsResponse' mode
    def getHeadTag(self):
        return self._headTag
    def getBodyTag(self):
        return self._bodyTag

    def handleGET(self,req):
        """
        Parse and dispatch a single GET request.
        Expects to receive a WebRequest object.
        """
        assert req.getRequestType() == "GET"

        self.landingPage.incrementQuickStat("Pages Served")
        
        uri = req.getURI()
        args = req.dictFromGET()

        # a more secure lookup, make sure number of args match function to be called
        callable,returnsResponse,autoSkin = self.uriToHandler.get((uri,len(args)),[self.invalidURI,False,False])
        if callable == self.invalidURI:
            # do less secure lookup that does not require number of arguments
            callable,returnsResponse,autoSkin = self.uriToHandler.get(uri, [self.invalidURI,False,False])

        if callable != self.invalidURI:
            self.notify.info("%s - %s - %s - 200" % (req.getSourceAddress(), uri, args))

        if returnsResponse:
            result = apply(callable,(),args)
            if autoSkin:
                self._headTag = ET.Element('head')
                self._bodyTag = ET.Element('body')
                req.respond(self.landingPage.skin(result,uri, headTag=self._headTag, bodyTag=self._bodyTag))
                del self._bodyTag
                del self._headTag
            else:
                req.respond(result)
        else:
            args["replyTo"] = SkinningReplyTo(req, self, uri, autoSkin)
            apply(callable,(),args)

    def poll(self):
        """
        Pump the web server, handle any incoming requests.
        This function should be called regularly, about 2-4
        calls/sec for current applications is a good number.
        """
        request = HttpRequest.HttpManagerGetARequest()
        while request is not None:
            wreq = WebRequest(request)
            wreq.logRequest()
            if wreq.getRequestType() == "GET":
                self.handleGET(wreq)
            else:
                self.notify.warning("Ignoring a non-GET request from %s: %s" % (request.GetSourceAddress(),request.GetRawRequest()))
                self.invalidURI(wreq)

            request = HttpRequest.HttpManagerGetARequest()


    def registerGETHandler(self,uri,handler,returnsResponse=False, autoSkin=False,numArgs=None):
        """
        Call this function to register a handler function to
        be called in response to a query to the given URI.

        GET options are translated into **kw arguments.
        Handler function should accept **kw in order to handle
        arbitrary queries.

        If returnsResponse is False, the request is left open after
        handler returns--handler or tasks it creates are responsible
        for fulfilling the query now or in the future.  Argument
        replyTo (a WebRequest) is guaranteed to be passed to the
        handler, and replyTo.respond must be called with an HTML
        response string to fulfill the query and close the socket.

        If returnsResponse is True, WebRequestDispatcher expects the
        handler to return its response string, and we will route the
        response and close the socket ourselves.  No replyTo argument
        is provided to the handler in this case.

        numArgs can be used for more secure uri handler lookup (helps avoid
        calling a handler when given a hacked/stale argument list), if this
        value is provided here, the handler is only called if the number of
        args received in the url matches, then it is up to the handler to
        verify the actual values of the args
        """
        if uri[0] != "/":
            uri = "/" + uri

        if numArgs != None:
            handlerSig = (uri,numArgs)
        else:
            handlerSig = uri

        if self.uriToHandler.get(handlerSig,None) is None:
            self.notify.info("Registered handler %s for URI %s." % (handler,handlerSig))
            self.uriToHandler[handlerSig] = [handler, returnsResponse, autoSkin]
        else:
            self.notify.warning("Attempting to register a duplicate handler for URI %s.  Ignoring." % handlerSig)

    def unregisterGETHandler(self,uri):
        if uri[0] != "/":
            uri = "/" + uri
        for currHandler in self.uriToHandler.keys():
            if type(currHandler) is types.TupleType and currHandler[0] == uri or currHandler == uri:
                self.uriToHandler.pop(currHandler)
        

    # -- Poll task wrappers --

    def pollHTTPTask(self,task):
        self.poll()
        return Task.again
        
    def startCheckingIncomingHTTP(self, interval=0.3):
        taskMgr.remove('pollHTTPTask')
        taskMgr.doMethodLater(interval,self.pollHTTPTask,'pollHTTPTask')

    def stopCheckingIncomingHTTP(self):
        taskMgr.remove('pollHTTPTask')


    # -- Landing page convenience functions --

    def enableLandingPage(self, enable):
        if enable:
            if not self.__dict__.has_key("landingPage"):
                self.landingPage = LandingPage()
                self.registerGETHandler("/", self._main, returnsResponse = True, autoSkin = True,
                                        numArgs = 0)
                self.registerGETHandler("/services", self._services, returnsResponse = True,
                                        autoSkin = True, numArgs = 0)
                self.registerGETHandler("/default.css", self._stylesheet, numArgs = 0)
                self.registerGETHandler("/favicon.ico", self._favicon, numArgs = 0)
                self.landingPage.addTab("Main", "/")
                self.landingPage.addTab("Services", "/services")
        else:
            self.landingPage = None
            self.unregisterGETHandler("/", numArgs = 0)
            self.unregisterGETHandler("/services", numArgs = 0)

        
    def _main(self):
        return self.landingPage.getMainPage()

    def _services(self):
        return self.landingPage.getServicesPage(self.uriToHandler)

    def _stylesheet(self,**kw):
        replyTo = kw.get("replyTo",None)
        assert replyTo is not None
        body = self.landingPage.getStyleSheet()
        replyTo.respondCustom("text/css",body)

    def _favicon(self,**kw):
        replyTo = kw.get("replyTo",None)
        assert replyTo is not None
        body = self.landingPage.getFavIcon()
        replyTo.respondCustom("image/x-icon",body)
