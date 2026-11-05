// CTCUnholyPower::FinishMove() -- finish the spell: start the level's fade-out wait, fade the environment back to its default theme, reset the caster's interface-0xA7 component, fade or kill the build-up and hand effects, kill the dark-faces effect and stop the looping sound
// Leaf: FinishMove
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90). Spellings unified: 0072abd0: ?FadeOrKillEffect@@YIXAAUCBaseIntelligentPointer@@@Z -> ?FadeOrKillEffect@@YIXAAVCBaseIntelligentPointer@@@Z
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00794C10, length 369 bytes (plain `ret`, no frame pointer; Ego_r.pdb
//   `?FinishMove@CTCUnholyPower@@QAEXXZ`).
// Callees: as the landed sibling CTCDivineWrath::FinishMove 0x0078F6E0: __ftol2 0x00BFEA70, CTCBase::GetWorld 0x00686A70,
//   CEnvironment::TransitionToDefaultThemeAllChannels(long, float) 0x006B3950, CVectorMap LowerBound 0x0040F020
//   (interfaces 0xA7 and 0x10), CBaseIntelligentPointer::GetPItem 0x00A01B50, CTCSpecialAbilities::FadeOrKillEffect
//   (CIntelligentPointer<CThing>&) 0x0072ABD0 (__fastcall), the emitter's StopSample(unsigned long) 0x0066ADD0; plus
//   CThing::Kill(bool) 0x004C9B80 (as landed) on the dark-faces effect.
// Globals: CMainGameComponent::GetConstantFPS() 0x01375550.
// Layout: Ego_r.pdb CTCUnholyPower with the retail containers (as the sibling: after Timer 4 lower, after the map/vector
//   0xc lower): PDef +0x0c, SpellLevel +0x50, Finished +0x64, NumFramesToWaitForFadeOut +0x68, BuildUpEffect +0x9c,
//   DarkFacesEffect +0xa4, HandEffect1 +0xb4, HandEffect2 +0xbc, LoopingSound +0xc4; the def's per-level fade-out seconds
//   (vector<float> first pointer) +0xc4 as the sibling's def (descriptive); CWorld environment +0x18.
// Shape: the sibling's body (bl = 1 from `Finished = true` reused as the dead-bit mask and the Kill argument; the named
//   `times` pointer and world locals; HasTC + GetTC for 0xA7); the dark-faces effect is killed through its item
//   (`DarkFacesEffect.GetPItem()->Kill(true)`, the argument pushed before the third GetPItem); the sound stops through
//   an inline StopSound(sound) member (Ego_r.pdb `?StopSound@CTCUnholyPower@@AAEXK@Z`) whose argument loads into edi
//   before the lookup.
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}
void __fastcall FadeOrKillEffect(CBaseIntelligentPointer& effect);   // 0x0072ABD0
class CEnvironment { public:
    void TransitionToDefaultThemeAllChannels(long channels, float time);   // 0x006B3950
};
class CWorld { public:
    unsigned char _pad00[0x18];
    CEnvironment* PEnvironment;                 // +0x18
};
struct CTCSoundEmitter {                        // interface 0x10
    void StopSample(unsigned long sampleID);    // 0x0066ADD0
};
struct CTCInterfaceA7 {                         // interface 0xA7 component
    unsigned char _pad00[0x0c];
    bool m_active;                              // +0x0c
    long m_count;                               // +0x10
};
class CSpecialAbilitiesUnholyPowerDef { public:
    unsigned char _pad00[0xc4];
    float* FadeOutTimes;                        // +0xc4 vector<float> first pointer (per spell level; descriptive)
};
class CTCBase
{
public:
    virtual void v00();
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCUnholyPower : public CTCBase { public:
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    const CSpecialAbilitiesUnholyPowerDef* PDef;    // +0x0c CDefPointer
    unsigned char _pad10[0x50 - 0x10];          // PItemRemoved .. Timer
    long  SpellLevel;                           // +0x50
    long  NumFramesBetweenDamageHit;            // +0x54
    long  DurationOfSpellFrames;                // +0x58
    long  NumFramesForEnvChange;                // +0x5c
    long  CurFramesForEnvChange;                // +0x60
    bool  Finished;                             // +0x64
    long  NumFramesToWaitForFadeOut;            // +0x68
    unsigned char _pad6c[0x9c - 0x6c];          // CutSceneCancel .. StaminaModify
    CBaseIntelligentPointer BuildUpEffect;      // +0x9c
    CBaseIntelligentPointer DarkFacesEffect;    // +0xa4
    unsigned long HeroDefaultChannel;           // +0xac
    unsigned long SpellExclusionChannel;        // +0xb0
    CBaseIntelligentPointer HandEffect1;        // +0xb4
    CBaseIntelligentPointer HandEffect2;        // +0xbc
    unsigned long LoopingSound;                 // +0xc4
    void StopSound(unsigned long sound)
    {
        CTCSoundEmitter* emitter;
        if (ParentThing->GetTC(TCI_SOUND_PLAYER, &emitter))
            emitter->StopSample(sound);
    }
    void FinishMove();
};

void CTCUnholyPower::FinishMove()
{
    Finished = true;
    NumFramesToWaitForFadeOut = (long)(CMainGameComponent::GetConstantFPS() * PDef->FadeOutTimes[SpellLevel]);
    const float* times = PDef->FadeOutTimes;
    CWorld* world = (&GetWorld());
    world->PEnvironment->TransitionToDefaultThemeAllChannels(2, times[SpellLevel]);
    if (ParentThing->HasTC(TCI_SPECIAL_ABILITIES)) {
        CTCInterfaceA7* tc = ((CTCInterfaceA7*)&ParentThing->GetTC(TCI_SPECIAL_ABILITIES));
        tc->m_active = false;
        tc->m_count = 0;
    }
    if (IS_THING_ALIVE(BuildUpEffect))
        FadeOrKillEffect(BuildUpEffect);
    if (IS_THING_ALIVE(HandEffect1))
        FadeOrKillEffect(HandEffect1);
    if (IS_THING_ALIVE(HandEffect2))
        FadeOrKillEffect(HandEffect2);
    if (IS_THING_ALIVE(DarkFacesEffect))
        ((CThing*)DarkFacesEffect.GetPItem())->Kill(true);
    StopSound(LoopingSound);
}