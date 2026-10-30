// NScript::CPredicate_And::Clone() const -- copy the 4-term `alive && alive && ...` script predicate into a new heap object (`new CPredicate_And(*this)`)
// Leaf: ?Clone@?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@UBEPAVCPredicateBase@2@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EF70B0, 67 bytes, thiscall, plain `ret`; operator new 0x00BFEA1A (0x4c bytes).
// Instantiation: NScript::CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>, CIsThingAlive>.
// Identity (proven): slot 1 (Clone) of the RTTI-less predicate vtable 0x012F6780 [operator(), Clone, scalar deleting dtor], stored by the landed copy ctor 0x00EF6FE0 and the landed dtor 0x00EF6D90 of this nesting;
//   the Clone member of these nestings is in Ego_r.pdb (?Clone@?$CPredicate_And@...@NScript@@UBEPAVCPredicateBase@2@XZ, one per depth).
// Label: the manifest's "sub_00ef70b0" is a gap-scan placeholder.
// Callees: operator new 0x00BFEA1A, CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90, the three-term copy ctor 0x00EF6F40.
// Layout: vptr, Left +0x04, Right after it; CIsThingAlive = {vptr 0x012C32F0, CScriptThing +4} (0x10 bytes).
// Shape: the nested three-term predicate is copied by its out-of-line copy constructor 0x00EF6F40 (declared here as an explicit specialisation, as the landed 0x00EF6FE0 does), then the last term inline. The CPredicateBase vtable stores are dead. The copy constructor is `__forceinline`: with a plain inline one VC7.1 calls the nested copy out of line from Clone.
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
template <>
CPredicate_And<CPredicate_And2, CIsThingAlive>::CPredicate_And(const CPredicate_And<CPredicate_And2, CIsThingAlive>& other);   // 0x00EF6F40
template CPredicateBase* CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>, CIsThingAlive>::Clone() const;
}