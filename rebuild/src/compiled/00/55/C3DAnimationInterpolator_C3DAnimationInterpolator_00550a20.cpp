// C3DAnimationInterpolator::C3DAnimationInterpolator(const C3DAnimationBlendState& from, const C3DAnimationBlendState& to) -- CBase, this class's vtable, then the two blend-state members copy-constructed inline (each: its vtable, the counted animation at +4 with the reference count at animation+4 bumped when set)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00550A20, length 63 bytes.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0. Vtables: 0x01239840 = C3DAnimationInterpolator (this class), 0x01239838 = C3DAnimationBlendState (both members, the constant
// kept in ecx). Shape: the first argument's animation is loaded before the vtable stores, each member's `test eax,eax ; mov [member+4],eax ; je ; inc [eax+4]`
// bumps the count after the store; `this` is returned. The chunk label "CEnginePrimitiveAnimation" is propagated; the classes are named through rtti-vtables.tsv.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

struct SCountedAnimation_00550a20 {               // the shared animation: the reference count at +4
    long m_a;
    long m_refCount;                               // +0x04
};

class C3DAnimationBlendState { public:                    // vtable 0x01239838: 8 bytes
    virtual void v00();
    SCountedAnimation_00550a20* m_pAnimation;      // +0x04
    C3DAnimationBlendState(const C3DAnimationBlendState& other) : m_pAnimation(other.m_pAnimation)
    {
        if (m_pAnimation)
            ++m_pAnimation->m_refCount;
    }
};

class C3DAnimationInterpolator : public CBaseClass { public:         // vtable 0x01239840
    C3DAnimationBlendState m_from;                 // +0x04
    C3DAnimationBlendState m_to;                   // +0x0c
    C3DAnimationInterpolator(const C3DAnimationBlendState& from, const C3DAnimationBlendState& to);
};

C3DAnimationInterpolator::C3DAnimationInterpolator(const C3DAnimationBlendState& from, const C3DAnimationBlendState& to)
    : CBaseClass()
    , m_from(from)
    , m_to(to)
{
}