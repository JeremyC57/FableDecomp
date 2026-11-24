// CTCFireballSpell::ProcessLevellingUpFX() -- on a will level-up: spawn the def's POWERUP_EFFECT at the caster and attach it to the
// fireball helper, play the level's power-up sound (levels 1..3) and make a simple noise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C8DE0, 485 bytes (+ int3 padding), `ret`.
// Leaf: ?ProcessLevellingUpFX@CTCFireballSpell@@AAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Callees: GetSubDef<CSpecialEffectsDef> 0x00669580, CSpecialEffectsDef::GetEffectFromString 0x00669D30 (catalog "CCRC::CalcAndLookup"),
// CThing::GetPos 0x004C73D0, CreateParticleEffect 0x0072AB70 (__fastcall), LowerBound 0x0040F020 (0x4b emitter, 0x10 sound
// player), CTCDParticleEmitter::AttachToThing 0x006E0BE0, CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00, MakeNoise 0x00662110, CCharString
// 0x0099EBF0 / ~ 0x0099EAE0.
// Shape: the CTCLightningOrb::OnDie (0x007F2410) `bool found = GetSubDef && GetEffectFromString("...")` with the literal converted
// implicitly (the temp is built before the other arguments and freed under the `test bl,1` flag); every string argument is an
// implicit conversion; the emitter is an UNinitialised local filled by HasTC + ternary FindTC, so the absent path reads its
// frame word (which retail zeroes at entry with the found flag's ebx); GetSubDef / GetEffectFromString are same-TU stand-ins.
// Layout: retail CTCFireballSpell = Ego_r.pdb minus 4 after the timer (12-byte vector): Timer +0x0c (WillAccumulatedLevel +0x10),
// PDef +0x40, FireballAttachmentName +0x6c. No engine header.
#include "fable_compiler.h"
// NF49 (round 7): the def classes come from the shared headers (were local CCharString / CDefVirtual / CDefBase / CDefPointer / CSpecialEffectsDef / CDefClassBase models with same-TU stand-in bodies over made-up Lookup_CSpecialEffectsDef_GetEffectFromString / Find_CDefClassBase_GetSubDef callees): the call at
// 0x00669580 is the member template ??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z
// (Ego_r.pdb / FableWin.pdb), its body visible and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10
// idiom), and CSpecialEffectsDef::GetEffectFromString 0x00669D30 is the header's same-TU noinline body
// (FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY, the 0x007941D0 idiom), so the caller's handles stay uncaptured as with the stand-ins.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/C3DVector.h"
struct CTCBase_Retail;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];            // +0x20 UsingComponents
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad54[0x3d];
    unsigned char  m_flags91;                   // +0x91 bit 0: dead
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)((CTCBase_Retail*)it->second);
            return true;
        }
        return false;
    }
    const C3DVector& GetPos() const;       // 0x004C73D0
    FABLE_FORCEINLINE unsigned long HasTC(long kind) const { return m_presentBits[kind >> 5] & (1UL << (kind & 31)); }
    FABLE_FORCEINLINE CTCBase_Retail* FindTC(long kind)
    {
        ETCInterfaceType type = (ETCInterfaceType)(kind);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        return ((CTCBase_Retail*)((it == map->end() || kind < it->first) ? map->end() : it)->second);
    }
};
class CThingCreatureBase : public CThing { public:
    void MakeNoise(const CCharString& noise);   // 0x00662110
};
// NF49 (round 7): CreateParticleEffect 0x0072AB70 is the public static __fastcall
// ?CreateParticleEffect@CTCSpecialAbilities@@SIPAVCThing@@JABVC3DVector@@@Z (Ego_r.pdb / FableWin.pdb), declared by
// engine/CTCSpecialAbilities.h; was a free stand-in.
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#include "engine/CTCSpecialAbilities.h"
class CTCDParticleEmitter { public:
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
class CTCSoundPlayer {
public:
    unsigned long PlayAttachedCriteria(CCharString const&a0, bool a1, bool a2, bool a3, long a4);   // 0x0066BD00 (Ego_r.pdb ?PlayAttachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@_N11J@Z)
private:    // 0x0066BD00
};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CWillButtonHelperTimer {                 // retail 0x34 bytes
    long ButtonHeldDownTimer;                   // +0x00
    long WillAccumulatedLevel;                  // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08
    long MyAbility;                             // +0x10
    long CurrentState;                          // +0x14
    void* WillPowerUpAccumulatedTimings[3];     // +0x18
    float MaxNumberLevels;                      // +0x24
    float CurMaxLevels;                         // +0x28
    bool WillIncrementedFlag;                   // +0x2c
    bool UpdateAccLevel;
    bool IsAICasting;
    bool IsAIHoldingDown;
    long LevelToCast;                           // +0x30
};

class CTCFireballSpell {
public:
    virtual void v00();
    CThingCreatureBase* ParentThing;            // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CWillButtonHelperTimer Timer;               // +0x0c
    CDefClassBase* PDef;                        // +0x40 CDefPointer<CSpecialAbilitiesFireballSpellDef const>
    unsigned long InitLoopingSound;             // +0x44
    CBaseIntelligentPointer FireballBuildUpEffect;   // +0x48
    bool ImmediateCastMode;                     // +0x50
    bool OneAnimCastMode;                       // +0x51
    unsigned char PTarget[8];                   // +0x52
    unsigned char PItemRemoved[8];              // +0x5a
    long WeaponState;                           // +0x64
    bool LeftHandAnim;                          // +0x68
    CCharString FireballAttachmentName;         // +0x6c
    bool StartEffect;                           // +0x70
    long MaxLevelAllowableByStamina;            // +0x74
    long ReleaseDelayFrameCount;                // +0x78
    bool SpellFinished;                         // +0x7c
    private:
    void ProcessLevellingUpFX();
    public:
};

void CTCFireballSpell::ProcessLevellingUpFX()
{
    CThingCreatureBase* parent = ParentThing;
    CDefPointer<const CSpecialEffectsDef> effects;
    long effectType = 0;
    bool found = PDef->GetSubDef(&effects) && effects->GetEffectFromString("POWERUP_EFFECT", effectType);
    if (found) {
        CThing* effect = CTCSpecialAbilities::CreateParticleEffect(effectType, parent->GetPos());
        if (effect && !(effect->m_flags91 & 1)) {
            CTCDParticleEmitter* emitter;
            if (effect->HasTC(0x4b))
                emitter = (CTCDParticleEmitter*)effect->FindTC(0x4b);
            emitter->AttachToThing(*parent, 10, FireballAttachmentName, 0, 0.0f);
        }
    }
    CTCSoundPlayer* sound;
    if (ParentThing->GetInterface(&sound, 0x10)) {
        switch (Timer.WillAccumulatedLevel) {
        case 1:
            ((CTCSoundPlayer*)sound)->PlayAttachedCriteria("HERO_ABILITY_FIREBALL_POWERUP_1", 1, 0, 0, -1);
            break;
        case 2:
            ((CTCSoundPlayer*)sound)->PlayAttachedCriteria("HERO_ABILITY_FIREBALL_POWERUP_2", 1, 0, 0, -1);
            break;
        case 3:
            ((CTCSoundPlayer*)sound)->PlayAttachedCriteria("HERO_ABILITY_FIREBALL_POWERUP_3", 1, 0, 0, -1);
            break;
        }
    }
    parent->MakeNoise("SIMPLE_NOISE");
}