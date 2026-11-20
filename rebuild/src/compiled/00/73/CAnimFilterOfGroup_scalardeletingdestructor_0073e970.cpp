// CAnimFilterOfGroup::`scalar deleting destructor'(unsigned int) -- the implicit destructor (Group's CCharString::~CCharString 0x0099EAE0
//   out of line, then the inlined base destructor stores the CAnimFilterBase vtable), then the flag-tested operator delete
// Leaf: ??_GCAnimFilterOfGroup@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073E970, 39 bytes, thiscall `ret 4`.
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x4] ; call ~CCharString ; test byte ptr [esp+8],1 ; mov dword ptr [esi],offset CAnimFilterBase::`vftable' ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// Identity (NF120, PRIORITY q4104): slot 2 of CAnimFilterOfGroup's vtable 0x01267568 (rtti-vtables.tsv: slot 0 0x0073E940
//   operator(), slot 1 0x0073E9A0 Clone, slot 2 this address; no other vtable holds it), and Ego_r.pdb ??_GCAnimFilterOfGroup@@UAEPAXI@Z
//   (FableWin.pdb, a debug build, has the vector form ??_E plus ??1CAnimFilterOfGroup@@UAE@XZ). The +4 member is Group (CCharString,
//   Ego_r.pdb CAnimFilterOfGroup 12 bytes: vptr, Group +4, GroupCRC +8). Was the descriptive stand-in
//   CAnimFilterBase_Derived_0073e970 (catalog _unattributed::ScalarDeletingDestructor_CAnimFilterBase_Derived_0073e970) over a local
//   CAnimFilterBase model and a CListMember_0099eae0 stand-in for the CCharString member.
// Shape: VC7.1 emits a class's vtable, and this ??_G with it, in a TU that constructs the class (GOTCHAS nf86). The header's
//   FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR mode gives the inline (const CCharString&) constructor and no user-declared destructor
//   (implicit, as retail: no store of this class's vptr before ~Group), so the vtable, ??_G and the implicit destructor are pick-any COMDATs.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
#include "fable_compiler.h"
#include "engine/CAnimFilterOfGroup.h"
namespace {
// Fidelity: scaffolding, not retail: the smallest use of the header's inline constructor, so this TU emits CAnimFilterOfGroup's vtable
// and with it the scalar deleting destructor (both pick-any COMDATs). Internal to this TU.
void ReferenceAnimFilterOfGroupVtable(const CCharString& group)
{
    CAnimFilterOfGroup filter(group);
}
}
