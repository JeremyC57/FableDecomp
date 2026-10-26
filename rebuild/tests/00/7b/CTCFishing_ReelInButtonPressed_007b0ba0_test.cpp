// Behaviour test for 007b0ba0: src/007b0ba0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCFishing::ReelInButtonPressed()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCFishing::ReelInButtonPressed() -- the reel button: while hooking it counts down the required presses; while reeling (once per frame) it raises the line strain (by the fish's weight capped by the rod, or the fishing def's per-press strain) and shortens the line by the rod's reel distance
// Leaf: ReelInButtonPressed
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
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                        // +0x04
    CThing* GetPItem() const;                               // 0x00A01B50
};
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
long __fastcall GFGetCurrentFrame();                        // 0x0049D870

enum EFishingStage { FISHING_STAGE_HOOKING = 3, FISHING_STAGE_REELING = 4 };

class CTCFishing {
public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CBaseIntelligentPointer PFish;                          // +0x0c
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
            CThing* fishThing = PFish.GetPItem();
            if (fishThing->HasTC(0xdc))
                fish = (CTCFish*)fishThing->GetTC(0xdc);
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
            LastFramePressed = GFGetCurrentFrame();
        }
        break;
    }
}

// ---- stubs and driver ----
static long g_frame = 100; static const CFishingRodDef* g_rod = 0; static CThing* g_fishThing = 0;
long __fastcall GFGetCurrentFrame() { return g_frame; }
void CBaseIntelligentPointer::v00() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return g_fishThing; }
void CTCFishing::v00() {}
const CFishingRodDef* CTCFishing::GetPFishingRodDef() const { return g_rod; }
int main()
{
    static CThing fishThing; static CTCFish fish; fish.Weight = 10.0f;
    CInterfaceMap::SEntry e[1] = { { 0xdc, (CTCBase*)&fish } }; fishThing.m_interfaces.first = e; fishThing.m_interfaces.last = e + 1; fishThing.m_presentBits[0xdc >> 5] = 1UL << (0xdc & 31);
    g_fishThing = &fishThing;
    static CFishingRodDef rod; rod.DistanceDecreasePerPress = 2.0f; rod.PullingDistanceDecreasePerPress = 1.0f; rod.MaxStrain = 20.0f;
    static CFishingDef def; def.StrainIncreasePerPress = 3.0f;
    static unsigned char raw[sizeof(CTCFishing)]; CTCFishing& f = *(CTCFishing*)raw; f.PDef = &def;
    f.Stage = FISHING_STAGE_HOOKING; f.HookPresses = 2; f.ReelInButtonPressed(); CHECK(f.HookPresses == 1); f.HookPresses = 0; f.ReelInButtonPressed(); CHECK(f.HookPresses == 0);
    f.Stage = FISHING_STAGE_REELING; f.PressedThisFrame = false; f.Pulling = true; f.Strain = 0; f.Distance = 50; g_rod = &rod;
    f.ReelInButtonPressed(); CHECK(f.Strain == 8.0f && f.Distance == 49.0f && f.PressedThisFrame && f.LastFramePressed == 100);   // weight capped at 0.4 * 20
    f.ReelInButtonPressed(); CHECK(f.Strain == 8.0f);   // once per frame
    f.PressedThisFrame = false; f.Pulling = false; f.ReelInButtonPressed(); CHECK(f.Strain == 11.0f && f.Distance == 47.0f);
    f.PressedThisFrame = false; g_rod = 0; f.Pulling = true; f.ReelInButtonPressed(); CHECK(f.Strain == 21.0f && f.Distance == 47.0f);   // no rod: uncapped weight, no distance
    f.Stage = 1; f.PressedThisFrame = false; f.ReelInButtonPressed(); CHECK(f.Strain == 21.0f);
    if (fails == 0) printf("007b0ba0_TEST PASS\n"); else printf("007b0ba0_TEST FAIL\n");
    return fails;
}