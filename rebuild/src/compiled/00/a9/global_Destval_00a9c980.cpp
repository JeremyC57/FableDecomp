#include "rebuild_abi.h"
// Leaf: CRegisteredMusicEntry>_>

// _Dest_val<std::allocator<...>,...> @ 0x00a9c980
// Destructor forwarder: destroys the embedded member at +0x40, then
// tail-jumps to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x40]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
class CChunkedFileSuperChunk {
public:
    virtual ~CChunkedFileSuperChunk();   // 0x00AA8490
};
struct FableDestMember_00a9c980;
struct FableDestHost_00a9c980;

extern "C" void FABLE_FASTCALL FableMemberDtor_00a9c980(FableDestMember_00a9c980* m);

extern "C" void FABLE_FASTCALL FableDestVal_00a9c980(FableDestHost_00a9c980* self)
{
    FableMemberDtor_00a9c980((FableDestMember_00a9c980*)((char*)self + 0x40));
    ((CChunkedFileSuperChunk*)(self))->CChunkedFileSuperChunk::~CChunkedFileSuperChunk();
}