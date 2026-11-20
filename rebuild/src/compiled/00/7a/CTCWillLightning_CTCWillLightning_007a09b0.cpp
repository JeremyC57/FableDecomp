// CTCWillLightning::CTCWillLightning(CThing&) -- construct the lightning-spell component: default members, look up the spell def, play its init sound, read the beam/hand effect indices, bind the caster, take the ability level and the starting state
// Leaf: ??0CTCWillLightning@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model (CTCBase_Calls, which bound a made-up constructor name to
//   0x00686800) is gone; the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it.
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Naming wave 18: the global stand-in g_FrameRate (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A09B0, 952 bytes (+ int3 padding), `ret 4`, vtable 0x0126C724.
// Callees: CTCBase::CTCBase(CThing&) 0x00686800; CBaseIntelligentPointer() 0x00A01B10 / ~ 0x00A01C10 / GetPItem 0x00A01B50
//   / SetPItem 0x00A01B90; CWillButtonHelperTimer(EHeroAbility, CThing*) 0x008EDFF0; malloc 0x00BFEA0E (list sentinel);
//   CGameDefinitionManager::Get 0x0044C6B0 / the def index by name 0x009AD390 / GetDef 0x007A35D0; LowerBound 0x0040F020
//   (interfaces 0xA7, 0x10, 4) and the const 0x004365B0 (0x6F); CDefStringTable::GetString 0x009D49B0 on 0x013CA828;
//   CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00; the effects sub-def 0x00669580 and its named lookup 0x00669D30;
//   CTCInventoryAbilities::GetAbilityLevel 0x005EDE93; CTCWillLightning::SetLevel 0x0079F810; __ftol2;
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0. Vtables: CIntelligentPointer<CThing> 0x01238C6C, <CThingCreatureBase> 0x0123E7D8.
// Layout: retail CTCWillLightning as the landed ~CTCWillLightning 0x007A07F0 (pack(1), PItemRemoved +0x11, m_lspDef +0x28,
//   m_tTimer +0x2c, m_lbBeams +0x68); InitSound +0x90, StartCostingStamina +0x94, BeamsInitialised +0x95, LoopingSound
//   +0x98, m_bPlayedSound +0xd8, m_OnhitEveryFrame +0xdc, m_CombatMultiplierTimer +0xe0.
// Naming wave 17: the CWillButtonHelperTimer ctor is c672's (EHeroAbility, CThing&) one (??0CWillButtonHelperTimer@@QAE@W4EHeroAbility@@AAVCThing@@@Z);
//   the ability is a cast of the landed value (no enumerator ordinal source) -- name-only.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
#include "engine/EHeroAbility.h"   // NF34: ?GetAbilityLevel@CTCInventoryAbilities@@QBEJW4EHeroAbility@@@Z (Ego_r + FableWin)
#include "fable_compiler.h"
// NF49 (round 7): the def classes come from the shared headers (were local CDefStringTable / CDefNative / CEffectsDef / CEffectsDefPointer / CLightningSpellDef / CLightningSpellDefPointer / CGameDefinitionManager / CCharString models with same-TU stand-in bodies over made-up GetSubDefIndex / GetEffectsDefFromGlobalIndex callees): the call at
// 0x00669580 is the member template ??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z
// (Ego_r.pdb / FableWin.pdb), its body visible and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10
// idiom).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_FORCEINLINE_GETSTRING
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CSpecialAbilitiesLightningSpellDef.h"
// NF49: the manager and the lightning def come from engine/CGameDefinitionManager.h and engine/CSpecialAbilitiesLightningSpellDef.h
// (were local CDefinitionManager / CGameDefinitionManager / CLightningSpellDef / CLightningSpellDefPointer / CDefStringTable models):
// GetDef 0x007A35D0 is the member template ??$GetDef@VCSpecialAbilitiesLightningSpellDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCSpecialAbilitiesLightningSpellDef@@@@@Z
// (FableWin.pdb; declared only, as the row had it), the def's InitSound (+0x110, Ego_r) is read through CDefString::GetString
// (FABLE_RETAIL_CDEFSTRING_FORCEINLINE_GETSTRING: GDefStringTable.GetString(TablePos) in place) and CombatMultiplierIncreaseTime is +0x10C.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
class CTCBase;
extern unsigned long g_InvalidSoundHandle;              // 0x0129D25C
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
extern long g_LightningBeamLargeEffect;                 // 0x013BAE14
extern long g_LightningBeamEffect;                      // 0x013BAE10

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingType;                            // +0x10 (descriptive)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> FABLE_FORCEINLINE T* GetTC(long wanted)
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || wanted < it->first)
            it = map->end();
        return (T*)it->second;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    template<class T> FABLE_FORCEINLINE T* GetTCConst(long wanted) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)((it == map->end() || wanted < it->first) ? map->end() : it)->second;
    }
};
struct CTCAbilityUser {                                 // interface 0xA7 (descriptive)
    unsigned char _pad00[0x0c];
    bool Active;                                        // +0x0c
    unsigned char _pad0d[3];
    long Ability;                                       // +0x10
};
class CTCSoundPlayer {
public:
    unsigned long PlayAttachedCriteria(CCharString const&a0, bool a1, bool a2, bool a3, long a4);   // 0x0066BD00 (Ego_r.pdb ?PlayAttachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@_N11J@Z)
private:    // 0x0066BD00
};
class CTCInventoryAbilities { public:                          // interface 0x6F
    long GetAbilityLevel(EHeroAbility ability) const;                 // 0x005EDE93
};
class CTCHeroStats { public:                                   // interface 4
    unsigned char _pad00[0x58];
    long WillPower;                                     // +0x58 (descriptive)
};

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject*a0);                        // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(CThing* item) { SetPItem((CBaseObject*)item); }
};
struct CCreaturePointer : CBaseIntelligentPointer {     // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CCreaturePointer() { }
    CCreaturePointer(CThing* item) { SetPItem((CBaseObject*)item); }
    CCreaturePointer& operator=(const CCreaturePointer& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
};


// (NF34: the local empty EHeroAbility enum is replaced by engine/EHeroAbility.h, included above)
struct CWillButtonHelperTimer {                         // retail 0x34 bytes
    unsigned char _pad00[0x2e];
    bool IsPlayer;                                      // +0x2e (descriptive)
    unsigned char _pad2f[0x34 - 0x2f];
    CWillButtonHelperTimer(EHeroAbility ability, CThing& thing);   // 0x008EDFF0
};
struct CBeamList {
    struct node { node* next; node* prev; void* data; void* info; };
    node* head;
    CBeamList() : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};

#pragma pack(push, 1)
class CTCWillLightning : public CTCBase { public:               // vtable 0x0126C724
    long  WeaponState;                                  // +0x0c
    bool  LeftHandAnim;                                 // +0x10
    CIntelligentPointer PItemRemoved;                   // +0x11
    unsigned char _pad19[3];
    long  AIHoldingDownTimer;                           // +0x1c
    long  PrimaryHandEffect;                            // +0x20
    long  SecondaryHandEffect;                          // +0x24
    CDefPointer<const CSpecialAbilitiesLightningSpellDef> m_lspDef;   // +0x28
    CWillButtonHelperTimer m_tTimer;                    // +0x2c
    float m_fTime;                                      // +0x60
    float m_fTimeBase;                                  // +0x64
    CBeamList m_lbBeams;                                // +0x68
    long  m_lsState;                                    // +0x6c
    CCreaturePointer m_cbpPlayer;                       // +0x70
    CIntelligentPointer m_tpLeftHandEffect;             // +0x78
    CIntelligentPointer m_tpRightHandEffect;            // +0x80
    CIntelligentPointer PTargetOverride;                // +0x88
    unsigned long InitSound;                            // +0x90
    bool  StartCostingStamina;                          // +0x94
    bool  BeamsInitialised;                             // +0x95
    unsigned char _pad96[2];
    unsigned long LoopingSound;                         // +0x98
    unsigned char _pad9c[0xd8 - 0x9c];
    bool  m_bPlayedSound;                               // +0xd8
    unsigned char _padd9[3];
    long  m_OnhitEveryFrame;                            // +0xdc
    long  m_CombatMultiplierTimer;                      // +0xe0
private:   // NF120 (PRIORITY q5161): Ego_r.pdb / FableWin.pdb ?SetLevel@CTCWillLightning@@AAEXJ@Z (private); was public (QAEXJ)
    void SetLevel(long level);                          // 0x0079F810
public:
    CTCWillLightning(CThing& parent);
};
#pragma pack(pop)

CTCWillLightning::CTCWillLightning(CThing& parent)
    : CTCBase(parent)
    , WeaponState(0)
    , LeftHandAnim(true)
    , PItemRemoved(0)
    , PrimaryHandEffect(0)
    , SecondaryHandEffect(0)
    , m_tTimer((EHeroAbility)11, *ParentThing)
    , StartCostingStamina(false)
    , LoopingSound(g_InvalidSoundHandle)
    , m_OnhitEveryFrame(0)
    , m_CombatMultiplierTimer(0)
{
    if (ParentThing->HasTC(0xa7)) {
        CTCAbilityUser* user = ParentThing->GetTC<CTCAbilityUser>(0xa7);
        user->Active = true;
        user->Ability = 11;
    }
    InitSound = g_InvalidSoundHandle;
    long index;
    {
        CCharString defName("SPECIAL_ABILITIES_LIGHTNING_SPELL_DEF");
        index = ((CDefinitionManager*)&(CGameDefinitionManager::Get()))->GetGlobalIndexFromClassIndex(defName, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &m_lspDef);
    if (ParentThing->HasTC(0x10)) {
        CTCSoundPlayer* sound = ParentThing->GetTC<CTCSoundPlayer>(0x10);
        {
            CCharString name = m_lspDef->InitSound.GetString();
            ((CTCSoundPlayer*)sound)->PlayAttachedCriteria(name, 0, 0, 0, -1);
        }
    }
    CDefPointer<const CSpecialEffectsDef> effects;
    if (m_lspDef->GetSubDef(&effects)) {
        effects->GetEffectFromString("LIGHTNING_BEAM_LARGE_EFFECT", g_LightningBeamLargeEffect);
        effects->GetEffectFromString("LIGHTNING_BEAM_EFFECT", g_LightningBeamEffect);
        effects->GetEffectFromString("LIGHTNING_HAND_EFFECT", PrimaryHandEffect);
        effects->GetEffectFromString("LIGHTNING_HAND_EFFECT_NO_LIGHT", SecondaryHandEffect);
    }
    m_cbpPlayer = CCreaturePointer(ParentThing);
    long level = 0;
    CThing* caster = ((CThing*)m_cbpPlayer.GetPItem());
    if (caster->HasTC(0x6f))
        level = caster->GetTCConst<CTCInventoryAbilities>(0x6f)->GetAbilityLevel(HERO_ABILITY_LIGHTNING_SPELL);
    SetLevel(level);
    if (ParentThing->ThingType == 1)
        m_tTimer.IsPlayer = true;
    CThing* player = ((CThing*)m_cbpPlayer.GetPItem());
    CTCHeroStats* stats;
    if (player->GetInterface(&stats, 4) && stats->WillPower <= 0) {
        m_lsState = 6;
    } else {
        m_lsState = 2;
        m_fTime = 0.0f;
        m_fTimeBase = 0.0f;
        m_bPlayedSound = false;
    }
    BeamsInitialised = false;
    m_CombatMultiplierTimer = (long)(CMainGameComponent::GetConstantFPS() * m_lspDef->CombatMultiplierIncreaseTime);
    CGameDefinitionManager::Get();
}