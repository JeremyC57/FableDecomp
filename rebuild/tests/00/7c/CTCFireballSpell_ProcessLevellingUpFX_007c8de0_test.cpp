#ifndef _MSC_VER
#define __forceinline inline
#define __cdecl
#define __fastcall
#define __stdcall
#define __declspec(x)
#endif
// CTCFireballSpell::ProcessLevellingUpFX() -- on a will level-up: spawn the def's POWERUP_EFFECT at the caster and attach it to the
// fireball helper, play the level's power-up sound (levels 1..3) and make a simple noise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C8DE0, 485 bytes (+ int3 padding), `ret`.
// Leaf: ProcessLevellingUpFX
// Callees: GetSubDef<CSpecialEffectsDef> 0x00669580, CSpecialEffectsDef::GetEffectFromString 0x00669D30 (catalog "CCRC::CalcAndLookup"),
// CThing::GetPosition 0x004C73D0, CreateParticleEffect 0x0072AB70 (__fastcall), LowerBound 0x0040F020 (0x4b emitter, 0x10 sound
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
struct CInterfaceMap {
    struct SEntry { long type; CTCBase_Retail* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];            // +0x20 UsingComponents
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x91 - 0x4c];
    unsigned char  m_flags91;                   // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline unsigned long HasTC(long kind) const { return m_presentBits[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCBase_Retail* FindTC(long kind)
    {
        long type = kind;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || kind < it->type) ? map->last : it)->component;
    }
};
class CThingCreatureBase : public CThing { public:
    void MakeNoise(const CCharString& noise);   // 0x00662110
};
CThing* __fastcall CreateParticleEffect(long effect, const C3DVector& position);   // 0x0072AB70
class CTCDParticleEmitter { public:
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
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
        CThing* effect = CreateParticleEffect(effectType, parent->GetPosition());
        if (effect && !(effect->m_flags91 & 1)) {
            CTCDParticleEmitter* emitter;
            if (effect->HasTC(0x4b))
                emitter = (CTCDParticleEmitter*)effect->FindTC(0x4b);
            emitter->AttachToThing(parent, 10, FireballAttachmentName, 0, 0.0f);
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

#include <cstdio>
#include <cstring>
#include <cstdlib>
static const char* g_names[16]; static int g_nn = 0;
CCharString::CCharString(const char* text, long) : m_pData((void*)text) { }
CCharString::~CCharString() { }
static int g_destroyed = 0;
void CDefVirtual::v00() { }
void CDefVirtual::Destroy() { g_destroyed++; }
static CSpecialEffectsDef g_fx; static int g_haveFx = 1; static long g_type = 42;
void* Find_CDefClassBase_GetSubDef(const void*) { if (!g_haveFx) return 0; g_fx.m_refCount++; return &g_fx; }
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void*, const CCharString& name) { g_names[g_nn++] = (const char*)name.m_pData; return std::strcmp((const char*)name.m_pData, "POWERUP_EFFECT") == 0 ? g_type : -1; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static C3DVector g_pos;
const C3DVector& CThing::GetPosition() const { return g_pos; }
static const char* g_noise = 0;
void CThingCreatureBase::MakeNoise(const CCharString& noise) { g_noise = (const char*)noise.m_pData; }
static CThing g_effect; static long g_createdType = -1; static int g_effectNull = 0;
CThing* __fastcall CreateParticleEffect(long effect, const C3DVector& position) { g_createdType = effect; if (&position != &g_pos) g_createdType = -99; return g_effectNull ? 0 : &g_effect; }
static CThing* g_attachThing = 0; static long g_attachType = -1; static const CCharString* g_attachName = 0; static CTCDParticleEmitter* g_attachEmitter = 0;
void CTCDParticleEmitter::AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long, float) { g_attachEmitter = this; g_attachThing = thing; g_attachType = attachType; g_attachName = &helperName; }
static const char* g_sound = 0;
unsigned long CTCSoundPlayer::PlaySound(const CCharString& name, long a, long, long, long d) { g_sound = (a == 1 && d == -1) ? (const char*)name.m_pData : "bad"; return 7; }
void CTCFireballSpell::v00() { }

int main()
{
    static CThingCreatureBase parent; static CTCSoundPlayer player; static CTCDParticleEmitter emitter; static CDefClassBase spellDef;
    CInterfaceMap::SEntry pe[1]; pe[0].type = 0x10; pe[0].component = (CTCBase_Retail*)&player;
    parent.m_interfaces.first = pe; parent.m_interfaces.last = pe + 1; parent.m_presentBits[0] = 1UL << 16;
    CInterfaceMap::SEntry ee[1]; ee[0].type = 0x4b; ee[0].component = (CTCBase_Retail*)&emitter;
    g_effect.m_interfaces.first = ee; g_effect.m_interfaces.last = ee + 1; g_effect.m_presentBits[2] = 1UL << 11;
    CTCFireballSpell* f = (CTCFireballSpell*)std::calloc(1, sizeof(CTCFireballSpell));
    f->ParentThing = &parent; f->PDef = &spellDef;
    int ok = 1;
    g_fx.m_refCount = 1; f->Timer.WillAccumulatedLevel = 2;
    f->ProcessLevellingUpFX();
    if (g_createdType != 42 || g_attachEmitter != &emitter || g_attachThing != &parent || g_attachType != 10 || g_attachName != &f->FireballAttachmentName) ok = 0;
    if (!g_sound || std::strcmp(g_sound, "HERO_ABILITY_FIREBALL_POWERUP_2") || !g_noise || std::strcmp(g_noise, "SIMPLE_NOISE")) ok = 0;
    if (g_fx.m_refCount != 1 || g_destroyed != 0) ok = 0;
    // dead effect: no attach; level 4: no sound; def released to zero
    g_attachEmitter = 0; g_sound = 0; g_noise = 0; g_effect.m_flags91 = 1; f->Timer.WillAccumulatedLevel = 4; g_fx.m_refCount = 0;
    f->ProcessLevellingUpFX();
    if (g_attachEmitter || g_sound || !g_noise || g_destroyed != 1) ok = 0;
    // no effects def: nothing created, level 1 sound still plays
    g_createdType = -1; g_haveFx = 0; f->Timer.WillAccumulatedLevel = 1;
    f->ProcessLevellingUpFX();
    if (g_createdType != -1 || !g_sound || std::strcmp(g_sound, "HERO_ABILITY_FIREBALL_POWERUP_1")) ok = 0;
    if (!ok) { std::printf("007c8de0_TEST FAIL\n"); return 1; }
    std::printf("007c8de0_TEST PASS\n");
    return 0;
}