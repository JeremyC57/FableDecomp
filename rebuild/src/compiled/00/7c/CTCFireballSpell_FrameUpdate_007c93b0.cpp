// CTCFireballSpell::FrameUpdate() -- per frame: count down the release delay, tick the will-button timer, spawn the build-up effect
// once (attached to the fireball helper), run the level-up effects, drive the held state and remove the spell when finished
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C93B0, 444 bytes (+ int3 padding), `ret` (vtable 0x0126E964 slot 9).
// Leaf: ?FrameUpdate@CTCFireballSpell
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Callees: CWillButtonHelperTimer::FrameUpdate 0x008EDDD0, GetSubDef<CSpecialEffectsDef> 0x00669580, GetEffectFromString 0x00669D30,
// GetPos 0x004C73D0, CreateParticleEffect 0x0072AB70, CBaseIntelligentPointer 0x00A01B10/B90/B50/C10, LowerBound 0x0040F020,
// AttachToThing 0x006E0BE0, ProcessLevellingUpFX 0x007C8DE0, the held-state helper 0x007C9180, CThing::RemoveTC 0x004C9840.
// Shape: the Kraken/OnDie def lookup (out slot read once into a named pointer released by hand), an implicit-literal `&&` temp,
// the effect through an intelligent-pointer temporary + operator=, an uninitialised emitter local; first try.
// Names: catalog "FrameUpdate" sits on 0x007C9180, but this row is the vtable slot-9 override; 0x007C9180 is a non-virtual helper.
// NF200 (2026-11-20, q4411 / q6405): the helper 0x007C9180 respelled in place on the row-local CTCFireballSpell from the descriptive
//   UpdateButtonHeld to its PDB name ?FinishSpell@CTCFireballSpell@@QAEXXZ (Ego_r.pdb 0x007C7F80 + FableWin.pdb 0x02721680; Ego_r order
//   OnKill / OnRemove / FinishSpell / FrameUpdate = retail 0x007C9160 / .. / 0x007C9180 / 0x007C93B0), the self row's new Leaf.
// Layout: retail CTCFireballSpell = Ego_r.pdb minus 4 after the timer; timer CurrentState +0x20, WillIncrementedFlag +0x38.
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
    void RemoveTC(ETCInterfaceType interfaceType);          // 0x004C9840
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
class CTCSoundPlayer { public:                         // interface 0x10
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        T* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CWillButtonHelperTimer {                 // retail 0x34 bytes
    void FrameUpdate();                         // 0x008EDDD0
    long ButtonHeldDownTimer;                   // +0x00
    long WillAccumulatedLevel;                  // +0x04
    unsigned char PCreature[8];                 // +0x08 CIntelligentPointer<CThing>
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
    CIntelligentPointer<CThing> FireballBuildUpEffect;  // +0x48
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
    void ProcessLevellingUpFX();                // 0x007C8DE0
    void FinishSpell();                         // 0x007C9180 (Ego_r.pdb / FableWin.pdb ?FinishSpell@CTCFireballSpell@@QAEXXZ; non-virtual, called only from here)
    void FrameUpdate();
};

void CTCFireballSpell::FrameUpdate()
{
    if (SpellFinished)
        --ReleaseDelayFrameCount;
    Timer.FrameUpdate();
    CThingCreatureBase* parent = ParentThing;
    if (StartEffect) {
        StartEffect = false;
        CDefPointer<const CSpecialEffectsDef> effects;     // NF49: the shared handle; its destructor is the release the stand-in ReleaseDef did here
        long effectType = 0;
        bool found = PDef->GetSubDef(&effects) && effects->GetEffectFromString("BUILDUP_EFFECT", effectType);
        if (found) {
            FireballBuildUpEffect = CIntelligentPointer<CThing>(CTCSpecialAbilities::CreateParticleEffect(effectType, parent->GetPos()));
            if (IS_THING_ALIVE(FireballBuildUpEffect)) {
                CTCDParticleEmitter* emitter;
                CThing* effect = FireballBuildUpEffect.GetPItem();
                if (effect->HasTC(0x4b))
                    emitter = (CTCDParticleEmitter*)effect->FindTC(0x4b);
                emitter->AttachToThing(*ParentThing, 10, FireballAttachmentName, 0, 0.0f);
            }
        }
    }
    if (Timer.WillIncrementedFlag && Timer.WillAccumulatedLevel <= MaxLevelAllowableByStamina)
        ProcessLevellingUpFX();
    switch (Timer.CurrentState) {
    case 2:
        FinishSpell();
        break;
    case 3:
        if (SpellFinished && ReleaseDelayFrameCount <= 0)
            parent->RemoveTC((ETCInterfaceType)(0x9d));
        break;
    }
}