#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define FABLE_PORTABLE
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
#define __int64 long long
#endif
#define FABLE_RETAIL_LSOCKET_TEST_PASS23
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define private public
#define protected public
// LHNetworkLib::LSocket::Send -- send datagrams or queue and flush connected stream bytes.
// Leaf: ?Send@LSocket@LHNetworkLib@@QAE_NAAV?$vector@DV?$allocator@D@std@@@std@@ABULAddress@2@1@Z
// Flags: /GS
// Retail: full parent008f8f50,970bytes; reserved catch008f92ef is generated from the typed catch below.
#define FABLE_RETAIL_LSOCKET_SEND_PASS23
#define FABLE_RETAIL_LSOCKET_RECEIVE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_STLP_VECTOR_CLEAR_OUTOFLINE
#include "engine/LSocket.h"
#include <string.h>
#if defined(_MSC_VER) && !defined(FABLE_RETAIL_LSOCKET_TEST_PASS23)
#define FABLE_SOCKET_IMPORT __declspec(dllimport)
#else
#define FABLE_SOCKET_IMPORT
#endif
struct sockaddr;
extern "C" FABLE_SOCKET_IMPORT int FABLE_STDCALL sendto(unsigned int,const char*,int,int,const sockaddr*,int);
extern "C" FABLE_SOCKET_IMPORT int FABLE_STDCALL send(unsigned int,const char*,int,int);
extern "C" FABLE_SOCKET_IMPORT int FABLE_STDCALL closesocket(unsigned int);
namespace LHNetworkLib {
bool LSocket::Send(std::vector<char>& data,const LAddress& destination,const LAddress& bindinterface)
{
 long size_sent=0;
 NrLastDataSent=0;
 if(GetMode()==LSP_TCP_PACKET)return false;
 if(destination.ServerName[0]==0&&!IsConnected())return false;
 if(destination.TransportProtocol==LSP_UDP&&destination.ServerName[0]!=0){
  sockaddr_in udpsocket_information;
  if(!ConvertLAddress2Sockaddr(destination,udpsocket_information))return false;
  unsigned int udpsocket=CreateSocket(bindinterface,LSP_UDP);
  size_sent=sendto(udpsocket,&data[0],data.size(),0,(const sockaddr*)&udpsocket_information,sizeof(udpsocket_information));
  if(size_sent==-1)return false;
  closesocket(udpsocket);
  NrLastDataSent=size_sent;NrTotalDataSent+=size_sent;
 }
 else try {
  if(IsWriteable(0)){
   if(SendOutputQueue.size()!=0){
    if(data.size()!=0){
     int oldsize=SendOutputQueue.size();
     SendOutputQueue.resize(oldsize+data.size());
     memcpy(&SendOutputQueue[oldsize],&data[0],data.size());
     NrLastDataSent=data.size();NrTotalDataSent+=data.size();
    }
    size_sent=send(Socket,&SendOutputQueue[0],SendOutputQueue.size(),0);
    if(size_sent==-1)return false;
    if(size_sent==(long)SendOutputQueue.size()){
     SendOutputQueue.clear();SendOutputQueue.resize(0);
    }
    else if(size_sent>0){
     int tosent=SendOutputQueue.size()-size_sent;
     std::vector<char> tempbuf;
     tempbuf.resize(tosent);
     memcpy(&tempbuf[0],&SendOutputQueue[size_sent],tosent);
     SendOutputQueue.clear();SendOutputQueue.resize(tosent);
     memcpy(&SendOutputQueue[0],&tempbuf[0],tosent);
     NeedToFlush=true;
    }
   }else{
    size_sent=send(Socket,&data[0],data.size(),0);
    if(size_sent==-1)return false;
    if(size_sent<(long)data.size()){
     unsigned long newsize=data.size()-size_sent;
     unsigned long oldsize=SendOutputQueue.size();
     SendOutputQueue.resize(oldsize+newsize);
     memcpy(&SendOutputQueue[oldsize],&data[size_sent],newsize);
     NeedToFlush=true;
    }
    NrLastDataSent=size_sent;NrTotalDataSent+=size_sent;
   }
  }else if(data.size()!=0){
   unsigned long oldsize=SendOutputQueue.size();
   SendOutputQueue.resize(oldsize+data.size());
   memcpy(&SendOutputQueue[oldsize],&data[0],data.size());
   NeedToFlush=true;
   NrLastDataSent=data.size();NrTotalDataSent+=data.size();
  }
 }catch(LSocketConnError& er){Close();return false;}
 return true;
}
}

