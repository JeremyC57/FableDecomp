// CTCFireballSpell::CTCFireballSpell(CThing& parent) -- constructor: CTCBase, the will-button timer for the fireball ability,
// cleared def/sound/effect/target state; take the special-ability lock, load SPECIAL_ABILITIES_FIREBALL_SPELL_DEF, start the
// looping build-up sound, cap the castable level by the hero's stamina thresholds and convert the release delay to frames
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C87F0, 510 bytes, `ret 4`; vtable 0x0126E964. No EH frame (TU without /EH).
// Leaf: ??0CTCFireballSpell
// Callees: CTCBase(CThing*) 0x00686800 (catalog "ClockSprite"), CWillButtonHelperTimer(EHeroAbility, CThing*) 0x008EDFF0,
// CBaseIntelligentPointer 0x00A01B10 / SetPItem 0x00A01B90 (CIntelligentPointer<CThing> vtable 0x01238C6C), CCharString
// 0x0099EBF0 / ~ 0x0099EAE0 ("" 0x0122D70E), CGameDefinitionManager::Get 0x0044C6B0, GetGlobalIndexFromClassIndex 0x009AD390,
// GetDef<CSpecialAbilitiesFireballSpellDef> 0x005F8A57, LowerBound 0x0040F020 (0xA7 special abilities: `test cl,cl ; jns`,
// 0x10 sound player, 4 hero stats), CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00, __ftol2 0x00BFEA70; g_FramesPerSecond 0x01375550.
// Shape: the CTCForcePushPower ctor (0x007A54A0) init list sharing the ebx zero; implicit-literal string temps; the stamina
// level loop converts `(float)stamina` INSIDE the loop condition from a named long -- VC hoists it as loop-invariant through a
// stack slot (`mov ecx,[eax+58h] ; mov [esp+14h],ecx ; fild ; fstp` to the same word) and aligns the loop head with
// `mov edi,edi`; converting once before the loop gives a direct `fild [eax+58h]` (10 bytes short).
// Layout: Ego_r.pdb CTCFireballSpell minus 4 after the 0x34-byte retail timer; PTarget at the packed +0x52 (pack(2) + explicit
// pads). Def +0x3c / +0x44 and CTCHeroStats +0x58 names inferred. No engine header.
// Layout (supersedes any local-model note above): engine/CTCHeroStats.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCHeroStats CTCHeroStatsDonor
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
class CThing;
class CBaseObject;
class CTCBase;

class CCharString { public:
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
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);           // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20
    CInterfaceMap m_interfaces;                     // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
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
class CTCSoundPlayer { public:                             // interface 0x10
public:
    unsigned long PlayAttachedCriteria(const CCharString&, bool, bool, bool, long);   // 0x0066BD00
};
class CTCHeroStats : public CTCHeroStatsDonor {
 public:                               // interface 4
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
        InitLoopingSound = sound->PlayAttachedCriteria("HERO_ABILITY_FIREBALL_LOOP", 1, 0, 0, -1);
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