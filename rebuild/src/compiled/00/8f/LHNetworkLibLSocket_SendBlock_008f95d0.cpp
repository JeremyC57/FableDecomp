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