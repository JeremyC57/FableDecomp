#include "rebuild_abi.h"
// Leaf: @FableDestVal_009e45d0@4

// _Dest_val<std::allocator<...>,...> @ 0x009e45d0
// Destructor forwarder: destroys the embedded member at +0x4, then
// tail-jumps to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x4]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
// Naming (NF142, 2026-11-20, q4299): the FableDestMember / FableDestHost stand-ins replaced by engine/CMaintainedEvent.h (Ego_r.pdb
//   CMaintainedEvent, 56 bytes: CBaseClass base +0, CInputEvent Event +4; the landed ??_GCMaintainedEvent 0x009E45F0, vtable 0x0129C0E0
//   slot 0, expands the same two calls): Event's destructor is ??1CInputEvent@@QAE@XZ 0x00A03B30 (Ego_r.pdb 0x00B6E5F0 + FableWin.pdb
//   0x0317B590), then ~CBaseClass 0x0099A2E0. The propagated label and the own extern "C" leaf are unchanged.
#include "engine/CMaintainedEvent.h"                   // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

extern "C" void FABLE_FASTCALL FableDestVal_009e45d0(CMaintainedEvent* self)
{
    self->Event.~CInputEvent();
    self->CBaseClass::~CBaseClass();
}
