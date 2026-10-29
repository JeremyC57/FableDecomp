#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#endif
#include <stdlib.h>
#include <string.h>
extern "C" int puts(const char*);
int main();
#define malloc fixtureMalloc
#define free fixtureFree
// 008f8a80 LHNetworkLib::LSocket::LSocket
// Leaf: ??0LSocket@LHNetworkLib@@QAE@XZ
// Flags: /GS
#include "rebuild_abi.h"
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
extern "C" unsigned int __cdecl strlen(const char*);
#pragma intrinsic(strlen)
namespace std {
template<class C,class I>class __char_traits_base{};
template<class C>class char_traits:public __char_traits_base<C,C>{};
template<>class char_traits<char>:public __char_traits_base<char,int>{public:static unsigned int __cdecl length(const char*s){return strlen(s);}};
template<class T>class allocator {
public:allocator(){}allocator(const allocator&){}
    T*allocate(unsigned int n,const void* =0);
    void deallocate(T*p,unsigned int){if(p)free(p);}
};
template<class T>T*allocator<T>::allocate(unsigned int n,const void*){return n?(T*)malloc(n*sizeof(T)):0;}
template<class V,class T,class A>class _STLP_alloc_proxy:public A {
public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}
};
template<class T,class A>class _String_base {
public:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;
    _String_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,(T*)0){}
    void _M_deallocate_block(){_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
    ~_String_base(){_M_deallocate_block();}
};
template<class C,class Traits,class A>class basic_string:protected _String_base<C,A> {
public:basic_string():_String_base<C,A>(A()){
        this->_M_start=this->_M_end_of_storage.allocate(8);this->_M_finish=this->_M_start;this->_M_end_of_storage._M_data=this->_M_start+8;_M_terminate_string();
    }
    ~basic_string(){}
    basic_string&assign(const C*,const C*);
    basic_string&operator=(const C*s){return assign(s,s+Traits::length(s));}
private:void _M_terminate_string(){*this->_M_finish=0;}
};
typedef basic_string<char,char_traits<char>,allocator<char> >string;
template<class T,class A>class _Vector_base{protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,(T*)0){}};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:vector():_Vector_base<T,A>(A()){}};
}
inline void*operator new(unsigned int,void*p){return p;}
namespace std {
template<class A,class B>inline void __fastcall _Construct(A*p,const B&v){new(p)A(v);}
struct _List_node_base{_List_node_base*_M_next;_List_node_base*_M_prev;};
template<class T>struct _List_node:public _List_node_base{T _M_data;};
struct _List_iterator_base {
    typedef int difference_type;_List_node_base*_M_node;_List_iterator_base(_List_node_base*n):_M_node(n){}
    void _M_incr(){_M_node=_M_node->_M_next;}
    bool operator!=(const _List_iterator_base&v)const{return _M_node!=v._M_node;}
};
template<class T>struct _Nonconst_traits{typedef T&reference;typedef T*pointer;};
template<class T>struct _Const_traits{typedef const T&reference;typedef const T*pointer;};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base {
    typedef _List_iterator<T,_Nonconst_traits<T> >iterator;typedef _List_iterator<T,Traits>_Self;
    _List_iterator(_List_node<T>*n):_List_iterator_base(n){}
    _List_iterator(const iterator&v):_List_iterator_base(v._M_node){}
    typename Traits::reference operator*()const{return ((_List_node<T>*)_M_node)->_M_data;}
    typename Traits::pointer operator->()const{return &((_List_node<T>*)_M_node)->_M_data;}
    _Self&operator++(){this->_M_incr();return *this;}
};
template<class T,class A>class _List_base{public:_List_base(const A&):_M_node(allocator<_List_node<T> >(),(_List_node<T>*)0){_List_node<T>*n=(_List_node<T>*)malloc(sizeof(_List_node<T>));n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;}_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > >_M_node;};
template<class T,class A=allocator<T> >class list:public _List_base<T,A> {
public:list(const A&a=A()):_List_base<T,A>(a){}typedef _List_iterator<T,_Nonconst_traits<T> >iterator;typedef _List_iterator<T,_Const_traits<T> >const_iterator;
    iterator begin(){return (_List_node<T>*)this->_M_node._M_data->_M_next;}
    iterator end(){return this->_M_node._M_data;}
    unsigned int size()const{int count=0;for(const_iterator first=begin();first!=end();++first)++count;return (unsigned int)count;}
protected:
    _List_node<T>*_M_create_node(const T&x){_List_node<T>*p=(_List_node<T>*)malloc(sizeof(_List_node<T>));_Construct(&p->_M_data,x);return p;}
public:
    iterator insert(iterator position,const T&x){_List_node<T>*tmp=_M_create_node(x);_List_node_base*n=position._M_node;_List_node_base*p=n->_M_prev;tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;return tmp;}
    void push_back(const T&x){insert(end(),x);}
    const_iterator begin()const{return (_List_node<T>*)this->_M_node._M_data->_M_next;}
    const_iterator end()const{return this->_M_node._M_data;}
};
}
struct WSAData {                                   // winsock WSAData, 0x190 bytes
    unsigned short wVersion;
    unsigned short wHighVersion;
    char           szDescription[257];
    char           szSystemStatus[129];
    unsigned short iMaxSockets;
    unsigned short iMaxUdpDg;
    char*          lpVendorInfo;
};
typedef WSAData WSADATA;
extern "C" int __stdcall fixtureWSAStartup(unsigned short wVersionRequested, WSADATA* lpWSAData); // IAT 0x01440510

