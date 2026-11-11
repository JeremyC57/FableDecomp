// CTCWife::OnHeroViolence() -- the hero's violence in front of his wife: before the marriage frame it only blocks courting, clears her love value and hides the attraction icon; once married it applies the def's hate spike (peak, run-out frames, persisting effect) to the parent's opinion-of-hero component as attitude transients 4 and 3
// Leaf: ?OnHeroViolence@CTCWife@@QAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CFDA0, 209 bytes (+15 int3). __fastcall, `ret`.
// Callees: CWorld::GetWorldFrame() 0x0049D870 (static); CTCWife::SetAttractionIcon(bool, float, unsigned char) 0x006CFC90;
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020; __ftol2 0x00BFEA70;
//          CTCCreatureOpinionOfHero::AdjustOpinionTransient(EOpinion, float, long, long, float, bool) 0x00682140 (manifest
//          label, module unattributed; called on interface 0x4f). Globals: CMainGameComponent::GetConstantFPS() 0x01375550.
// Shape: the unmarried arm stores CourtingBlocked (+0x15) and LoveAttitudeValue (+0x8c, PDB +0x90: the retail map at +0x78
//        is 12 bytes) between the pushes and the SetAttractionIcon(false, 1.0f, 0xff) call. The married arm uses the
//        UNCHECKED out-parameter GetInterface for 0x4f (bitset word 2 bit 15: `test ch,ch ; jns`): retail ignores the
//        result, so the absent path reads the key word as the component (`mov ebx,[esp+0Ch]`). The frame count is
//        `(long)(SecondsRunOut * (float)CMainGameComponent::GetConstantFPS())` (fild first, fmul the def float) and the def pointer is re-read
//        from the member for each call (it is a pointer member; the callee may replace it).
// Layout: CTCWife ParentThing +0x04, PHeroMarriageDef +0x0c, CourtingBlocked +0x15, FrameGotMarriedToThePlayer +0x20,
//         LoveAttitudeValue +0x8c; CHeroMarriageDef SecondsRunOutWifeOnViolenceHateSpike +0x90, PeakEffect.. +0x94,
//         PersistEffect.. +0x98 (PDB 144/148/152); retail CThing bitset +0x20, interface map +0x44.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CWorld { public:
    static long __fastcall GetFrame();                // 0x0049D870
};
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
class CHeroMarriageDef { public:
    unsigned char _pad00[0x90];
    float SecondsRunOutWifeOnViolenceHateSpike; // +0x90
    float PeakEffectWifeOnViolenceHateSpike;    // +0x94
    float PersistEffectWifeOnViolenceHateSpike; // +0x98
};
enum EOpinion;                                           // MS forward enum (Ego_r.pdb EOpinion; the row passes 4 and 3)
class CTCCreatureOpinionOfHero { public:
    void AdjustOpinionTransient(EOpinion opinion, float peak, long unused, long runOutFrames, float persist, bool ignorePersonality);   // 0x00682140 ?AdjustOpinionTransient@CTCCreatureOpinionOfHero@@QAEXW4EOpinion@@MJJM_N@Z (6th parameter bool: retail tests it as a byte)
};
class CTCWife { public:
    virtual void v00();
    CThing*          ParentThing;                // +0x04
    unsigned char    _pad_0x08[4];
    CHeroMarriageDef* PHeroMarriageDef;          // +0x0c CDefPointer<CHeroMarriageDef const>::Object
    unsigned char    _pad_0x10[5];
    bool             CourtingBlocked;            // +0x15
    unsigned char    _pad_0x16[0x20 - 0x16];
    long             FrameGotMarriedToThePlayer; // +0x20
    unsigned char    _pad_0x24[0x8c - 0x24];
    float            LoveAttitudeValue;          // +0x8c
    void OnHeroViolence();
private:
    void SetAttractionIcon(bool show, float scale, unsigned char alpha);   // 0x006CFC90 ?SetAttractionIcon@CTCWife@@AAEX_NME@Z (private)
};
void CTCWife::OnHeroViolence()
{
    if (FrameGotMarriedToThePlayer > CWorld::GetFrame()) {
        CourtingBlocked = true;
        LoveAttitudeValue = 0.0f;
        SetAttractionIcon(false, 1.0f, 0xff);
        return;
    }
    CTCCreatureOpinionOfHero* opinion;
    ParentThing->GetTC(TCI_CREATURE_OPINION_OF_HERO, &opinion);
    opinion->AdjustOpinionTransient((EOpinion)4, PHeroMarriageDef->PeakEffectWifeOnViolenceHateSpike, 0,
        (long)(PHeroMarriageDef->SecondsRunOutWifeOnViolenceHateSpike * (float)CMainGameComponent::GetConstantFPS()),
        PHeroMarriageDef->PersistEffectWifeOnViolenceHateSpike, 1);
    opinion->AdjustOpinionTransient((EOpinion)3, PHeroMarriageDef->PeakEffectWifeOnViolenceHateSpike, 0,
        (long)(PHeroMarriageDef->SecondsRunOutWifeOnViolenceHateSpike * (float)CMainGameComponent::GetConstantFPS()),
        PHeroMarriageDef->PersistEffectWifeOnViolenceHateSpike, 1);
}