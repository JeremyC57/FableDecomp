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
class CScriptThing {                                     // 12 bytes
public:
    void* PImp_Data;                                     // +0x04
    void* PImp_Info;                                     // +0x08
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

class CIsThingAlive : public CPredicateBase {            // vtable 0x012C32F0, 16 bytes
public:
    CScriptThing Thing;                                  // +0x04
    CIsThingAlive(const CScriptThing& thing);
    virtual ~CIsThingAlive();                            // 0x00E241B0
    virtual bool operator()() const;
    virtual CPredicateBase* Clone() const;
};

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