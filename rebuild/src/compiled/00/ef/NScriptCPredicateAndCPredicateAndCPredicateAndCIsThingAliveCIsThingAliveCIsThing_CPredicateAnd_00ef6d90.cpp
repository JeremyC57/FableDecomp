// NScript::CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive,CIsThingAlive>,CIsThingAlive>,CIsThingAlive>::~CPredicate_And() -- destroy the four-term "all things alive" script predicate: own vtable, the fourth term, then the nested three- and two-term Ands inline, then the CPredicateBase vtable on all three subobjects
// Leaf: ??1?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF6D90, 70 bytes (+10 int3), __fastcall, `ret`.
// Identity: FableWin.pdb ??1?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@
//   VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@UAE@XZ. Vtables: 0x012F6780 = this instantiation (stored by the
//   landed ??_G 0x00EF6E40, which inlines this destructor), 0x012F6628 = the three-term And, 0x012E1B70 = the two-term
//   And, 0x012C2FF4 = NScript::CPredicateBase. The catalog name is right for this row.
// Callees: NScript::CIsThingAlive::~CIsThingAlive @ 0x00E241B0 (out of line, x4).
// Layout (Ego_r.pdb): outer vptr +0x00, middle +0x04, inner +0x08; the four leaves at +0x0C, +0x1C (inner), +0x2C
//   (middle) and +0x3C (outer); 0x4C bytes.
//   CIsThingAlive : CPredicateBase { CScriptThing Thing +0x04 } (16 bytes); CScriptThing {vptr, PImp_Data, PImp_Info}.
// Shape: user-declared virtual destructors at every level, each destroying its right term then its left one; the three
//   inline CPredicateBase destructors' vptr resets merge into one `mov eax,0x012C2FF4` and three stores (+8, +4, +0).
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

template class NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,
    NScript::CIsThingAlive>, NScript::CIsThingAlive>, NScript::CIsThingAlive>;