#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; std::printf("check failed line %d\n", __LINE__); } } while (0)
// ---- the candidate model (copied from src/0077c740.cpp) ----
// NGraphicAppearance::CTCGraphicAppearanceSprite::FrameUpdate() -- vtable slot 9: advance the sprite's rotation and animation stage by one frame and flag the owner's appearance as changed when either moved
// Leaf: ?FrameUpdate@CTCGraphicAppearanceSprite@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077C740, 79 bytes (+1 int3 padding).
// Name: FableWin.pdb ?FrameUpdate@CTCGraphicAppearanceSprite@NGraphicAppearance@@UAEXXZ (the sprite's only void no-argument
// virtual besides OnCreate, slot 4 = the landed 0x0077C480, and OnAppearanceClear); slot 9 of the vtable 0x0126AE94.
// Callees: CThing::SetGraphicAppearanceChanged(unsigned long) @ 0x004C7590 (landed; reason 4).
// Constants: 0.0f @ 0x0122DEDC (compiler literal).
// Layout: as the landed SetRotationAngle 0x0077C490: CTCBase (ParentThing +4), the component vptr +0x0C,
// RotationAmountPerFrame +0x10, RotationAngle +0x14, OldRotationAngle +0x18, AnimStage +0x1C, LastAnimStage +0x20,
// AnimSpeed +0x24; owner PTCGraphicAppearance +0x64 (retail), whose CEngineGraphic MainGraphic sits at +0x44 (Ego_r.pdb:
// AnimStep at +4 of it, +0x48).
// Shape: the two "old" copies are integer moves interleaved with the x87 updates (LastAnimStage written FIRST in the
// source gives retail's eax/edx pairing and store order; source order Old/Rotation/Last swaps them); `x += y` loads y first
// (`fld [10h] ; fadd [14h]`, `fld step ; fmul speed ; fadd stage`); the new stage stays on the FPU for the
// `!=` test (`fucompp ; test ah,44h ; jp`), then `RotationAmountPerFrame > 0.0f`. The owner is loaded once (edx).
// Plain `ret`.
struct CEngineGraphic {
    unsigned long BankIndex;                            // +0x00
    float         AnimStep;                             // +0x04
    float         RenderSizeX;                          // +0x08
    bool          AdditiveAlpha;                        // +0x0c
    unsigned char Type;                                 // +0x0d
};

struct CTCGraphicAppearance {
    unsigned char  _pad00[0x44];
    CEngineGraphic MainGraphic;                         // +0x44
};

class CThing {
public:
    unsigned char         _pad_0x00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64 (retail)
    void SetGraphicAppearanceChanged(unsigned long flags);   // 0x004C7590
};

struct CTCBase {                                        // 0x0c bytes: vptr, ParentThing, ToBeRemoved, IsInGlobalUpdateTCs
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
};

namespace NGraphicAppearance {

struct CPrimitive;

struct CAGraphicAppearanceComponent {
    virtual void v00();
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
    virtual void FrameUpdate();
};

void CTCGraphicAppearanceSprite::FrameUpdate()
{
    LastAnimStage = AnimStage;
    OldRotationAngle = RotationAngle;
    RotationAngle += RotationAmountPerFrame;
    AnimStage += AnimSpeed * ParentThing->PTCGraphicAppearance->MainGraphic.AnimStep;
    if (AnimStage != LastAnimStage || RotationAmountPerFrame > 0.0f)
        ParentThing->SetGraphicAppearanceChanged(4);
}

} // namespace NGraphicAppearance

// ---- stubs for the external callees ----

static int g_changedCalls = 0;
static unsigned long g_changedFlags = 0;
void CThing::SetGraphicAppearanceChanged(unsigned long flags) { ++g_changedCalls; g_changedFlags = flags; }
void CTCBase::v00() {}
void NGraphicAppearance::CAGraphicAppearanceComponent::v00() {}
void NGraphicAppearance::CTCGraphicAppearanceSprite::v00() {}

static void run()
{

    CTCGraphicAppearance appearance;
    appearance.MainGraphic.AnimStep = 2.0f;
    CThing thing;
    thing.PTCGraphicAppearance = &appearance;
    NGraphicAppearance::CTCGraphicAppearanceSprite s;
    s.ParentThing = &thing;
    s.RotationAmountPerFrame = 0.5f;
    s.RotationAngle = 0.25f;
    s.OldRotationAngle = 0.0f;
    s.AnimStage = 1.0f;
    s.LastAnimStage = 0.0f;
    s.AnimSpeed = 0.5f;
    s.FrameUpdate();
    CHECK(s.LastAnimStage == 1.0f && s.OldRotationAngle == 0.25f && s.RotationAngle == 0.75f && s.AnimStage == 2.0f);
    CHECK(g_changedCalls == 1 && g_changedFlags == 4);
    // a still sprite: no animation step, no rotation -> not marked changed
    s.AnimSpeed = 0.0f;
    s.RotationAmountPerFrame = 0.0f;
    s.FrameUpdate();
    CHECK(s.LastAnimStage == 2.0f && s.AnimStage == 2.0f && s.RotationAngle == 0.75f);
    CHECK(g_changedCalls == 1);
    // rotation only (anim stage unchanged) still marks it
    s.RotationAmountPerFrame = 0.25f;
    s.FrameUpdate();
    CHECK(s.RotationAngle == 1.0f && g_changedCalls == 2);
}

int main()
{
    run();
    if (!g_ok) { std::printf("0077c740_TEST FAIL\n"); return 1; }
    std::printf("0077c740_TEST PASS\n");
    return 0;
}