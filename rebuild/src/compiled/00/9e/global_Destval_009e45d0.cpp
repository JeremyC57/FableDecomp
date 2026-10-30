#include "rebuild_abi.h"
// Leaf: CRegisteredMusicEntry>_>

// _Dest_val<std::allocator<...>,...> @ 0x009e45d0
// Destructor forwarder: destroys the embedded member at +0x4, then
// tail-jumps to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x4]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct FableDestMember_009e45d0;
struct FableDestHost_009e45d0;

extern "C" void FABLE_FASTCALL FableMemberDtor_009e45d0(FableDestMember_009e45d0* m);

extern "C" void FABLE_FASTCALL FableDestVal_009e45d0(FableDestHost_009e45d0* self)
{
    FableMemberDtor_009e45d0((FableDestMember_009e45d0*)((char*)self + 0x4));
    ((CBaseClass*)(self))->CBaseClass::~CBaseClass();
}