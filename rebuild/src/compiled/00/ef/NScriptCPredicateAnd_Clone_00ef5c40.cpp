// NScript::CPredicate_And::Clone() const -- copy the 3-term `alive && alive && ...` script predicate into a new heap object (`new CPredicate_And(*this)`)
// Leaf: ?Clone@?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@UBEPAVCPredicateBase@2@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EF5C40, 100 bytes, thiscall, plain `ret`; operator new 0x00BFEA1A (0x38 bytes).
// Instantiation: NScript::CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>.
// Identity (proven): slot 1 (Clone) of the RTTI-less predicate vtable 0x012F6628 [operator(), Clone, scalar deleting dtor], stored by the landed (left, right) ctor 0x00EF5BC0 and the landed copy ctor 0x00EF6F40 of this nesting;
//   the Clone member of these nestings is in Ego_r.pdb (?Clone@?$CPredicate_And@...@NScript@@UBEPAVCPredicateBase@2@XZ, one per depth).
// Label: the manifest's "sub_00ef5c40" is a gap-scan placeholder.
// Callees: operator new 0x00BFEA1A, CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90.
// Layout: vptr, Left +0x04, Right after it; CIsThingAlive = {vptr 0x012C32F0, CScriptThing +4} (0x10 bytes).
// Shape: the whole copy is inline: own vtable, the nested two-term predicate's vtable, then the three CIsThingAlive terms (vtable store + out-of-line CScriptThing copy each). The CPredicateBase vtable stores are dead. The copy constructor is `__forceinline`: with a plain inline one VC7.1 calls the nested copy out of line from Clone.
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
namespace NScript {
class CPredicateBase {
public:
    virtual bool operator()() const = 0;             // slot 0
    virtual CPredicateBase* Clone() const = 0;       // slot 1
    virtual ~CPredicateBase() { }                    // slot 2
};
class CIsThingAlive : public CPredicateBase {         // 16 bytes, vtable 0x012C32F0
public:
    CScriptThing Thing;                              // +0x04
    CIsThingAlive(const CIsThingAlive& other) : Thing(other.Thing) { }
    virtual bool operator()() const;
    virtual CPredicateBase* Clone() const;
};
template <class L, class R> class CPredicate_And : public CPredicateBase {
public:
    L Left;                                          // +0x04
    R Right;
    __forceinline CPredicate_And(const CPredicate_And& other) : Left(other.Left), Right(other.Right) { }
    virtual bool operator()() const { return Left() && Right(); }
    virtual CPredicateBase* Clone() const { return new CPredicate_And(*this); }
};
typedef CPredicate_And<CIsThingAlive, CIsThingAlive> CPredicate_And2;
template CPredicateBase* CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>::Clone() const;
}