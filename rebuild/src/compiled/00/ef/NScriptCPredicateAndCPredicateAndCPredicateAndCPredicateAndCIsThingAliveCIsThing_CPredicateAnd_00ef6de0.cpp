// NScript::CPredicate_And<CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive,CIsThingAlive>,CIsThingAlive>,CIsThingAlive>,CIsThingAlive>::~CPredicate_And() -- destroy the five-term "all things alive" script predicate: own vtable, the fifth term, then the nested four-, three- and two-term Ands inline, then the CPredicateBase vtable on all four subobjects
// Leaf: ??1?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF6DE0, 88 bytes (+8 int3), __fastcall, `ret`.
// Identity: FableWin.pdb ??1?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@
//   V12@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@UAE@XZ. Vtables:
//   0x012F678C = this instantiation (the landed copy ctor 0x00EF7060 stores it), 0x012F6780 / 0x012F6628 / 0x012E1B70 =
//   the nested four-, three- and two-term Ands, 0x012C2FF4 = NScript::CPredicateBase. Label correction: the catalog's
//   four-term name is propagated -- four derived vtable stores and five leaf calls make this the five-term And.
// Callees: NScript::CIsThingAlive::~CIsThingAlive @ 0x00E241B0 (out of line, x5).
// Layout (Ego_r.pdb): vptrs at +0x00/+0x04/+0x08/+0x0C (outer to inner); the five leaves at +0x10, +0x20 (innermost
//   pair), +0x30, +0x40 and +0x50 (outer); 0x60 bytes.
//   CIsThingAlive : CPredicateBase { CScriptThing Thing +0x04 } (16 bytes); CScriptThing {vptr, PImp_Data, PImp_Info}.
// Shape: user-declared virtual destructors at every level, each destroying its right term then its left one; the four
//   inline CPredicateBase destructors' vptr resets merge into one `mov eax,0x012C2FF4` and four stores (+0xC .. +0).
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

template class NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<
    NScript::CIsThingAlive, NScript::CIsThingAlive>, NScript::CIsThingAlive>, NScript::CIsThingAlive>, NScript::CIsThingAlive>;