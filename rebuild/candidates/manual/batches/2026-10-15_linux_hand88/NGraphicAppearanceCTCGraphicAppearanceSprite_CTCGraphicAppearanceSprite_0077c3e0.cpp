// NGraphicAppearance::CTCGraphicAppearanceSprite::CTCGraphicAppearanceSprite(CThing* owner) -- thing-component and appearance-component bases, rotation/animation state cleared with unit anim speed, then registered with the owner's graphic appearance
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077C3E0, length 96 bytes. Callees: CTCBase::CTCBase(CThing*) @ 0x00686800
// (manifest label "CEngineSubPrimitive2DClockSprite", propagated), CTCGraphicAppearance::RegisterComponent @ 0x004BD390
// (manifest "DoubleLinkedListInsertNode").
// Vtables: CAGraphicAppearanceComponent 0x0125A170, CTCGraphicAppearanceSprite 0x0126AE94 (primary) and 0x0126AE78
// (in the component base). Only caller: the factory @ 0x004D4488 (operator new(0x30)).
// Layout from Ego_r.pdb (48 bytes): CTCBase +0x00..+0x0b, CAGraphicAppearanceComponent (vptr only) +0x0c,
// RotationAmountPerFrame +0x10, RotationAngle +0x14, OldRotationAngle +0x18, AnimStage +0x1c, LastAnimStage +0x20,
// AnimSpeed +0x24 (floats), FadeOutWithAnim (bool) +0x28, PrimHandle (CPrimitive*) +0x2c.
// Owner side (retail CThing): PTCGraphicAppearance +0x64.
// Shape: plain initialisers; VC7.1 zeroes the float members through the shared integer zero register and sinks
// the 1.0f immediate store (AnimSpeed) below the derived vtable stores.
// The engine header CTCBase.h is a flattened POD (`void* __vftable`), unusable as a polymorphic base: local model.
// Leaf: ??0CTCGraphicAppearanceSprite
struct CThing;

namespace NGraphicAppearance {

struct CAGraphicAppearanceComponent;
struct CPrimitive;

}

struct CTCGraphicAppearance {
    void RegisterComponent(NGraphicAppearance::CAGraphicAppearanceComponent* component);   // 0x004BD390
};

struct CThing {
    unsigned char         _pad_0x00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64 (retail)
};

struct CTCBase {                                        // 0x0c bytes: vptr, ParentThing, ToBeRemoved, IsInGlobalUpdateTCs
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CTCBase(CThing* owner);                             // 0x00686800
};

namespace NGraphicAppearance {

struct CAGraphicAppearanceComponent {                   // vtable 0x0125A170
    virtual void v00();
    CAGraphicAppearanceComponent() { }
};

struct CTCGraphicAppearanceSprite : CTCBase, CAGraphicAppearanceComponent {
    float       RotationAmountPerFrame;                 // +0x10
    float       RotationAngle;                          // +0x14
    float       OldRotationAngle;                       // +0x18
    float       AnimStage;                              // +0x1c
    float       LastAnimStage;                          // +0x20
    float       AnimSpeed;                              // +0x24
    bool        FadeOutWithAnim;                        // +0x28
    CPrimitive* PrimHandle;                             // +0x2c
    virtual void v00();
    CTCGraphicAppearanceSprite(CThing* owner);
};

CTCGraphicAppearanceSprite::CTCGraphicAppearanceSprite(CThing* owner)
    : CTCBase(owner), CAGraphicAppearanceComponent(),
      RotationAmountPerFrame(0.0f), RotationAngle(0.0f), OldRotationAngle(0.0f), AnimStage(0.0f), LastAnimStage(0.0f),
      AnimSpeed(1.0f), FadeOutWithAnim(false), PrimHandle(0)
{
    ParentThing->PTCGraphicAppearance->RegisterComponent(this);
}

}