struct in_addr {
    // Layout: the original S_un union is unread; retain its four-byte size and alignment.
    unsigned long unobservedAddress;
};
struct sockaddr_in {                               // winsock sockaddr_in, 16 bytes
    short          sin_family;
    unsigned short sin_port;
    in_addr        sin_addr;
    char           sin_zero[8];
};

struct _iobuf;
namespace LHNetworkLib {

enum LSOCKET_PROTOCOL {};
enum LSOCKET_FLAGS {};

struct LAddress {                                  // Ego_r.pdb names; retail 0x1C bytes (12-byte STLport string)
    std::string      ServerName;                   // +0x00
    unsigned long    ServerIP;                     // +0x0C
    unsigned short   ServerPort;                   // +0x10
    LSOCKET_PROTOCOL TransportProtocol;            // +0x14
    LSOCKET_FLAGS    TransportFlags;               // +0x18
    LAddress() { ClearData(); }
    void ClearData()
    {
        ServerName = "";
        ServerPort = 0;
        ServerIP = 0;
        TransportProtocol = (LSOCKET_PROTOCOL)0;
        TransportFlags = (LSOCKET_FLAGS)0;
    }
};

class LSocketServer;

class LSocket {friend int ::main();                                    // retail 0x9C bytes
private:
    LAddress          RemoteAddress;               // +0x00
    LAddress          LocalAddress;                // +0x1C
    unsigned int      Socket;                      // +0x38
    sockaddr_in       SocketInformation;           // +0x3C
    sockaddr_in       SocketInterfaceInformation;  // +0x4C
    bool              ListeningSocket;             // +0x5C
    LSOCKET_PROTOCOL  ConnectionMode;              // +0x60
    std::vector<char> SendOutputQueue;             // +0x64
    std::vector<char> InternalPacketBuffer;        // +0x70
    bool              NoListeningBinding;          // +0x7C
    LSocketServer*    ServerObject;                // +0x80
public:
    bool              ActivityPending;             // +0x84
    unsigned long     NrTotalDataSent;             // +0x88
    unsigned long     NrTotalDataReceived;         // +0x8C
    unsigned long     NrLastDataSent;              // +0x90
    unsigned long     NrLastDataReceived;          // +0x94
    _iobuf*           _DEBUGFP;                    // +0x98
    LSocket();
    static std::list<LSocket*>GlobalList;
private:
    bool InitWS();
    void ClearData();                              // 0x008FA120
};

} // namespace LHNetworkLib

__forceinline bool LHNetworkLib::LSocket::InitWS()
{
    if (GlobalList.size() == 0) {
        WSADATA wsaData;
        fixtureWSAStartup(0x0202, &wsaData);
    }
    GlobalList.push_back(this);
    return true;
}

LHNetworkLib::LSocket::LSocket()
{
    Socket = (unsigned int)-1;
    ClearData();
    InitWS();
    NoListeningBinding = false;
    ServerObject = 0;
    _DEBUGFP = 0;
}

FABLE_STATIC_ASSERT(sizeof(LHNetworkLib::LSocket)==156);

