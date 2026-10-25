// NScript::CPredicate_And::CPredicate_And(const NScript::CIsThingAlive& predicate1, const NScript::CIsVariableFalse& predicate2) -- CPredicate_And<CIsThingAlive, CIsVariableFalse>: build a quest-script "thing is alive and variable is false" predicate by copying both halves
// Leaf: ??0?$CPredicate_And@VCIsThingAlive@NScript@@VCIsVariableFalse@2@@NScript@@QAE@ABVCIsThingAlive@1@ABVCIsVariableFalse@1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E7E020, 55 bytes (+9 int3), `__fastcall`, `ret 8`, returns `this`.
// Identity: FableWin.pdb ??0?$CPredicate_And@VCIsThingAlive@NScript@@VCIsVariableFalse@2@@NScript@@QAE@ABVCIsThingAlive@1@
//   ABVCIsVariableFalse@1@@Z. The class's vtable 0x012EA430 (no RTTI) holds operator() 0x00E7DE30 (Predicate1() &&
//   Predicate2() through the members' vtables), Clone 0x00E7E060 (`new` 0x1c + the inline copy) and the scalar
//   deleting destructor 0x00E7DFE0; it sits after the quest strings "MK_INTRO_WASP1..3" / "CutsceneWasp", next to
//   CIsVariableFalse's vtable 0x012EA43C (operator() `!*Variable`, Clone, destructor). 4 callers (0x00E7C923, ...),
//   each building the two halves on the stack first. The manifest's "deque<pair<CCharString, long>>::begin" is
//   propagated.
// Layout (Ego_r.pdb): CPredicate_And<P1, P2> {vptr; P1 Predicate1 +4; P2 Predicate2 +4+sizeof(P1)}; CIsThingAlive
//   {vptr 0x012C32F0; CScriptThing Thing +4} (16 bytes); CIsVariableFalse {vptr; bool* Variable +4} (8 bytes);
//   CPredicateBase is the 4-byte polymorphic base (vtable 0x012C2FF4).
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90 (out of line: vptr, +4, the add-ref'd
//   counted pointer at +8).
// Shape: both member copies are the implicit (inline) copy constructors: each stores its vptr; CIsThingAlive's
//   calls the CScriptThing copy, CIsVariableFalse's copies the pointer.
class CScriptThing {
public:
    long  m_id;                                          // +0x04
    void* m_pCounted;                                    // +0x08 (counted pointer info, add-ref'd on copy)
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    virtual ~CScriptThing();
};

namespace NScript {

class CPredicateBase {                                   // vtable 0x012C2FF4
public:
    virtual bool operator()() const = 0;
    virtual CPredicateBase* Clone() const = 0;
    virtual ~CPredicateBase() { }
};

class CIsThingAlive : public CPredicateBase {            // vtable 0x012C32F0
public:
    CScriptThing Thing;                                  // +0x04
    CIsThingAlive(const CScriptThing& thing);
    virtual ~CIsThingAlive();
    virtual bool operator()() const;
    virtual CPredicateBase* Clone() const;
};

class CIsVariableFalse : public CPredicateBase {         // vtable 0x012EA43C
public:
    const bool* Variable;                                // +0x04
    CIsVariableFalse(const bool& variable) : Variable(&variable) { }
    virtual ~CIsVariableFalse() { }
    virtual bool operator()() const { return !*Variable; }
    virtual CPredicateBase* Clone() const { return new CIsVariableFalse(*this); }
};

template <class P1, class P2> class CPredicate_And : public CPredicateBase {
public:
    P1 Predicate1;                                       // +0x04
    P2 Predicate2;                                       // +0x14
    CPredicate_And(const P1& predicate1, const P2& predicate2) : Predicate1(predicate1), Predicate2(predicate2) { }
    virtual ~CPredicate_And() { }
    virtual bool operator()() const { return Predicate1() && Predicate2(); }
    virtual CPredicateBase* Clone() const { return new CPredicate_And(*this); }
};

}

template NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsVariableFalse>::CPredicate_And(
    const NScript::CIsThingAlive&, const NScript::CIsVariableFalse&);