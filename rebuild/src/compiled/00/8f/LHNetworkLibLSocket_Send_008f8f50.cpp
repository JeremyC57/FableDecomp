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