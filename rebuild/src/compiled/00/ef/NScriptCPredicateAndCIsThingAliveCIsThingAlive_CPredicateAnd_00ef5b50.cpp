// NScript::CPredicate_And<CIsThingAlive,CIsThingAlive>::~CPredicate_And() -- destroy the two-term "both things alive" script predicate: own vtable, the right then the left CIsThingAlive term (out of line), then the CPredicateBase vtable
// Leaf: ??1?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF5B50, 33 bytes (+15 int3), __fastcall, `ret`.
// Identity: FableWin.pdb ??1?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@UAE@XZ. The vtable stored first,
//   0x012E1B70, is this instantiation's (the landed copy ctor 0x00EF5C00 stores it); 0x012C2FF4 is NScript::CPredicateBase.
//   Label correction: the catalog's five-level "~CPredicate_And<...CPredicate_And<CIsThingAlive,CIsThingAlive>...>" is
//   propagated -- one derived vtable store and two leaf calls make this the two-term And.
// Callees: NScript::CIsThingAlive::~CIsThingAlive @ 0x00E241B0 (out of line, x2).
// Layout (Ego_r.pdb): CPredicate_And<P1,P2> : CPredicateBase { P1 Predicate1 +0x04; P2 Predicate2 +0x14 } (0x24 bytes);
//   CIsThingAlive : CPredicateBase { CScriptThing Thing +0x04 } (16 bytes); CScriptThing {vptr, PImp_Data, PImp_Info}.
// Shape: a user-declared virtual destructor (its own vtable store), members destroyed in reverse order (Predicate2 at
//   +0x14, then Predicate1 at +0x04), then the base's inline user destructor resets the vptr to CPredicateBase's vtable.
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
namespace NScript {



template <class P1, class P2> class CPredicate_And : public CPredicateBase {
public:
    P1 Predicate1;                                       // +0x04
    P2 Predicate2;                                       // +0x04 + sizeof(P1)
    CPredicate_And(const P1& predicate1, const P2& predicate2) : Predicate1(predicate1), Predicate2(predicate2) { }
    virtual ~CPredicate_And() { }
    virtual bool operator()() const { return Predicate1() && Predicate2(); }
    virtual CPredicateBase* Clone() const { return new CPredicate_And(*this); }
};

}

template class NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>;