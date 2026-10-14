// NScript::CPredicate_And<CIsThingAlive,CIsThingAlive>::CPredicate_And(const CPredicate_And&) --
// copy constructor of the two-term script predicate: own vtable, then each CIsThingAlive term
// copied inline (its vtable store + the out-of-line CScriptThing copy)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EF5C00, length 59 bytes (+ int3 padding).
// Callee: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90 (x2).
// Vtables (script module, no RTTI): 0x012E1B70 = this instantiation, 0x012C32F0 = CIsThingAlive
// (the same vtable the landed CVtable_012ef7c8::Main stores for its CIsThingAlive temporary).
// The class names come from the manifest's operator&& label @ 0x00C1C810
// (`operator&&<NScript::CPredicate_And<...NScript::CIsThingAlive...>>`); the manifest label
// "CConsoleCommandParameters::CConsoleCommandParameters" for this body is propagated and rejected.
// Layout: vptr, CIsThingAlive m_left +0x04 (vptr + CScriptThing +0x08), m_right +0x14 (0x24 bytes).
// The deeper instantiations @ 0x00EF6F40 / 0x00EF6FE0 / 0x00EF7060 nest this one. This level's copy
// is defined out of line (an explicit specialisation) so the translation unit emits it.
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
    CPredicate_And(const CPredicate_And& other);
};

template<>
CPredicate_And<CIsThingAlive, CIsThingAlive>::CPredicate_And(const CPredicate_And<CIsThingAlive, CIsThingAlive>& other)
    : m_left(other.m_left), m_right(other.m_right)
{
}

}