// CTCFishing::ReelInButtonPressed() -- the reel button: while hooking it counts down the required presses; while reeling (once per frame) it raises the line strain (by the fish's weight capped by the rod, or the fishing def's per-press strain) and shortens the line by the rod's reel distance
// Leaf: ReelInButtonPressed
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
//
// Retail 0x007B0BA0, 288 bytes incl. padding (plain `ret`). Ego_r.pdb `?ReelInButtonPressed@CTCFishing@@...`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the fish thing), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
// @ 0x0040F020 (inlined HasTC + GetTC for CTCFish, type 0xDC = bit 28 of the bitset word +0x38, with the zero preload),
// CTCFishing::GetPFishingRodDef() const @ 0x007B0880 (landed), GFGetCurrentFrame @ 0x0049D870 (static). Floats: 0x0122DEDC =
// 0.0f, 0x0126D63C = 0.4f.
// Layout = Ego_r.pdb CTCFishing (0x64, no containers so no retail shift): PFish +0x0c, Stage +0x14, HookPresses +0x20,
// PressedThisFrame +0x24, LastFramePressed +0x28, Distance +0x2c, Strain +0x34, Pulling +0x3c, PDef +0x60; CTCFish Weight
// +0x14; CFishingRodDef DistanceDecreasePerPress +0x2c, PullingDistanceDecreasePerPress +0x30, MaxStrain +0x34; retail
// CFishingDef StrainIncreasePerPress +0x78 (PDB +0x7c: its vector is 12 bytes in retail).
// Shape: a two-case switch on the stage (`sub eax,3 ; je ; dec eax ; jne`); the two rod distances are float locals that
// default to zero (the pulling one in the frame, the other on the FPU stack); the strain cap is a named float local; VC7.1
// tail-duplicates the frame-number store into both arms.
class CTCBase;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCFish { public:
    unsigned char _pad00[0x14];
    float Weight;                                           // +0x14
};
class CFishingRodDef { public:
    unsigned char _pad00[0x2c];
    float DistanceDecreasePerPress;                         // +0x2c
    float PullingDistanceDecreasePerPress;                  // +0x30
    float MaxStrain;                                        // +0x34
};
class CFishingDef { public:
    unsigned char _pad00[0x78];
    float StrainIncreasePerPress;                           // +0x78
};
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                        // 0x0049D870

enum EFishingStage { FISHING_STAGE_HOOKING = 3, FISHING_STAGE_REELING = 4 };

class CTCFishing {
public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CIntelligentPointer<CThing> PFish;                          // +0x0c
    long  Stage;                                            // +0x14 EFishingStage
    bool  StageInitialised;                                 // +0x18
    long  FrameStageFinishes;                               // +0x1c
    long  HookPresses;                                      // +0x20
    bool  PressedThisFrame;                                 // +0x24
    long  LastFramePressed;                                 // +0x28
    float Distance;                                         // +0x2c
    float MaxDistance;                                      // +0x30
    float Strain;                                           // +0x34
    long  FrameReelFinishes;                                // +0x38
    bool  Pulling;                                          // +0x3c
    unsigned char _pad3d[0x60 - 0x3d];
    const CFishingDef* PDef;                                // +0x60 CDefPointer<CFishingDef const>
    const CFishingRodDef* GetPFishingRodDef() const;        // 0x007B0880
    void ReelInButtonPressed();
};

void CTCFishing::ReelInButtonPressed()
{
    float maxStrain;
    switch (Stage) {
    case FISHING_STAGE_HOOKING:
        if (HookPresses > 0)
            --HookPresses;
        break;
    case FISHING_STAGE_REELING:
        if (!PressedThisFrame) {
            float pullingDistance;
            CTCFish* fish = 0;
            CThing* fishThing = PFish.Get();
            if (fishThing->HasTC(TCI_FISH))
                fish = (CTCFish*)((CTCFish*)&fishThing->GetTC(TCI_FISH));
            const CFishingRodDef* rod = GetPFishingRodDef();
            pullingDistance = 0.0f;
            if (rod)
                pullingDistance = rod->PullingDistanceDecreasePerPress;
            float distance = 0.0f;
            if (rod)
                distance = rod->DistanceDecreasePerPress;
            if (Pulling) {
                float strain = fish->Weight;
                if (rod) {
                    maxStrain = rod->MaxStrain * 0.4f;
                    if (strain > maxStrain)
                        strain = maxStrain;
                }
                PressedThisFrame = true;
                Strain += strain;
                Distance -= pullingDistance;
            } else {
                Strain += PDef->StrainIncreasePerPress;
                PressedThisFrame = true;
                Distance -= distance;
            }
            LastFramePressed = CWorld::GetFrame();
        }
        break;
    }
}