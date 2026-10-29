// CTCFireballSpell::ProcessLevellingUpFX() -- on a will level-up: spawn the def's POWERUP_EFFECT at the caster and attach it to the
// fireball helper, play the level's power-up sound (levels 1..3) and make a simple noise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C8DE0, 485 bytes (+ int3 padding), `ret`.
// Leaf: ProcessLevellingUpFX
// Callees: GetSubDef<CSpecialEffectsDef> 0x00669580, CSpecialEffectsDef::GetEffectFromString 0x00669D30 (catalog "CCRC::CalcAndLookup"),
// CThing::GetPos 0x004C73D0, CreateParticleEffect 0x0072AB70 (__fastcall), LowerBound 0x0040F020 (0x4b emitter, 0x10 sound
// player), CTCDParticleEmitter::AttachToThing 0x006E0BE0, CTCSoundPlayer::PlaySound 0x0066BD00, MakeNoise 0x00662110, CCharString
// 0x0099EBF0 / ~ 0x0099EAE0.
// Shape: the CTCLightningOrb::OnDie (0x007F2410) `bool found = GetSubDef && GetEffectFromString("...")` with the literal converted
// implicitly (the temp is built before the other arguments and freed under the `test bl,1` flag); every string argument is an
// implicit conversion; the emitter is an UNinitialised local filled by HasTC + ternary FindTC, so the absent path reads its
// frame word (which retail zeroes at entry with the found flag's ebx); GetSubDef / GetEffectFromString are same-TU stand-ins.
// Layout: retail CTCFireballSpell = Ego_r.pdb minus 4 after the timer (12-byte vector): Timer +0x0c (WillAccumulatedLevel +0x10),
// PDef +0x40, FireballAttachmentName +0x6c. No engine header.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
};
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p && --m_p->m_refCount == 0)
            m_p->Destroy();
    }
    T* operator->() const { return m_p; }
};
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);   // stand-in callee
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)
struct CTCBase_Retail;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];            // +0x20 UsingComponents
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x91 - 0x4c];
    unsigned char  m_flags91;                   // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)((CTCBase_Retail*)it->second);
            return true;
        }
        return false;
    }
    const C3DVector& GetPos() const;       // 0x004C73D0
    __forceinline unsigned long HasTC(long kind) const { return m_presentBits[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCBase_Retail* FindTC(long kind)
    {
        ETCInterfaceType type = (ETCInterfaceType)(kind);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return ((CTCBase_Retail*)((it == map->last || kind < it->first) ? map->last : it)->second);
    }
};
class CThingCreatureBase : public CThing { public:
    void MakeNoise(const CCharString& noise);   // 0x00662110
};
CThing* __fastcall CreateParticleEffect(long effect, const C3DVector& position);   // 0x0072AB70
class CTCDParticleEmitter { public:
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
class CTCSoundPlayer { public:                         // interface 0x10
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};
struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
};
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
    void ProcessLevellingUpFX();
};

void CTCFireballSpell::ProcessLevellingUpFX()
{
    CThingCreatureBase* parent = ParentThing;
    CDefPointer<CSpecialEffectsDef> effects;
    long effectType = 0;
    bool found = PDef->GetSubDef(effects) && effects->GetEffectFromString("POWERUP_EFFECT", effectType);
    if (found) {
        CThing* effect = CreateParticleEffect(effectType, parent->GetPos());
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
            sound->PlaySound("HERO_ABILITY_FIREBALL_POWERUP_1", 1, 0, 0, -1);
            break;
        case 2:
            sound->PlaySound("HERO_ABILITY_FIREBALL_POWERUP_2", 1, 0, 0, -1);
            break;
        case 3:
            sound->PlaySound("HERO_ABILITY_FIREBALL_POWERUP_3", 1, 0, 0, -1);
            break;
        }
    }
    parent->MakeNoise("SIMPLE_NOISE");
}