// CTCWillLightning::CTCWillLightning(CThing*) -- construct the lightning-spell component: default members, look up the spell def, play its init sound, read the beam/hand effect indices, bind the caster, take the ability level and the starting state
// Leaf: ??0CTCWillLightning
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A09B0, 952 bytes (+ int3 padding), `ret 4`, vtable 0x0126C724.
// Callees: CTCBase::CTCBase(CThing*) 0x00686800; CBaseIntelligentPointer() 0x00A01B10 / ~ 0x00A01C10 / GetPItem 0x00A01B50
//   / SetPItem 0x00A01B90; CWillButtonHelperTimer(EHeroAbility, CThing*) 0x008EDFF0; malloc 0x00BFEA0E (list sentinel);
//   CGameDefinitionManager::Get 0x0044C6B0 / the def index by name 0x009AD390 / GetDef 0x007A35D0; LowerBound 0x0040F020
//   (interfaces 0xA7, 0x10, 4) and the const 0x004365B0 (0x6F); CDefStringTable::GetString 0x009D49B0 on 0x013CA828;
//   CTCSoundPlayer::PlaySound 0x0066BD00; the effects sub-def 0x00669580 and its named lookup 0x00669D30;
//   CTCInventoryAbilities::GetAbilityLevel 0x005EDE93; CTCWillLightning::SetLevel 0x0079F810; __ftol2;
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0. Vtables: CIntelligentPointer<CThing> 0x01238C6C, <CThingCreatureBase> 0x0123E7D8.
// Layout: retail CTCWillLightning as the landed ~CTCWillLightning 0x007A07F0 (pack(1), PItemRemoved +0x11, m_lspDef +0x28,
//   m_tTimer +0x2c, m_lbBeams +0x68); InitSound +0x90, StartCostingStamina +0x94, BeamsInitialised +0x95, LoopingSound
//   +0x98, m_bPlayedSound +0xd8, m_OnhitEveryFrame +0xdc, m_CombatMultiplierTimer +0xe0.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
class CTCBase;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefStringTable {
    CCharString GetString(long id);                     // 0x009D49B0
};
extern CDefStringTable g_StringTable;                   // 0x013CA828
extern unsigned long g_InvalidSoundHandle;              // 0x0129D25C
extern long g_FrameRate;                                // 0x01375550
extern long g_LightningBeamLargeEffect;                 // 0x013BAE14
extern long g_LightningBeamEffect;                      // 0x013BAE10

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingType;                            // +0x10 (descriptive)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || wanted < it->type)
            it = map->last;
        return (T*)it->component;
    }
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
    template<class T> __forceinline T* GetTCConst(long wanted) const
    {
        long type = wanted;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
struct CTCAbilityUser {                                 // interface 0xA7 (descriptive)
    unsigned char _pad00[0x0c];
    bool Active;                                        // +0x0c
    unsigned char _pad0d[3];
    long Ability;                                       // +0x10
};
struct CTCSoundPlayer {                                 // interface 0x10
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};
struct CTCInventoryAbilities {                          // interface 0x6F
    long GetAbilityLevel(long ability);                 // 0x005EDE93
};
struct CTCHeroStats {                                   // interface 4
    unsigned char _pad00[0x58];
    long WillPower;                                     // +0x58 (descriptive)
};

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(CThing* item) { SetPItem(item); }
};
struct CCreaturePointer : CBaseIntelligentPointer {     // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CCreaturePointer() { }
    CCreaturePointer(CThing* item) { SetPItem(item); }
    CCreaturePointer& operator=(const CCreaturePointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CDefNative {
    virtual void v00();
    virtual void Delete();
    long RefCount;                                      // +0x04
};
struct CEffectsDef : CDefNative {
    void Lookup(const CCharString& name, long* out);    // 0x00669D30
};
struct CEffectsDefPointer {
    CEffectsDef* Object;
    CEffectsDefPointer() : Object(0) { }
    ~CEffectsDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Delete(); } }
    CEffectsDef* operator->() const { return Object; }
};
struct CLightningSpellDef : CDefNative {
    unsigned char _pad08[0x10c - 0x08];
    float CombatMultiplierTime;                         // +0x10c (descriptive)
    long InitSoundName;                                 // +0x110 (descriptive)
    bool GetSubDef(CEffectsDefPointer* out);            // 0x00669580
};
struct CLightningSpellDefPointer {
    CLightningSpellDef* Object;
    CLightningSpellDefPointer() : Object(0) { }
    CLightningSpellDef* operator->() const { return Object; }
};
long* GetSubDefIndex(CLightningSpellDef* def, const CCharString& name);
CEffectsDef* GetEffectsDefFromGlobalIndex(long index);
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef 0x00669580: VC7.1 then knows the out pointer's address does not escape, so the
//   effects def is held in a register for the lookups and its release (as retail).
bool CLightningSpellDef::GetSubDef(CEffectsDefPointer* out)
{
    long* pIndex = GetSubDefIndex(this, "CEffectsDef");
    if (pIndex) {
        out->Object = GetEffectsDefFromGlobalIndex(*pIndex);
        return true;
    }
    return false;
}
#pragma auto_inline(on)
struct CGameDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();    // 0x0044C6B0
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const;   // 0x009AD390
    bool GetDef(long index, CLightningSpellDefPointer* out) const;   // 0x007A35D0
};

