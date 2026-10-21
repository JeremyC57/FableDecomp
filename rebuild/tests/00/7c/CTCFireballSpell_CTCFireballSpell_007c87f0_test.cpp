#ifndef _MSC_VER
#define __forceinline inline
#define __cdecl
#define __fastcall
#define __stdcall
#endif
#include <new>
// CTCFireballSpell::CTCFireballSpell(CThing& parent) -- constructor: CTCBase, the will-button timer for the fireball ability,
// cleared def/sound/effect/target state; take the special-ability lock, load SPECIAL_ABILITIES_FIREBALL_SPELL_DEF, start the
// looping build-up sound, cap the castable level by the hero's stamina thresholds and convert the release delay to frames
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C87F0, 510 bytes, `ret 4`; vtable 0x0126E964. No EH frame (TU without /EH).
// Leaf: ??0CTCFireballSpell
// Callees: CTCBase(CThing*) 0x00686800 (catalog "ClockSprite"), CWillButtonHelperTimer(EHeroAbility, CThing*) 0x008EDFF0,
// CBaseIntelligentPointer 0x00A01B10 / SetPItem 0x00A01B90 (CIntelligentPointer<CThing> vtable 0x01238C6C), CCharString
// 0x0099EBF0 / ~ 0x0099EAE0 ("" 0x0122D70E), CGameDefinitionManager::Get 0x0044C6B0, GetGlobalIndexFromClassIndex 0x009AD390,
// GetDef<CSpecialAbilitiesFireballSpellDef> 0x005F8A57, LowerBound 0x0040F020 (0xA7 special abilities: `test cl,cl ; jns`,
// 0x10 sound player, 4 hero stats), CTCSoundPlayer::PlaySound 0x0066BD00, __ftol2 0x00BFEA70; g_FramesPerSecond 0x01375550.
// Shape: the CTCForcePushPower ctor (0x007A54A0) init list sharing the ebx zero; implicit-literal string temps; the stamina
// level loop converts `(float)stamina` INSIDE the loop condition from a named long -- VC hoists it as loop-invariant through a
// stack slot (`mov ecx,[eax+58h] ; mov [esp+14h],ecx ; fild ; fstp` to the same word) and aligns the loop head with
// `mov edi,edi`; converting once before the loop gives a direct `fild [eax+58h]` (10 bytes short).
// Layout: Ego_r.pdb CTCFireballSpell minus 4 after the 0x34-byte retail timer; PTarget at the packed +0x52 (pack(2) + explicit
// pads). Def +0x3c / +0x44 and CTCHeroStats +0x58 names inferred. No engine header.
class CThing;
class CBaseObject;
class CTCBase;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CSpecialAbilitiesFireballSpellDef {
public:
    virtual void v00();
    long RefCount;                                  // +0x04
    unsigned char _pad08[0x3c - 0x08];
    float ReleaseDelaySeconds;                      // +0x3c (name inferred)
    unsigned char _pad40[0x44 - 0x40];
    float* StaminaLevelThresholds;                  // +0x44 vector<float> start (4 levels)
};

template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    T* operator->() const { return Object; }
};

class CDefinitionManager {
public:
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const;   // 0x009AD390
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;            // 0x005F8A57 for the fireball def
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();                                     // 0x0044C6B0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20
    CInterfaceMap m_interfaces;                     // +0x44
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
};

class CTCSpecialAbilities {                         // interface 0xA7
public:
    unsigned char _pad00[0x0c];
    bool AbilityMutexLock;                          // +0x0C
    bool UsingBed;                                  // +0x0D
    long CurrentAbility;                            // +0x10 EHeroAbility
};
struct CTCSoundPlayer {                             // interface 0x10
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};
struct CTCHeroStats {                               // interface 4
    unsigned char _pad00[0x58];
    long Stamina;                                   // +0x58 (name inferred)
};

struct CWillButtonHelperTimer {                     // retail 0x34 bytes
    CWillButtonHelperTimer(long ability, CThing* creature);   // 0x008EDFF0
    unsigned char _data[0x34];
};

extern long g_FramesPerSecond;                      // 0x01375550

class CTCBase {
public:
    CTCBase(CThing* parent);                        // 0x00686800
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x4];
};

#pragma pack(push, 2)
class CTCFireballSpell : public CTCBase {           // vtable 0x0126E964
public:
    CWillButtonHelperTimer Timer;                   // +0x0c
    CDefPointer<const CSpecialAbilitiesFireballSpellDef> PDef;   // +0x40
    unsigned long InitLoopingSound;                 // +0x44
    CIntelligentPointer<CThing> FireballBuildUpEffect;   // +0x48
    bool ImmediateCastMode;                         // +0x50
    bool OneAnimCastMode;                           // +0x51
    CIntelligentPointer<CThing> PTarget;            // +0x52
    CIntelligentPointer<CThing> PItemRemoved;       // +0x5a
    unsigned char _pad62[2];
    long WeaponState;                               // +0x64
    bool LeftHandAnim;                              // +0x68
    unsigned char _pad69[3];
    CCharString FireballAttachmentName;             // +0x6c
    bool StartEffect;                               // +0x70
    unsigned char _pad71[3];
    long MaxLevelAllowableByStamina;                // +0x74
    long ReleaseDelayFrameCount;                    // +0x78
    bool SpellFinished;                             // +0x7c
    CTCFireballSpell(CThing& parent);
    virtual void v00();
};
#pragma pack(pop)

