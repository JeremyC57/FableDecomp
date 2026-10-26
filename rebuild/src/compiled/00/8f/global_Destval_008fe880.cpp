#include "rebuild_abi.h"
// Leaf: CRegisteredMusicEntry>_>

// _Dest_val<std::allocator<...>,...> @ 0x008fe880
// Destructor forwarder: destroys the embedded member at +0x24, then
// tail-jumps to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x24]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();   // 0x008FD288
};
struct FableDestMember_008fe880;
struct FableDestHost_008fe880;

extern "C" void FABLE_FASTCALL FableMemberDtor_008fe880(FableDestMember_008fe880* m);

extern "C" void FABLE_FASTCALL FableDestVal_008fe880(FableDestHost_008fe880* self)
{
    FableMemberDtor_008fe880((FableDestMember_008fe880*)((char*)self + 0x24));
    ((CAIStateGroupBase*)(self))->CAIStateGroupBase::~CAIStateGroupBase();
}