#undef private
#undef protected
namespace std {
// Controlled external insertion contract on the genuine shared vector.
template<class T,class A> void vector<T,A>::_M_fill_insert(iterator pos,size_type n,const T& value){if(!n)return;size_type before=pos-this->_M_start,old=size();T copy=value;T* p=(T*)malloc((old+n)*sizeof(T));assert(p);size_type i;for(i=0;i<before;++i)new(p+i)T(this->_M_start[i]);for(i=0;i<n;++i)new(p+before+i)T(copy);for(i=before;i<old;++i)new(p+n+i)T(this->_M_start[i]);_Destroy(this->_M_start,this->_M_finish);free(this->_M_start);this->_M_start=p;this->_M_finish=p+old+n;this->_M_end_of_storage._M_data=p+old+n;}
template<class T,class A> void vector<T,A>::resize(size_type n){resize(n,T());}
template<class T,class A> void vector<T,A>::clear(){erase(begin(),end());}
}
static LHNetworkLib::LSocket* Current;
static const LHNetworkLib::LAddress *Destination,*Interface;
static bool Writable,Resolve,Datagram,ThrowNow;
static int SendResult,SendCalls,CloseCalls,CreateCalls,ResolveCalls,WriteableCalls;
static unsigned char Input[64],Queued[64],Wire[128];
static unsigned InputN,QueueN,WireN;
namespace LHNetworkLib {
bool LSocket::NeedToFlush=false;
bool LSocket::IsWriteable(long timeout){assert(this==Current&&timeout==0);++WriteableCalls;
#if !defined(_MSC_VER) || defined(_CPPUNWIND)
 if(ThrowNow)throw LSocketConnError();
#endif
 return Writable;
}
bool LSocket::ConvertLAddress2Sockaddr(const LAddress& a,sockaddr_in& b){assert(&a==Destination);++ResolveCalls;memset(&b,0,sizeof(b));b.sin_family=2;b.sin_port=0x1234;b.sin_addr.S_un.S_addr=0x01020304;return Resolve;}
unsigned int LSocket::CreateSocket(const LAddress&a,LSOCKET_PROTOCOL p){assert(&a==Interface&&p==LSP_UDP);++CreateCalls;return 81;}
bool LSocket::Close(){assert(this==Current);++CloseCalls;return true;}
}
extern "C" int FABLE_STDCALL closesocket(unsigned int socket){assert(socket==81&&Datagram&&SendCalls==1);++CloseCalls;return 0;}
extern "C" int FABLE_STDCALL sendto(unsigned int socket,const char* p,int n,int flags,const sockaddr* address,int bytes){assert(socket==81&&Datagram&&Resolve&&n==(int)InputN&&flags==0&&bytes==(int)sizeof(sockaddr_in));const sockaddr_in* a=(const sockaddr_in*)address;assert(a->sin_family==2&&a->sin_port==0x1234&&a->sin_addr.S_un.S_addr==0x01020304);assert(!n||memcmp(p,Input,n)==0);++SendCalls;return SendResult;}
extern "C" int FABLE_STDCALL send(unsigned int socket,const char* p,int n,int flags){assert(socket==Current->Socket&&!Datagram&&Writable&&flags==0);assert(n==(int)WireN&&(!n||memcmp(p,Wire,n)==0));++SendCalls;return SendResult;}
static void Fill(std::vector<char>& v,const unsigned char* p,unsigned n){v.clear();v.resize(n);for(unsigned i=0;i<n;++i)v[i]=(char)p[i];}
static void CheckQueue(const std::vector<char>&v,const unsigned char*p,unsigned n){assert(v.size()==n);for(unsigned i=0;i<n;++i)assert((unsigned char)v[i]==p[i]);}
int main(){unsigned cases=0,exception_cases=0;
 for(unsigned mode=0;mode<4;++mode)for(unsigned named=0;named<2;++named)for(unsigned socket_ok=0;socket_ok<2;++socket_ok)for(unsigned writable=0;writable<2;++writable)for(unsigned resolve=0;resolve<2;++resolve)for(unsigned q=0;q<3;++q)for(unsigned n=0;n<3;++n)for(unsigned result=0;result<5;++result){
  LHNetworkLib::LSocket socket;LHNetworkLib::LAddress dest,iface;std::vector<char> data;
  Current=&socket;Destination=&dest;Interface=&iface;Writable=writable!=0;Resolve=resolve!=0;ThrowNow=false;
  InputN=n*3;QueueN=q*4;for(unsigned i=0;i<InputN;++i)Input[i]=(unsigned char)(31+i);for(unsigned i=0;i<QueueN;++i)Queued[i]=(unsigned char)(71+i);
  Fill(data,Input,InputN);Fill(socket.SendOutputQueue,Queued,QueueN);socket.Socket=socket_ok?7:(unsigned int)-1;socket.ConnectionMode=(LHNetworkLib::LSOCKET_PROTOCOL)mode;socket.NrTotalDataSent=100;socket.NrLastDataSent=99;socket.NrTotalDataReceived=43;socket.NrLastDataReceived=27;
  dest.ServerName[0]=named?'x':0;dest.TransportProtocol=mode==2?LHNetworkLib::LSP_UDP:LHNetworkLib::LSP_TCP;
  WireN=QueueN+InputN;if(QueueN)memcpy(Wire,Queued,QueueN);if(InputN)memcpy(Wire+QueueN,Input,InputN);
  Datagram=mode==2&&named;unsigned amount=Datagram?InputN:WireN;SendResult=result==0?-1:result==1?0:result==2?(int)(amount/2):result==3?(int)amount:(amount?(int)amount-1:0);
  SendCalls=CloseCalls=CreateCalls=ResolveCalls=WriteableCalls=0;LHNetworkLib::LSocket::NeedToFlush=false;
  unsigned char expected[128];unsigned expected_n=QueueN;if(QueueN)memcpy(expected,Queued,QueueN);unsigned long total=100,last=0;bool wanted=true,flush=false;
  if(mode==3||(!named&&!socket_ok))wanted=false;
  else if(Datagram){if(!Resolve||SendResult==-1)wanted=false;else{last=(unsigned long)SendResult;total+=last;}}
  else if(Writable){
   if(QueueN){if(InputN){memcpy(expected+expected_n,Input,InputN);expected_n+=InputN;last=InputN;total+=InputN;}
    if(SendResult==-1)wanted=false;else if((unsigned)SendResult==expected_n)expected_n=0;else if(SendResult>0){expected_n-=(unsigned)SendResult;memmove(expected,expected+SendResult,expected_n);flush=true;}
   }else{if(SendResult==-1)wanted=false;else{if((unsigned)SendResult<InputN){expected_n=InputN-(unsigned)SendResult;memcpy(expected,Input+SendResult,expected_n);flush=true;}last=(unsigned long)SendResult;total+=last;}}
  }else if(InputN){memcpy(expected+expected_n,Input,InputN);expected_n+=InputN;flush=true;last=InputN;total+=InputN;}
  bool actual=socket.Send(data,dest,iface);assert(actual==wanted&&socket.NrLastDataSent==last&&socket.NrTotalDataSent==total&&socket.NrTotalDataReceived==43&&socket.NrLastDataReceived==27&&LHNetworkLib::LSocket::NeedToFlush==flush);CheckQueue(socket.SendOutputQueue,expected,expected_n);CheckQueue(data,Input,InputN);
  if(mode==3||(!named&&!socket_ok))assert(SendCalls==0&&ResolveCalls==0&&CreateCalls==0&&CloseCalls==0&&WriteableCalls==0);
  else if(Datagram)assert(ResolveCalls==1&&CreateCalls==(Resolve?1:0)&&SendCalls==(Resolve?1:0)&&CloseCalls==(Resolve&&SendResult!=-1?1:0)&&WriteableCalls==0);
  else assert(WriteableCalls==1&&SendCalls==(Writable?1:0)&&ResolveCalls==0&&CreateCalls==0&&CloseCalls==0);
  ++cases;
 }
#if !defined(_MSC_VER) || defined(_CPPUNWIND)
 {LHNetworkLib::LSocket socket;LHNetworkLib::LAddress dest,iface;std::vector<char> data;Current=&socket;Destination=&dest;Interface=&iface;dest.TransportProtocol=LHNetworkLib::LSP_TCP;socket.Socket=7;socket.ConnectionMode=LHNetworkLib::LSP_TCP;socket.NrTotalDataSent=123;socket.NrLastDataSent=42;CloseCalls=0;ThrowNow=true;assert(!socket.Send(data,dest,iface));assert(CloseCalls==1&&socket.NrTotalDataSent==123&&socket.NrLastDataSent==0);++exception_cases;ThrowNow=false;}
 assert(exception_cases==1);
#endif
 assert(cases==2880);puts("008f92ef_TEST PASS");return 0;
}