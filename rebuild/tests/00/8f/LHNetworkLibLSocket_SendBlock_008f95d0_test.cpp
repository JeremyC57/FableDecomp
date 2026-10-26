#include <assert.h>
#include <stdio.h>
#include <string.h>
// BEGIN exact source snapshot SHA256 e48bebba414ee862283d9bb6cda8f31fb7b354fdbd1cf241ee8ac12345bc4679
// 008f95d0 LHNetworkLib::LSocket::SendBlock
// Flags: /O2 /Oy
#include <stdlib.h>
// Layout: retail state+0x60; LAddress28-byte temporaries with vector+0..8; ctor00841e90.
namespace LHNetworkLib {
struct ByteBuffer {char*first,*last,*limit;void Fill(char*pos,unsigned n,const char&v);};
struct LAddress {ByteBuffer text;int tail[4];LAddress();~LAddress(){if(text.first)free(text.first);}};
class LSocket { public:char prefix[96];int state;bool Send(const ByteBuffer&,const LAddress&,const LAddress&);bool SendBlock(ByteBuffer&);};
bool LSocket::SendBlock(ByteBuffer&data){if(state!=3)return false;state=1;bool okay=false;ByteBuffer header={0,0,0};char zero=0;header.Fill(0,4,zero);*(unsigned*)header.first=data.last-data.first;if(Send(header,LAddress(),LAddress())){if(Send(data,LAddress(),LAddress()))okay=true;}state=3;free(header.first);return okay;}

}

// END exact source snapshot
using namespace LHNetworkLib;
void ByteBuffer::Fill(char*,unsigned n,const char&v){first=(char*)malloc(n);last=limit=first+n;memset(first,v,n);}
LAddress::LAddress(){text.first=text.last=text.limit=0;for(int i=0;i<4;++i)tail[i]=0;}
int calls=0,failAt=0;unsigned length=0;
bool LSocket::Send(const ByteBuffer&b,const LAddress&,const LAddress&){assert(state==1);++calls;if(calls==1)length=*(unsigned*)b.first;else assert((unsigned)(b.last-b.first)==length);return calls!=failAt;}
int main(){LSocket s={0};char a[3]={1,2,3};ByteBuffer b={a,a+3,a+3};assert(!s.SendBlock(b)&&calls==0);s.state=3;assert(s.SendBlock(b)&&s.state==3&&calls==2&&length==3);calls=0;failAt=1;assert(!s.SendBlock(b)&&calls==1&&s.state==3);calls=0;failAt=2;assert(!s.SendBlock(b)&&calls==2&&s.state==3);puts("008f95d0_TEST PASS");}