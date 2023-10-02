from coapthon.server.coap import CoAP
from coapthon.resources.resource import Resource


class CoAPServer(CoAP):
    def __init__(self, host, port):
        CoAP.__init__(self, (host, port))
        self.add_resource('tagID/', BasicResource())
        self.add_resource('bath/', BasicResource())


class BasicResource(Resource):
    def __init__(self, name="BasicResource", coap_server=None):
        super(BasicResource, self).__init__(name, coap_server, visible=True,
                                            observable=True, allow_children=True)
        self.payload = ""

    def render_GET(self, request):
        result = BasicResource()
        result.payload = self.payload
        self.payload = None
        return result

    def render_PUT(self, request):
        self.payload = request.payload
        return self


def main():
    server = CoAPServer("0.0.0.0", 5683)
    try:
        server.listen(10)
    except KeyboardInterrupt:
        print ("Server Shutdown")
        server.close()
        print ("Exiting...")

if __name__ == '__main__':
    main()
