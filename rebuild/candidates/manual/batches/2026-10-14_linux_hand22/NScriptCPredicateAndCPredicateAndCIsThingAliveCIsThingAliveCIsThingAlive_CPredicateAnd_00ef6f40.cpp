// NScript::CPredicate_And<CPredicate_And<CIsThingAlive,CIsThingAlive>,CIsThingAlive>::CPredicate_And(const CPredicate_And&)
// -- copy constructor of the three-term script predicate: own vtable, the nested two-term predicate
// copied inline (its vtable, then its two CIsThingAlive terms), then the third term
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EF6F40, length 85 bytes (+ int3 padding).
// Callee: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90 (x3).
// Vtables (script module, no RTTI): 0x012F6628 = this instantiation, 0x012E1B70 = the nested
// CPredicate_And<CIsThingAlive,CIsThingAlive> (its own copy constructor @ 0x00EF5C00 is inlined
// here), 0x012C32F0 = CIsThingAlive. The class names come from the manifest's operator&& label @
// 0x00C1C810; the manifest label "CConsoleCommandParameters::CConsoleCommandParameters" for this
// body is propagated and rejected.
// Layout: vptr, nested predicate +0x04 (vptr, CIsThingAlive +0x08 / +0x18), CIsThingAlive +0x28
// (0x38 bytes). The deeper instantiations @ 0x00EF6FE0 / 0x00EF7060 call THIS copy out of line
// (it is past the inliner's size budget), so this level is defined out of line (an explicit
// specialisation) while the inner level stays an in-class inline.
// engine/CScriptThing.h (PDB): { vptr, PImp_Data, PImp_Info }, 12 bytes; modelled here as an opaque
// 12-byte body because only its out-of-line copy constructor is used.
struct CScriptThing {
    unsigned char _body[12];
    CScriptThing(const CScriptThing& other);   // 0x004ABE90
};

namespace NScript {

struct CIsThingAlive {                         // vtable 0x012C32F0
    virtual void v00();
    CScriptThing Thing;                        // +0x04
    CIsThingAlive(const CIsThingAlive& other) : Thing(other.Thing) { }
};

template<class TLeft, class TRight>
struct CPredicate_And {
    virtual void v00();
    TLeft  m_left;                             // +0x04
    TRight m_right;                            // after the left term
    CPredicate_And(const CPredicate_And& other) : m_left(other.m_left), m_right(other.m_right) { }
};

typedef CPredicate_And<CIsThingAlive, CIsThingAlive> CPredicate_And2;

template<>
CPredicate_And<CPredicate_And2, CIsThingAlive>::CPredicate_And(const CPredicate_And<CPredicate_And2, CIsThingAlive>& other)
    : m_left(other.m_left), m_right(other.m_right)
{
}

// The explicit specialisation above is only emitted when something references it: this factory
// (a plain `new` copy; the out-of-line constructor is not inlined into it) provides the reference.
CPredicate_And<CPredicate_And2, CIsThingAlive>* CopyPredicate(const CPredicate_And<CPredicate_And2, CIsThingAlive>& other)
{
    return new CPredicate_And<CPredicate_And2, CIsThingAlive>(other);
}

}
