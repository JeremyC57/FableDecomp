// CSlotTable_00c39f10::CSlotTable_00c39f10() -- store the vtable, zero the 3-word slot array at +0x04 with memset, then zero the word at 0x10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C39F10, length 25 bytes (+ int3 padding). No callees.
// Leaf: ??0CSlotTable_00c39f10
// The catalog label "CVsyncCallbackTimerInfo::CVsyncCallbackTimerInfo" is propagated over byte-identical bodies; this is the
// inline base constructor of a sub-object built by the CTCTavernGameSpotTheAddition constructor (callers 0x00C3916B/0x00C3A723: each
// constructs this base then a member at +0x14/+0x18 with (owner, 1)). Vtable 0x012B5494 has no RTTI row, so the class name is descriptive.
// Shape: the vtable store comes FIRST, then `xor ecx,ecx ; lea edx,[eax+4]` and three/four stores through edx: the intrinsic
// memset of the array (its destination pointer is materialised; a plain init list, an array loop, a pointer local, an inline
// Clear() and the STLport vector model all fold the stores onto eax and sink the vtable store below the xor). The trailing word
// is a body assignment after the memset so it shares the zero register.
#include <string.h>

struct CSlotTable_00c39f10 {
    virtual void v00();
    long m_slots[3];                 // +0x04
    long m_count;                     // 0x10
    CSlotTable_00c39f10();
};

CSlotTable_00c39f10::CSlotTable_00c39f10()
{
    memset(m_slots, 0, sizeof(m_slots));
    m_count = 0;
}
