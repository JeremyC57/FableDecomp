// NScript::CIsThingConscious::Clone() const -- a new heap copy of the predicate (0 when the allocation fails)
// Leaf: ?Clone@CIsThingConscious@NScript@@UBEPAVCPredicateBase@2@XZ
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDDAE0, 48 bytes, thiscall, plain `ret` (virtual slot 1).
// Identity (proven): RTTI-less vtable 0x012C2FE8: slot 0 0x00CDDAB0, slot 1 0x00CDDAE0, slot 2 the landed ??_GCIsThingConscious@NScript@@
//   0x00CDDB10; Ego_r.pdb NScript::CIsThingConscious has exactly operator(), Clone and the destructor (??RCIsThingConscious@NScript@@UBE_NXZ,
//   ?Clone@CIsThingConscious@NScript@@UBEPAVCPredicateBase@2@XZ); its constructor from a CScriptThing is in Ego_r.pdb too.
// Layout: CPredicateBase vptr, CScriptThing Thing +0x04 (0x10 bytes; the model is the landed 0x00CDDB10 one).
// Header debt: engine/CScriptThing.h flattens the vptr into a data member; the slot 61 / 75 calls need a polymorphic local model.
// Callees: scalar operator new 0x00BFEA1A (0x10 bytes), CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90.
// Shape: the implicit copy constructor inlined: only the class's own vtable 0x012C2FE8 is stored, before the Thing copy.
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
namespace NScript {
class CPredicateBase {                                  // vtable 0x012C2FF4 (slots 0 / 1 pure)
public:
    CPredicateBase() { }
    virtual bool operator()() const = 0;
    virtual CPredicateBase* Clone() const = 0;
    virtual ~CPredicateBase() { }
};
class CIsThingConscious : public CPredicateBase {       // vtable 0x012C2FE8
public:
    CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
    virtual bool operator()() const;                    // slot 0, 0x00CDDAB0
    virtual CPredicateBase* Clone() const;              // slot 1, 0x00CDDAE0
    virtual ~CIsThingConscious() { }                    // slot 2, ??_G 0x00CDDB10 (landed)
    CScriptThing Thing;                                 // +0x04
};
}

NScript::CPredicateBase* NScript::CIsThingConscious::Clone() const
{
    return new CIsThingConscious(*this);
}