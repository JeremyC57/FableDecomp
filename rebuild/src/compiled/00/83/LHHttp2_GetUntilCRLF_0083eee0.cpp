// 0083eee0 LHHttp2::GetUntilCRLF
// Flags: /O2 /Oy /Gr
// Leaf: GetUntilCRLF
#include "engine/LHHttp2.h"
extern "C" void __cdecl free(void*);
// Layout: socket methods observed at008f8210,008f99a0; packet12 points at byte buffer+0.
struct Packet_0083eee0{char* start;char* finish;char* end;Packet_0083eee0(int first,int count,const char* value):start(0),finish(0),end(0){Insert(first,count,value);}void Insert(int,int,const char*);~Packet_0083eee0(){if(start)free(start);}};
extern "C" void* __cdecl malloc(unsigned int);
// Fidelity: genuine concrete char-vector insertion body for the address-taking constructor call (F1). Reads the input byte before any storage relocation.
#pragma auto_inline(off)
__declspec(noinline) void Packet_0083eee0::Insert(int position,int count,const char* value){
 if(count<=0)return;char fill=*value;int old=finish-start;int index=position ? position-(int)start:0;
 if(end-finish<count){int capacity=old+(old>count?old:count);char* data=(char*)malloc(capacity);int i;for(i=0;i<index;++i)data[i]=start[i];for(i=0;i<count;++i)data[index+i]=fill;for(i=index;i<old;++i)data[count+i]=start[i];if(start)free(start);start=data;finish=data+old+count;end=data+capacity;}
 else{int i;for(i=old;i>index;--i)start[i+count-1]=start[i-1];for(i=0;i<count;++i)start[index+i]=fill;finish+=count;}
}
#pragma auto_inline(on)
struct LSocket{bool IsConnected(int);bool Receive(Packet_0083eee0&);};
int __fastcall GetUntilCRLF(LHHttp2* self,void*,char* buffer,int& offset){
 bool seenCR=false;
 if(self->Host && self->Host->IsConnected(0)){
  unsigned short count=0;
  do{
   {char zero=0;Packet_0083eee0 packet(0,1,&zero);
   if(!self->Host->Receive(packet))return -102;
   buffer[offset]=*packet.start;
   if(buffer[offset]=='\r')seenCR=true;
   if(buffer[offset]=='\n' && seenCR){buffer[offset-1]=0;return 7;}
   ++offset;}
  }while(++count<1024);
 }
 return 6;
}