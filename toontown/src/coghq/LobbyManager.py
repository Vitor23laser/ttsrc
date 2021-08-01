from direct.showbase.ShowBaseGlobal import *
from toontown.toonbase import ToontownGlobals
from direct.distributed import DistributedObject
from direct.directnotify import DirectNotifyGlobal
from toontown.toonbase import TTLocalizer

class LobbyManager(DistributedObject.DistributedObject):
    __module__ = __name__
    notify = DirectNotifyGlobal.directNotify.newCategory('LobbyManager')
    SetFactoryZoneMsg = 'setFactoryZone'

    def __init__(self, cr):
        DistributedObject.DistributedObject.__init__(self, cr)

    def generate(self):
        self.notify.debug('generate')
        DistributedObject.DistributedObject.generate(self)

    def disable(self):
        self.notify.debug('disable')
        self.ignoreAll()

    def getSuitDoorOrigin(self):
        return 1

    def getBossLevel(self):
        return 0