struct CWillButtonHelperTimer {                         // retail 0x34 bytes
    unsigned char _pad00[0x2e];
    bool IsPlayer;                                      // +0x2e (descriptive)
    unsigned char _pad2f[0x34 - 0x2f];
    CWillButtonHelperTimer(long ability, CThing* thing);   // 0x008EDFF0
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

struct CTCBase_Calls {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[4];
    CTCBase_Calls(CThing* parent);                      // 0x00686800
};

#pragma pack(push, 1)
struct CTCWillLightning : CTCBase_Calls {               // vtable 0x0126C724
    long  WeaponState;                                  // +0x0c
    bool  LeftHandAnim;                                 // +0x10
    CIntelligentPointer PItemRemoved;                   // +0x11
    unsigned char _pad19[3];
    long  AIHoldingDownTimer;                           // +0x1c
    long  PrimaryHandEffect;                            // +0x20
    long  SecondaryHandEffect;                          // +0x24
    CLightningSpellDefPointer m_lspDef;                 // +0x28
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
    virtual void v00();
    void SetLevel(long level);                          // 0x0079F810
    CTCWillLightning(CThing* parent);
};
#pragma pack(pop)

CTCWillLightning::CTCWillLightning(CThing* parent)
    : CTCBase_Calls(parent)
    , WeaponState(0)
    , LeftHandAnim(true)
    , PItemRemoved(0)
    , PrimaryHandEffect(0)
    , SecondaryHandEffect(0)
    , m_tTimer(11, ParentThing)
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
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(defName, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &m_lspDef);
    if (ParentThing->HasTC(0x10)) {
        CTCSoundPlayer* sound = ParentThing->GetTC<CTCSoundPlayer>(0x10);
        {
            CCharString name = g_StringTable.GetString(m_lspDef->InitSoundName);
            sound->PlaySound(name, 0, 0, 0, -1);
        }
    }
    CEffectsDefPointer effects;
    if (m_lspDef->GetSubDef(&effects)) {
        effects->Lookup("LIGHTNING_BEAM_LARGE_EFFECT", &g_LightningBeamLargeEffect);
        effects->Lookup("LIGHTNING_BEAM_EFFECT", &g_LightningBeamEffect);
        effects->Lookup("LIGHTNING_HAND_EFFECT", &PrimaryHandEffect);
        effects->Lookup("LIGHTNING_HAND_EFFECT_NO_LIGHT", &SecondaryHandEffect);
    }
    m_cbpPlayer = CCreaturePointer(ParentThing);
    long level = 0;
    CThing* caster = m_cbpPlayer.GetPItem();
    if (caster->HasTC(0x6f))
        level = caster->GetTCConst<CTCInventoryAbilities>(0x6f)->GetAbilityLevel(11);
    SetLevel(level);
    if (ParentThing->ThingType == 1)
        m_tTimer.IsPlayer = true;
    CThing* player = m_cbpPlayer.GetPItem();
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
    m_CombatMultiplierTimer = (long)(g_FrameRate * m_lspDef->CombatMultiplierTime);
    CGameDefinitionManager::Get();
}