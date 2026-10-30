// NScript::CPredicate_And<CIsThingConscious, CIsThingConscious >::`scalar deleting destructor'(unsigned int flags) -- the And's vtable, Right then Left destroyed, the predicate base's vtable, then free this when bit 0 of flags is set
// Leaf: ??_G?$CPredicate_And@VCIsThingConscious@NScript@@V12@@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF8700, 53 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): slot 2 of the predicate vtable 0x012F687C; the member layout fixes the instantiation: Left at +4 is CIsThingConscious
//   (destructor 0x00EF7AB0), Right at +0x14 is a CIsThingConscious (so Left is 0x10 bytes); FableWin.pdb / Ego_r.pdb list this
//   instantiation's deleting destructor (FableWin.pdb ??_E (vector form) under the same name).
// Callees: ~CIsThingConscious 0x00EF7AB0 (Right), ~CIsThingConscious 0x00EF7AB0 (Left), operator delete 0x00BFE9BC.
// Shape (family K070, representative 0x00E245A0): the implicit member destructors in reverse order, CPredicateBase's inline
//   destructor storing its vtable 0x012C2FF4; the explicit instantiation emits the vtable and this ??_G.
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive, CIsThingConscious: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
#include "engine/CIsThingConscious.h"
namespace NScript {
template <class L, class R> class CPredicate_And : public CPredicateBase {
public:
    CPredicate_And(const L& left, const R& right) : Left(left), Right(right) { }
    virtual ~CPredicate_And() { }
    L Left;                                             // +0x04
    R Right;
};
template class CPredicate_And<CIsThingConscious, CIsThingConscious >;
}