CTCFireballSpell::CTCFireballSpell(CThing& parent)
    : CTCBase(&parent), Timer(0x10, ParentThing), InitLoopingSound(0), ImmediateCastMode(false), OneAnimCastMode(false),
      PTarget(0), PItemRemoved(0), WeaponState(0), LeftHandAnim(true), FireballAttachmentName(""), StartEffect(true),
      MaxLevelAllowableByStamina(0), ReleaseDelayFrameCount(0), SpellFinished(false)
{
    CTCSpecialAbilities* abilities;
    if (ParentThing->GetInterface(&abilities, 0xa7)) {
        abilities->AbilityMutexLock = true;
        abilities->CurrentAbility = 0x10;
    }
    long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_FIREBALL_SPELL_DEF", 1);
    CGameDefinitionManager::Get().GetDef(index, &PDef);
    CTCSoundPlayer* sound;
    if (ParentThing->GetInterface(&sound, 0x10))
        InitLoopingSound = sound->PlaySound("HERO_ABILITY_FIREBALL_LOOP", 1, 0, 0, -1);
    CTCHeroStats* stats;
    if (ParentThing->GetInterface(&stats, 4)) {
        long stamina = stats->Stamina;
        long level;
        for (level = 0; level < 4; ++level)
            if ((float)stamina < PDef->StaminaLevelThresholds[level])
                break;
        MaxLevelAllowableByStamina = level - 1;
        if (MaxLevelAllowableByStamina < 0)
            MaxLevelAllowableByStamina = 0;
    }
    ReleaseDelayFrameCount = (long)(g_FramesPerSecond * PDef->ReleaseDelaySeconds);
}

#include <cstdio>
#include <cstring>
#include <cstdlib>
CCharString::CCharString(const char* text, long) : PStringData((void*)text) { }
CCharString::~CCharString() { }
void CSpecialAbilitiesFireballSpellDef::v00() { }
static CSpecialAbilitiesFireballSpellDef g_def; static float g_thr[4] = { 10.0f, 20.0f, 30.0f, 40.0f };
static const char* g_className = 0; static long g_classIndex = -1;
long CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString& name, long idx) const { g_className = (const char*)name.PStringData; g_classIndex = idx; return 77; }
template<class T> bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const { if (index != 77) return false; out->Object = (const T*)&g_def; return true; }
static CGameDefinitionManager g_gdm;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_gdm; }
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem((CThing*)1) { }
void CBaseIntelligentPointer::v00() { }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; }
template<class T> void CIntelligentPointer<T>::v00() { }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static const char* g_sound = 0;
unsigned long CTCSoundPlayer::PlaySound(const CCharString& name, long a, long, long, long d) { g_sound = (a == 1 && d == -1) ? (const char*)name.PStringData : "bad"; return 555; }
static long g_timerAbility = -1; static CThing* g_timerThing = 0;
CWillButtonHelperTimer::CWillButtonHelperTimer(long ability, CThing* creature) { g_timerAbility = ability; g_timerThing = creature; }
long g_FramesPerSecond = 30;
CTCBase::CTCBase(CThing* parent) : ParentThing(parent) { }
void CTCBase::v00() { }
void CTCFireballSpell::v00() { }

int main()
{
    g_def.StaminaLevelThresholds = g_thr; g_def.ReleaseDelaySeconds = 0.5f;
    static CThing thing; static CTCSpecialAbilities abilities; static CTCSoundPlayer player; static CTCHeroStats stats;
    CInterfaceMap::SEntry e[3];
    e[0].type = 4; e[0].component = (CTCBase*)&stats; e[1].type = 0x10; e[1].component = (CTCBase*)&player; e[2].type = 0xa7; e[2].component = (CTCBase*)&abilities;
    thing.m_interfaces.first = e; thing.m_interfaces.last = e + 3;
    thing.m_presentBits[0] = (1UL << 4) | (1UL << 16); thing.m_presentBits[5] = 1UL << 7;
    stats.Stamina = 25;
    void* mem = std::malloc(sizeof(CTCFireballSpell)); std::memset(mem, 0xcd, sizeof(CTCFireballSpell));
    CTCFireballSpell* f = new (mem) CTCFireballSpell(thing);
    int ok = 1;
    if (g_timerAbility != 0x10 || g_timerThing != &thing || !abilities.AbilityMutexLock || abilities.CurrentAbility != 0x10) ok = 0;
    if (!g_className || std::strcmp(g_className, "SPECIAL_ABILITIES_FIREBALL_SPELL_DEF") || g_classIndex != 1 || f->PDef.Object != &g_def) ok = 0;
    if (f->InitLoopingSound != 555 || !g_sound || std::strcmp(g_sound, "HERO_ABILITY_FIREBALL_LOOP")) ok = 0;
    if (f->MaxLevelAllowableByStamina != 1 || f->ReleaseDelayFrameCount != 15) ok = 0;
    if (f->ImmediateCastMode || f->OneAnimCastMode || f->WeaponState != 0 || !f->LeftHandAnim || !f->StartEffect || f->SpellFinished) ok = 0;
    if (f->PTarget.m_pItem != 0 || f->PItemRemoved.m_pItem != 0 || f->FireballBuildUpEffect.m_pItem != (CThing*)1) ok = 0;
    // low stamina clamps to 0; no components: sound and level untouched
    stats.Stamina = 5;
    f = new (mem) CTCFireballSpell(thing);
    if (f->MaxLevelAllowableByStamina != 0) ok = 0;
    stats.Stamina = 99;
    f = new (mem) CTCFireballSpell(thing);
    if (f->MaxLevelAllowableByStamina != 3) ok = 0;
    thing.m_presentBits[0] = 0; thing.m_presentBits[5] = 0;
    f = new (mem) CTCFireballSpell(thing);
    if (f->InitLoopingSound != 0 || f->MaxLevelAllowableByStamina != 0) ok = 0;
    if (!ok) { std::printf("007c87f0_TEST FAIL\n"); return 1; }
    std::printf("007c87f0_TEST PASS\n");
    return 0;
}