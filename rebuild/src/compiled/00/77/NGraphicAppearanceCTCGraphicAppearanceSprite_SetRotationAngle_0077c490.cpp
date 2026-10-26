// NGraphicAppearance::CTCGraphicAppearanceSprite::SetRotationAngle(float amount) -- flag the owner's appearance as changed, store the per-frame rotation amount and pick a random starting angle in [0,1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077C490, length 56 bytes (64 with padding). Callees:
// CTCGraphicAppearance::SetAsChanged(unsigned long reasons) @ 0x004BF9E0 (reason 4), the shared display-engine getter
// @ 0x00686A70 (manifest "CViewBase::GetDisplayEngine": `return g_world->DisplayEngine` -- it ignores ecx),
// CWorld::GetRandomGenerator @ 0x0049D810 (manifest "CVertexBufferWin32::DoSizeof", propagated: `return &m_p->+0x1613c`)
// and the generator's FloatRandom(float max) @ 0x004978A0 (manifest "GFFloatRandom", but it is a __fastcall: ecx = the seed).
// Only caller @ 0x00703457.
// Layout (Ego_r.pdb CTCGraphicAppearanceSprite, 48 bytes): CTCBase +0x00..+0x0b (ParentThing +0x04), the component vptr +0x0c,
// RotationAmountPerFrame +0x10, RotationAngle +0x14. Owner side (retail CThing): PTCGraphicAppearance +0x64.
// Shape: the float argument is copied through ecx (a plain float-to-float member store); the random result is popped
// straight into RotationAngle (`fstp [esi+0x14]`). The generator is fetched into a local in its own statement: written as one
// chained expression VC7.1 hoists the `push 1.0f` above the two getter calls.
// The engine header CTCBase.h is a flattened POD (`void* __vftable`), unusable as a polymorphic base: local model.
class CThing;

struct CRandomGenerator {
    float FloatRandom(float max);                       // 0x004978A0
};

class CWorld { public:
    CRandomGenerator* GetRandomGenerator();             // 0x0049D810
};

struct CTCGraphicAppearance {
    void SetAsChanged(unsigned long reasons);           // 0x004BF9E0
};

class CThing { public:
    unsigned char         _pad_0x00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64 (retail)
};

class CTCBase { public:                                        // 0x0c bytes: vptr, ParentThing, ToBeRemoved, IsInGlobalUpdateTCs
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

namespace NGraphicAppearance {

class CPrimitive;

class CAGraphicAppearanceComponent { public:
    virtual void v00();
};

class CTCGraphicAppearanceSprite : public CTCBase, public CAGraphicAppearanceComponent { public:
    float       RotationAmountPerFrame;                 // +0x10
    float       RotationAngle;                          // +0x14
    float       OldRotationAngle;                       // +0x18
    float       AnimStage;                              // +0x1c
    float       LastAnimStage;                          // +0x20
    float       AnimSpeed;                              // +0x24
    bool        FadeOutWithAnim;                        // +0x28
    CPrimitive* PrimHandle;                             // +0x2c
    virtual void v00();
    void SetRotationAngle(float amount);
};

void CTCGraphicAppearanceSprite::SetRotationAngle(float amount)
{
    ParentThing->PTCGraphicAppearance->SetAsChanged(4);
    RotationAmountPerFrame = amount;
    CRandomGenerator* random = GetWorld().GetRandomGenerator();
    RotationAngle = random->FloatRandom(1.0f);
}

}