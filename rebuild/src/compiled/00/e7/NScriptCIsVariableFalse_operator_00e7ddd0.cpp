// NScript::CIsVariableFalse::operator()() const -- the predicate holds while the watched bool is false
// Leaf: ??RCIsVariableFalse@NScript@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7DDD0, 15 bytes, thiscall, plain `ret` (virtual slot 0).
// Identity (proven): RTTI-less vtables 0x012EA430 (CPredicate_And<CIsThingAlive, CIsVariableFalse>: operator() 0x00E7DE30,
//   Clone 0x00E7E060, ??_E 0x00E7DFE0) and 0x012EA43C (CIsVariableFalse: operator() 0x00E7DDD0, Clone 0x00E7DDE0,
//   destructor 0x00E7DE10), both stored by the landed CPredicate_And<CIsThingAlive, CIsVariableFalse>::CPredicate_And 0x00E7E020
//   (FableWin.pdb ??0?$CPredicate_And@VCIsThingAlive@NScript@@VCIsVariableFalse@2@@NScript@@QAE@...); predicate slots are
//   operator(), Clone, destructor (the landed CIsThingConscious rows 0x00CDDAB0 / 0x00CDDAE0). Ego_r.pdb has
//   ??RCIsVariableFalse@NScript@@UBE_NXZ, ?Clone@CIsVariableFalse@NScript@@UBEPAVCPredicateBase@2@XZ and
//   ?Clone@?$CPredicate_And@VCIsThingAlive@NScript@@VCIsVariableFalse@2@@NScript@@UBEPAVCPredicateBase@2@XZ.
// Layout (Ego_r.pdb): CIsVariableFalse {vptr; const bool* Variable +4} (8 bytes); CIsThingAlive {vptr 0x012C32F0;
//   CScriptThing Thing +4} (16 bytes); CPredicate_And<P1, P2> {vptr; P1 +4; P2 +0x14} (0x1c bytes).
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

namespace NScript {
class CPredicateBase {                                  // vtable 0x012C2FF4
public:
    virtual bool operator()() const = 0;
    virtual CPredicateBase* Clone() const = 0;
    virtual ~CPredicateBase() { }
};
class CIsThingAlive : public CPredicateBase {           // vtable 0x012C32F0
public:
    CScriptThing Thing;                                 // +0x04
    CIsThingAlive(const CScriptThing& thing);
    virtual ~CIsThingAlive();
    virtual bool operator()() const;
    virtual CPredicateBase* Clone() const;
};
class CIsVariableFalse : public CPredicateBase {        // vtable 0x012EA43C
public:
    const bool* Variable;                               // +0x04
    CIsVariableFalse(const bool& variable) : Variable(&variable) { }
    virtual bool operator()() const;                    // slot 0, 0x00E7DDD0
    virtual CPredicateBase* Clone() const;              // slot 1, 0x00E7DDE0
    virtual ~CIsVariableFalse() { }                     // slot 2, 0x00E7DE10
};
template <class P1, class P2> class CPredicate_And : public CPredicateBase {
public:
    P1 Predicate1;                                      // +0x04
    P2 Predicate2;                                      // +0x14
    CPredicate_And(const P1& predicate1, const P2& predicate2) : Predicate1(predicate1), Predicate2(predicate2) { }
    virtual bool operator()() const;                    // slot 0, 0x00E7DE30
    virtual CPredicateBase* Clone() const;              // slot 1, 0x00E7E060
    virtual ~CPredicate_And() { }                       // slot 2, 0x00E7DFE0
};
}

bool NScript::CIsVariableFalse::operator()() const
{
    return !*Variable;
}