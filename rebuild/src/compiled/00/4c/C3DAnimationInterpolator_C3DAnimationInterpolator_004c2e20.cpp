// C3DAnimationInterpolator::C3DAnimationInterpolator() -- CBaseClass base (@ 0x0099A2D0), own vtable (0x01239840), then the two C3DAnimationBlendState members (+4, +0xc) each built inline: CBaseClass, blend-state vtable (0x01239838), null data pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C2E20, length 66 bytes.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 (stores vtable 0x01231710; rtti-vtables.tsv).
// PDB layout: C3DAnimationInterpolator = {vptr, StartState +4, EndState +0xc} (0x14 bytes);
// C3DAnimationBlendState = {vptr, CIVCountedPointer<CSharedData> Data +4} (8 bytes).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

class CIVCountedPointeeBase;

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer() : Object(0) { }
};

class C3DAnimationBlendState : public CBaseClass { public:   // vtable 0x01239838
    CIVCountedPointer Data;                // +4
    C3DAnimationBlendState() { }
};

class C3DAnimationInterpolator : public CBaseClass { public:   // vtable 0x01239840
    C3DAnimationBlendState StartState;     // +4
    C3DAnimationBlendState EndState;       // +0xc
    C3DAnimationInterpolator();
};

C3DAnimationInterpolator::C3DAnimationInterpolator()
{
}