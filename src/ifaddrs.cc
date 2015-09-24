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
  Nan::HandleScope scope;

  struct ifaddrs *ifap;

  getifaddrs(&ifap);

  Local<Object> ifaces = Nan::New<Object>();

  // while( ifap->ifa_next != NULL ){

    char* name = ifap->ifa_name;

    // convert socket to internet socket
    struct sockaddr_in *addr_ = (struct sockaddr_in *) ifap->ifa_addr;
    struct sockaddr_in *netm_ = (struct sockaddr_in *) ifap->ifa_addr;

    // get address as string
    char* addr = inet_ntoa(addr_->sin_addr);
    char* netm = inet_ntoa(netm_->sin_addr);

    Local<Object> iface = Nan::New<Object>();

    iface->Set( Nan::New<String>("address").ToLocalChecked(), Nan::New<String>( addr ).ToLocalChecked() );
    iface->Set( Nan::New<String>("netmask").ToLocalChecked(), Nan::New<String>( netm ).ToLocalChecked() );

    ifaces->Set(Nan::New<String>(name).ToLocalChecked(), iface );
    ifap = ifap->ifa_next;

  // }
  info.GetReturnValue().Set(ifaces);
}

void init(Local<Object> exports) {
  exports->Set(Nan::New<String>("getifaddrs").ToLocalChecked(),
    Nan::New<FunctionTemplate>(GetIFAddrs)->GetFunction());
}

NODE_MODULE(binding, init)
