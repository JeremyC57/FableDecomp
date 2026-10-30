// NScript::CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive,CIsThingAlive>,CIsThingAlive>,CIsThingAlive>::CPredicate_And(const CPredicate_And&)
// -- copy constructor of the four-term script predicate: own vtable, the nested three-term
// predicate copied OUT OF LINE (@ 0x00EF6F40), then the fourth CIsThingAlive term inline
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EF6FE0, length 52 bytes (+ int3 padding).
// Callees: CPredicate_And<CPredicate_And<CIsThingAlive,CIsThingAlive>,CIsThingAlive>::CPredicate_And(const&)
// @ 0x00EF6F40, CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90.
// Vtables (script module, no RTTI): 0x012F6780 = this instantiation, 0x012C32F0 = CIsThingAlive.
// The class names come from the manifest's operator&& label @ 0x00C1C810; the manifest label
// "CConsoleCommandParameters::CConsoleCommandParameters" for this body is propagated and rejected.
// Layout: vptr, nested predicate +0x04 (0x38 bytes), CIsThingAlive +0x3c (0x4c bytes).
// The three-term copy is declared (not defined) here, as an out-of-line explicit specialisation,
// which reproduces the retail call; this level is defined out of line so the TU emits it.
// engine/CScriptThing.h (PDB): { vptr, PImp_Data, PImp_Info }, 12 bytes; modelled here as an opaque
// 12-byte body because only its out-of-line copy constructor is used.
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

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

template<>
CPredicate_And<CPredicate_And2, CIsThingAlive>::CPredicate_And(const CPredicate_And<CPredicate_And2, CIsThingAlive>& other);   // 0x00EF6F40

template<>
CPredicate_And<CPredicate_And3, CIsThingAlive>::CPredicate_And(const CPredicate_And<CPredicate_And3, CIsThingAlive>& other)
    : m_left(other.m_left), m_right(other.m_right)
{
}

// The explicit specialisation above is only emitted when something references it: this factory
// (a plain `new` copy) provides the reference; inlining is switched off for it (inline_depth(0))
// so the small constructor stays a call (the deeper levels are past the inliner's budget on
// their own and need no such guard).
#pragma inline_depth(0)
CPredicate_And<CPredicate_And3, CIsThingAlive>* CopyPredicate(const CPredicate_And<CPredicate_And3, CIsThingAlive>& other)
{
    return new CPredicate_And<CPredicate_And3, CIsThingAlive>(other);
}

}