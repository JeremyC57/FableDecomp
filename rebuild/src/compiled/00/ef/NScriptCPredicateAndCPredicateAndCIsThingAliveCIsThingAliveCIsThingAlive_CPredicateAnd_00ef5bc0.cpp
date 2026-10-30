// NScript::CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>::CPredicate_And(const CPredicate_And<CIsThingAlive, CIsThingAlive>& left, const CIsThingAlive& right) -- copy both operands of an alive-predicate conjunction
// Leaf: ??0?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@QAE@ABV?$CPredicate_And@VCIsThingAlive@NScript@@V12@@1@ABVCIsThingAlive@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ef5bc0, 51 bytes, thiscall, `ret 8`, returns this.
// Identity (proven): it stores the vtable 0x012F6628 (bound ??_7?$CPredicate_And<...> of this nesting depth) and copies the left operand through 0x00EF5C00 (the left type's copy constructor) and the right CIsThingAlive at 0x28; Ego_r.pdb holds the (const L&, const R&) constructors of these nestings (e.g. 0x00F70870 for depth 2); callers are the script threads that build `a && b && ...` alive predicates.
// Callees: the left operand's copy constructor 0x00EF5C00, CScriptThing(const CScriptThing&) 0x004ABE90 (the right operand's
//   thing; CIsThingAlive's vtable 0x012C32F0).
// Shape (family L145): the base CPredicateBase vtable store is dead; the left copy is an out-of-line call, the right
//   CIsThingAlive's implicit copy is inline around the out-of-line CScriptThing copy.
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
namespace NScript {
template <class L, class R> class CPredicate_And : public CPredicateBase {
public:
    L Left;                                          // +0x04
    R Right;
    CPredicate_And(const L& left, const R& right) : Left(left), Right(right) { }
    CPredicate_And(const CPredicate_And& other);     // out of line (0x00EF5C00 for the left type)
    virtual bool operator()() const { return Left() && Right(); }
    virtual CPredicateBase* Clone() const;
};
template CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>::CPredicate_And(const CPredicate_And<CIsThingAlive, CIsThingAlive>&, const CIsThingAlive&);
}