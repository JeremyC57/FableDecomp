// NScript::CPredicate_And::Clone() const -- copy the 5-term `alive && alive && ...` script predicate into a new heap object (`new CPredicate_And(*this)`)
// Leaf: ?Clone@?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@UBEPAVCPredicateBase@2@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EF7100, 93 bytes, thiscall, plain `ret`; operator new 0x00BFEA1A (0x60 bytes).
// Instantiation: NScript::CPredicate_And<CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>, CIsThingAlive>, CIsThingAlive>.
// Identity (proven): slot 1 (Clone) of the RTTI-less predicate vtable 0x012F678C [operator(), Clone, scalar deleting dtor], stored by the landed copy ctor 0x00EF7060, the landed (left, right) ctor 0x00EF6FA0 and the landed dtor 0x00EF6DE0 of this nesting;
//   the Clone member of these nestings is in Ego_r.pdb (?Clone@?$CPredicate_And@...@NScript@@UBEPAVCPredicateBase@2@XZ, one per depth).
// Label: the manifest's "sub_00ef7100" is a gap-scan placeholder.
// Callees: operator new 0x00BFEA1A, CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90, the three-term copy ctor 0x00EF6F40.
// Layout: vptr, Left +0x04, Right after it; CIsThingAlive = {vptr 0x012C32F0, CScriptThing +4} (0x10 bytes).
// Shape: the nested four-term copy is inline (its vtable 0x012F6780 at +4), the three-term copy inside it is the out-of-line 0x00EF6F40, then the two last terms inline. The CPredicateBase vtable stores are dead. The copy constructor is `__forceinline`: with a plain inline one VC7.1 calls the nested copy out of line from Clone.
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
namespace NScript {
template <class L, class R> class CPredicate_And : public CPredicateBase {
public:
    L Left;                                          // +0x04
    R Right;
    __forceinline CPredicate_And(const CPredicate_And& other) : Left(other.Left), Right(other.Right) { }
    virtual bool operator()() const { return Left() && Right(); }
    virtual CPredicateBase* Clone() const { return new CPredicate_And(*this); }
};
typedef CPredicate_And<CIsThingAlive, CIsThingAlive> CPredicate_And2;
template <>
CPredicate_And<CPredicate_And2, CIsThingAlive>::CPredicate_And(const CPredicate_And<CPredicate_And2, CIsThingAlive>& other);   // 0x00EF6F40
template CPredicateBase* CPredicate_And<CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>, CIsThingAlive>, CIsThingAlive>::Clone() const;
}