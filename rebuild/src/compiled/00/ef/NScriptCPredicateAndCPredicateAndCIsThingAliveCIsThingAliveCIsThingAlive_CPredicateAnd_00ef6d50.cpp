// NScript::CPredicate_And<CPredicate_And<CIsThingAlive,CIsThingAlive>,CIsThingAlive>::~CPredicate_And() -- destroy the three-term "all things alive" script predicate: own vtable, the third CIsThingAlive term, then the nested two-term And inline (its vtable, its right and left terms), then the CPredicateBase vtable on both subobjects
// Leaf: ??1?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF6D50, 55 bytes (+9 int3), __fastcall, `ret`.
// Identity: FableWin.pdb ??1?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@UAE@XZ.
//   Vtables: 0x012F6628 = this instantiation (the landed copy ctor 0x00EF6F40 stores it), 0x012E1B70 = the nested
//   CPredicate_And<CIsThingAlive,CIsThingAlive>, 0x012C2FF4 = NScript::CPredicateBase. Label correction: the catalog's
//   five-level name is propagated -- two derived vtable stores and three leaf calls make this the three-term And.
// Callees: NScript::CIsThingAlive::~CIsThingAlive @ 0x00E241B0 (out of line, x3).
// Layout (Ego_r.pdb): vptr +0x00; Predicate1 = CPredicate_And<CIsThingAlive,CIsThingAlive> +0x04 (its vptr +0x04, terms
//   +0x08 and +0x18); Predicate2 = CIsThingAlive +0x28 (0x38 bytes).
//   CIsThingAlive : CPredicateBase { CScriptThing Thing +0x04 } (16 bytes); CScriptThing {vptr, PImp_Data, PImp_Info}.
// Shape: user-declared virtual destructors at every And level; Predicate2 (+0x28) goes first, then the nested And's inline
//   destructor (its vtable store at +0x04, its terms +0x18 and +0x08); the two inline CPredicateBase destructors reset
//   +0x04 and +0x00 (two immediate stores).
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

template class NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>,
    NScript::CIsThingAlive>;