// NScript::CPredicate_And<CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive,CIsThingAlive>,CIsThingAlive>,CIsThingAlive>,CIsThingAlive>::CPredicate_And(const CPredicate_And&)
// -- copy constructor of the five-term script predicate: own vtable, the nested four-term
// predicate copied inline (its vtable, the three-term predicate copied OUT OF LINE @ 0x00EF6F40,
// its fourth CIsThingAlive term inline), then the fifth CIsThingAlive term inline
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EF7060, length 78 bytes (+ int3 padding).
// Callees: CPredicate_And<CPredicate_And<CIsThingAlive,CIsThingAlive>,CIsThingAlive>::CPredicate_And(const&)
// @ 0x00EF6F40, CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90 (x2).
// Vtables (script module, no RTTI): 0x012F678C = this instantiation, 0x012F6780 = the nested
// four-term predicate (its copy @ 0x00EF6FE0 is inlined here), 0x012C32F0 = CIsThingAlive.
// The class names come from the manifest's operator&& label @ 0x00C1C810; the manifest label
// "CConsoleCommandParameters::CConsoleCommandParameters" for this body is propagated and rejected.
// Layout: vptr, nested predicate +0x04 (0x4c bytes), CIsThingAlive +0x50 (0x60 bytes).
// Only the three-term copy is past the inliner's budget: it is declared out of line (an explicit
// specialisation) while the four-term level stays the in-class inline; this level is defined out
// of line so the TU emits it.
// engine/CScriptThing.h (PDB): { vptr, PImp_Data, PImp_Info }, 12 bytes; modelled here as an opaque
// 12-byte body because only its out-of-line copy constructor is used.
class CScriptThing { public:
    unsigned char _body[12];
    CScriptThing(const CScriptThing& other);   // 0x004ABE90
};

namespace NScript {

class CIsThingAlive { public:                         // vtable 0x012C32F0
    virtual void v00();
    CScriptThing Thing;                        // +0x04
    CIsThingAlive(const CIsThingAlive& other) : Thing(other.Thing) { }
};

template<class TLeft, class TRight>
class CPredicate_And { public:
    virtual void v00();
    TLeft  m_left;                             // +0x04
    TRight m_right;                            // after the left term
    CPredicate_And(const CPredicate_And& other) : m_left(other.m_left), m_right(other.m_right) { }
};

typedef CPredicate_And<CIsThingAlive, CIsThingAlive> CPredicate_And2;
typedef CPredicate_And<CPredicate_And2, CIsThingAlive> CPredicate_And3;
typedef CPredicate_And<CPredicate_And3, CIsThingAlive> CPredicate_And4;

template<>
CPredicate_And<CPredicate_And2, CIsThingAlive>::CPredicate_And(const CPredicate_And<CPredicate_And2, CIsThingAlive>& other);   // 0x00EF6F40

template<>
CPredicate_And<CPredicate_And4, CIsThingAlive>::CPredicate_And(const CPredicate_And<CPredicate_And4, CIsThingAlive>& other)
    : m_left(other.m_left), m_right(other.m_right)
{
}

// The explicit specialisation above is only emitted when something references it: this factory
// (a plain `new` copy; the out-of-line constructor is not inlined into it) provides the reference.
CPredicate_And<CPredicate_And4, CIsThingAlive>* CopyPredicate(const CPredicate_And<CPredicate_And4, CIsThingAlive>& other)
{
    return new CPredicate_And<CPredicate_And4, CIsThingAlive>(other);
}

}