import dbus

import pygtk
import gtk

class SomeObject(dbus.Object):
    def __init__(self, service):
        dbus.Object.__init__(self, "/SomeObject", [self.HelloWorld], service)

    def HelloWorld(self, hello_message):
        print (hello_message)
        return "Hello from example-service.py"

session_bus = dbus.SessionBus()
service = dbus.Service("org.designfu.SampleService", bus=session_bus)
object = SomeObject(service)

gtk.main()
