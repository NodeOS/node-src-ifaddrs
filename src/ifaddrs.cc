#include <v8.h>
#include <node.h>
#include <sys/types.h>
#include <ifaddrs.h>

// http://man7.org/linux/man-pages/man7/netdevice.7.html
// http://linux.about.com/library/cmd/blcmdl7_netdevice.htm

using namespace v8;
using namespace node;

namespace NodeOS
{
  
  static Handle<Value> GetIFAddrs(const Arguments& args) {
    HandleScope scope;
    
    struct ifaddrs *ifap;
    
    getifaddrs(&ifap);
    
    Handle<Object> ifaces = Object::New();
    
    // while( ifap->ifa_next != NULL ){
      
      char* name = ifap->ifa_name;
      
      // convert socket to internet socket
      struct sockaddr_in *addr_ = (struct sockaddr_in *) ifap->ifa_addr;
      struct sockaddr_in *netm_ = (struct sockaddr_in *) ifap->ifa_addr;
      
      // get address as string
      char* addr = inet_ntoa(addr_->sin_addr);
      char* netm = inet_ntoa(netm_->sin_addr);
      
      Handle<Object> iface = Object::New();
      
      iface->Set( String::New("address"), String::New( addr ) );
      iface->Set( String::New("netmask"), String::New( netm ) );
      
      ifaces->Set(String::New(name), iface );
      ifap = ifap->ifa_next;
      
    // }
    
    return scope.Close( ifaces );
  }
  
  static void init(Handle<Object> target) {
    target->Set(String::NewSymbol("getifaddrs"),
      FunctionTemplate::New(GetIFAddrs)->GetFunction());
  }
}

NODE_MODULE(binding, NodeOS::init)