#undef malloc
#undef free
static void*fixtureAllocs[3];static unsigned int fixtureAllocationCount,fixtureFreeCount,fixtureAssigns,fixtureClears,fixtureStartups,fixtureErrors;
static unsigned int fixtureExpectedInitialSize;static int fixtureStartupReturn;static LHNetworkLib::LSocket*fixtureSocket;
extern "C" void*__cdecl fixtureMalloc(unsigned int n){void*p=malloc(n);if(fixtureSocket){if(fixtureAllocationCount>=3||n!=(fixtureAllocationCount<2?8U:12U))++fixtureErrors;else fixtureAllocs[fixtureAllocationCount]=p;++fixtureAllocationCount;}return p;}
extern "C" void __cdecl fixtureFree(void*p){if(fixtureSocket)++fixtureFreeCount;free(p);}
std::list<LHNetworkLib::LSocket*>LHNetworkLib::LSocket::GlobalList;
template<class C,class Traits,class A>std::basic_string<C,Traits,A>&std::basic_string<C,Traits,A>::assign(const C*first,const C*last){
 if(fixtureSocket){if(fixtureAssigns>=2||first!=last||*first||this->_M_start!=fixtureAllocs[fixtureAssigns]||this->_M_finish!=this->_M_start||this->_M_end_of_storage._M_data!=this->_M_start+8||*this->_M_start)++fixtureErrors;++fixtureAssigns;}
 return *this;
}
void LHNetworkLib::LSocket::ClearData(){
 if(this!=fixtureSocket||Socket!=0xffffffffU||fixtureAllocationCount!=2||fixtureAssigns!=2)++fixtureErrors;
 char*words[3]={0,0,0};if(memcmp(&SendOutputQueue,words,sizeof(words))||memcmp(&InternalPacketBuffer,words,sizeof(words)))++fixtureErrors;
 unsigned long*remote=reinterpret_cast<unsigned long*>(&RemoteAddress);unsigned long*local=reinterpret_cast<unsigned long*>(&LocalAddress);
 if(remote[3]||local[3]||RemoteAddress.ServerPort||LocalAddress.ServerPort||RemoteAddress.TransportProtocol!=(LSOCKET_PROTOCOL)0||LocalAddress.TransportProtocol!=(LSOCKET_PROTOCOL)0||RemoteAddress.TransportFlags!=(LSOCKET_FLAGS)0||LocalAddress.TransportFlags!=(LSOCKET_FLAGS)0)++fixtureErrors;
 ++fixtureClears;ListeningSocket=true;ConnectionMode=(LSOCKET_PROTOCOL)7;ActivityPending=true;NrTotalDataSent=17;NrTotalDataReceived=31;NrLastDataSent=41;NrLastDataReceived=53;
 memset(&SocketInformation,0x36,sizeof(SocketInformation));memset(&SocketInterfaceInformation,0x47,sizeof(SocketInterfaceInformation));
}
extern "C" int __stdcall fixtureWSAStartup(unsigned short version,WSADATA*data){
 if(version!=0x0202||fixtureExpectedInitialSize||fixtureClears!=1||fixtureAllocationCount!=2||fixtureStartups)++fixtureErrors;
 memset(data,0xa5,sizeof(*data));++fixtureStartups;return fixtureStartupReturn;
}
struct FixtureGuardedStorage{unsigned int before;unsigned long storage[156/4];unsigned int after;};
int main(){
 unsigned int cases=0;std::_List_node<LHNetworkLib::LSocket*>*sentinel=LHNetworkLib::LSocket::GlobalList._M_node._M_data;
 for(unsigned int count=0;count<=8;++count)for(unsigned int seed=0;seed<32;++seed)for(int response=0;response<3;++response){
  std::_List_node<LHNetworkLib::LSocket*>nodes[8];for(unsigned int i=0;i<count;++i){nodes[i]._M_prev=i?nodes+i-1:sentinel;nodes[i]._M_next=i+1<count?nodes+i+1:sentinel;nodes[i]._M_data=reinterpret_cast<LHNetworkLib::LSocket*>((i+1)*16);}
  sentinel->_M_next=count?nodes:sentinel;sentinel->_M_prev=count?nodes+count-1:sentinel;
  unsigned char beforeNodes[sizeof(nodes)];memcpy(beforeNodes,nodes,sizeof(nodes));FixtureGuardedStorage guarded;guarded.before=0x12345678U;guarded.after=0xfedcba98U;memset(guarded.storage,(int)seed,sizeof(guarded.storage));
  fixtureAllocationCount=fixtureFreeCount=fixtureAssigns=fixtureClears=fixtureStartups=fixtureErrors=0;fixtureExpectedInitialSize=count;fixtureStartupReturn=response==0?0:response==1?10091:-1;fixtureSocket=reinterpret_cast<LHNetworkLib::LSocket*>(guarded.storage);
  LHNetworkLib::LSocket*socket=new(guarded.storage)LHNetworkLib::LSocket();
  if(socket!=fixtureSocket||fixtureErrors||fixtureAllocationCount!=3||fixtureAssigns!=2||fixtureClears!=1||fixtureStartups!=(count==0?1U:0U)||fixtureFreeCount)return 1;
  if(socket->Socket!=0xffffffffU||socket->NoListeningBinding||socket->ServerObject||socket->_DEBUGFP||!socket->ListeningSocket||socket->ConnectionMode!=(LHNetworkLib::LSOCKET_PROTOCOL)7||!socket->ActivityPending||socket->NrTotalDataSent!=17||socket->NrTotalDataReceived!=31||socket->NrLastDataSent!=41||socket->NrLastDataReceived!=53)return 2;
  for(unsigned int i=0;i<16;++i)if(reinterpret_cast<unsigned char*>(&socket->SocketInformation)[i]!=0x36||reinterpret_cast<unsigned char*>(&socket->SocketInterfaceInformation)[i]!=0x47)return 3;
  std::_List_node<LHNetworkLib::LSocket*>*added=static_cast<std::_List_node<LHNetworkLib::LSocket*>*>(sentinel->_M_prev);if(added!=fixtureAllocs[2]||added->_M_data!=socket||added->_M_next!=sentinel||added->_M_prev!=(count?nodes+count-1:sentinel)||LHNetworkLib::LSocket::GlobalList.size()!=count+1)return 4;
  if(sentinel->_M_next!=(count?nodes:added))return 5;for(unsigned int i=0;i<count;++i){std::_List_node<LHNetworkLib::LSocket*>*old=reinterpret_cast<std::_List_node<LHNetworkLib::LSocket*>*>(beforeNodes)+i;if(nodes[i]._M_prev!=old->_M_prev||nodes[i]._M_data!=old->_M_data||nodes[i]._M_next!=(i+1==count?added:old->_M_next))return 6;}
  unsigned char expectedBytes[156];memset(expectedBytes,(int)seed,sizeof(expectedBytes));LHNetworkLib::LSocket*expected=reinterpret_cast<LHNetworkLib::LSocket*>(expectedBytes);
  for(unsigned int address=0;address<2;++address){char*stringWords[3]={(char*)fixtureAllocs[address],(char*)fixtureAllocs[address],(char*)fixtureAllocs[address]+8};LHNetworkLib::LAddress*dest=address?&expected->LocalAddress:&expected->RemoteAddress;memcpy(&dest->ServerName,stringWords,sizeof(stringWords));dest->ServerIP=0;dest->ServerPort=0;dest->TransportProtocol=(LHNetworkLib::LSOCKET_PROTOCOL)0;dest->TransportFlags=(LHNetworkLib::LSOCKET_FLAGS)0;}
  expected->Socket=0xffffffffU;memset(&expected->SocketInformation,0x36,16);memset(&expected->SocketInterfaceInformation,0x47,16);expected->ListeningSocket=true;expected->ConnectionMode=(LHNetworkLib::LSOCKET_PROTOCOL)7;memset(&expected->SendOutputQueue,0,12);memset(&expected->InternalPacketBuffer,0,12);expected->NoListeningBinding=false;expected->ServerObject=0;expected->ActivityPending=true;expected->NrTotalDataSent=17;expected->NrTotalDataReceived=31;expected->NrLastDataSent=41;expected->NrLastDataReceived=53;expected->_DEBUGFP=0;
  if(memcmp(expectedBytes,socket,156)||guarded.before!=0x12345678U||guarded.after!=0xfedcba98U)return 7;
  // Destroy the two genuine string members; the external engine socket destructor is outside this constructor test.
  socket->RemoteAddress.~LAddress();socket->LocalAddress.~LAddress();if(fixtureFreeCount!=2)return 8;
  fixtureSocket=0;free(added);sentinel->_M_next=sentinel;sentinel->_M_prev=sentinel;++cases;
 }
 if(cases!=864)return 9;free(sentinel);puts("008f8a80_TEST PASS");return 0;
}