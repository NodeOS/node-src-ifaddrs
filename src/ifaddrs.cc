#include <v8.h>
#include <node.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <nan.h>

// http://man7.org/linux/man-pages/man7/netdevice.7.html
// http://linux.about.com/library/cmd/blcmdl7_netdevice.htm

using namespace v8;
using namespace node;

NAN_METHOD(GetIFAddrs) {
  NanScope();

  struct ifaddrs *ifap;

  getifaddrs(&ifap);

  Handle<Object> ifaces = NanNew<Object>();

  // while( ifap->ifa_next != NULL ){

    char* name = ifap->ifa_name;

    // convert socket to internet socket
    struct sockaddr_in *addr_ = (struct sockaddr_in *) ifap->ifa_addr;
    struct sockaddr_in *netm_ = (struct sockaddr_in *) ifap->ifa_addr;

    // get address as string
    char* addr = inet_ntoa(addr_->sin_addr);
    char* netm = inet_ntoa(netm_->sin_addr);

    Handle<Object> iface = NanNew<Object>();

    iface->Set( NanNew<String>("address"), NanNew<String>( addr ) );
    iface->Set( NanNew<String>("netmask"), NanNew<String>( netm ) );

    ifaces->Set(NanNew<String>(name), iface );
    ifap = ifap->ifa_next;

  // }
  NanReturnValue(ifaces);
}

void init(Handle<Object> exports) {
  exports->Set(NanNew<String>("getifaddrs"),
    NanNew<FunctionTemplate>(GetIFAddrs)->GetFunction());
}

NODE_MODULE(binding, init)
