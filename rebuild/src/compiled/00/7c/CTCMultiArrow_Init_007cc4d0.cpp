// CTCMultiArrow::Init() -- load SPECIAL_ABILITIES_MULTI_ARROW_DEF, take the spell level from the owner's ability inventory, derive the shot/volley counts and the ammo-effect delay from the def, make the cast noise, put the effect on the weapon, create the first ammo effect, spawn the def's CAST_EFFECT at the owner and play the def's cast sound
// Leaf: ?Init@CTCMultiArrow
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CC4D0, 563 bytes, plain `ret`.
// Callees: CCharString @ 0x0099EBF0 / 0x0099EAE0, CGameDefinitionManager::Get @ 0x0044C6B0 (__fastcall),
//   CDefinitionManager::GetGlobalIndexFromClassIndex @ 0x009AD390, GetDef<CSpecialAbilitiesMultiArrowDef> @ 0x007CC8E0,
//   const LowerBound @ 0x004365B0 (interfaces 0x6F, 0x13) / LowerBound @ 0x0040F020 (0x10), CTCInventoryAbilities::
//   GetAbilityLevel @ 0x005EDE93 (ability 0x11), __ftol2 @ 0x00BFEA70, CThingCreatureBase::MakeNoise @ 0x00662110,
//   CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCMultiArrow::ApplyEffectOnWeapon @ 0x007CC060 / CreateAmmoEffect
//   @ 0x007CC2B0, GetSubDef<CSpecialEffectsDef> @ 0x00669580, CalcAndLookup @ 0x00669D30, CThing::GetPosition @ 0x004C73D0,
//   CreateParticleEffect @ 0x0072AB70 (__fastcall), CDefStringTable::GetString @ 0x009D49B0 (table 0x013CA828),
//   CTCSoundPlayer::PlaySound @ 0x0066BD00; the def's slot 1.
// Data: "SPECIAL_ABILITIES_MULTI_ARROW_DEF" 0x01235AC0, "SIMPLE_NOISE" 0x01268A5C, "CAST_EFFECT" 0x0126EDF8, frames per
//   second (long) 0x01375550.
// Layout: CTCMultiArrow as Ego_r.pdb (landed siblings): PDef +0x0C, NumberOfShots +0x10, NumberInVolley +0x14, NumOfVolleys
//   +0x18, SpellLevel +0x1C, CreateAmmoEffectCount/Delay/Timer +0x24/+0x28/+0x2C. CSpecialAbilitiesMultiArrowDef is 4 bytes
//   below Ego_r in retail: NumberOfShots[4] +0x3C, NumberInVolley[4] +0x4C, AmmoEffectCreationDelaySecs +0x5C, CastSound +0x60.
class CThing;
class CTCBase;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct C3DVector { float X, Y, Z; };

struct CDefPointeeBase {
    virtual void Destroy();
    virtual void Delete();                              // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
struct CSpecialEffectsDef : CDefPointeeBase {
    bool CalcAndLookup(const CCharString& name, long* value);   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_CalcAndLookup(const void* def, const CCharString& name);   // stand-in callee
#pragma auto_inline(off)
// Stand-in body (out of line in retail): writes the out value only, so the effect index's address does not escape.
__declspec(noinline) bool CSpecialEffectsDef::CalcAndLookup(const CCharString& name, long* value)
{
    long found = Lookup_CSpecialEffectsDef_CalcAndLookup(this, name);
    if (found < 0)
        return false;
    *value = found;
    return true;
}
#pragma auto_inline(on)
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580
};
void* FindDef_CDefClassBase_GetSubDef_0();
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef<CSpecialEffectsDef> (the landed 0x007CC2B0 idiom): writes the out slot only.
bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CSpecialAbilitiesMultiArrowDef : CDefClassBase {
    unsigned char _pad08[0x3c - 0x08];
    long  NumberOfShots[4];                             // +0x3c
    long  NumberInVolley[4];                            // +0x4c
    float AmmoEffectCreationDelaySecs;                  // +0x5c
    long  CastSound;                                    // +0x60 (CDefString)
};

class CDefinitionManager {
public:
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const;   // 0x009AD390
    bool GetDef(long index, CDefPointer<const CSpecialAbilitiesMultiArrowDef>* out) const;    // 0x007CC8E0
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
struct CDefStringTable {
    CCharString GetString(long id);                    // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;               // 0x013CA828
extern long g_FramesPerSecond;                         // 0x01375550

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
    SEntry* LowerBound(const long* type) const;        // 0x004365B0
};

#pragma auto_inline(off)
// Same-TU body of the non-const CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound (0x0040F020): the key's address then
// does not escape, so the interface-0x10 key shares the effect index's frame word as in retail.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                   // +0x04
    CThing* GetPItem() const;                          // 0x00A01B50
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
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
    const C3DVector& GetPosition() const;              // 0x004C73D0
    void MakeNoise(const CCharString& noise);          // 0x00662110 (CThingCreatureBase)
};
CThing* __fastcall CreateParticleEffect(long type, const C3DVector& position);   // 0x0072AB70

class CTCInventoryAbilities {
public:
    long GetAbilityLevel(long ability);                // 0x005EDE93 (EHeroAbility)
};
class CTCSoundPlayer {
public:
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};
#pragma pack(push, 1)
struct CTCWeaponHolder {                               // interface 0x13 (descriptive)
    unsigned char _pad00[0x141];
    CBaseIntelligentPointer PWeapon;                   // +0x141
};
#pragma pack(pop)

class CTCMultiArrow {
public:
    void*         __vftable;                           // +0x00
    CThing*       ParentThing;                         // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CDefPointer<const CSpecialAbilitiesMultiArrowDef> PDef;   // +0x0c
    long          NumberOfShots;                       // +0x10
    long          NumberInVolley;                      // +0x14
    long          NumOfVolleys;                        // +0x18
    long          SpellLevel;                          // +0x1c
    bool          RemovedEffect;                       // +0x20
    long          CreateAmmoEffectCount;               // +0x24
    long          CreateAmmoEffectDelay;               // +0x28
    long          CreateAmmoEffectTimer;               // +0x2c
    void ApplyEffectOnWeapon(CThing* weapon);          // 0x007CC060
    void CreateAmmoEffect();                           // 0x007CC2B0
    void Init();
};

void CTCMultiArrow::Init()
{
    CThing* parent = ParentThing;
    long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_MULTI_ARROW_DEF", 1);
    CGameDefinitionManager::Get().GetDef(index, &PDef);
    const CThing* parentThing = parent;
    if (parentThing->HasTC(0x6f))
        SpellLevel = ((CTCInventoryAbilities*)parentThing->GetTC(0x6f))->GetAbilityLevel(0x11);
    NumberOfShots = PDef->NumberOfShots[SpellLevel];
    NumberInVolley = PDef->NumberInVolley[SpellLevel];
    NumOfVolleys = NumberOfShots / NumberInVolley;
    CreateAmmoEffectCount = NumOfVolleys;
    CreateAmmoEffectDelay = CreateAmmoEffectTimer = (long)(g_FramesPerSecond * PDef->AmmoEffectCreationDelaySecs);
    parent->MakeNoise("SIMPLE_NOISE");
    CTCWeaponHolder* holder;
    ((const CThing*)ParentThing)->GetInterface(&holder, 0x13);
    ApplyEffectOnWeapon(holder->PWeapon.GetPItem());
    CreateAmmoEffect();
    CDefPointer<CSpecialEffectsDef> effects;
    {
        long effect = 0;
        bool found = ((CDefClassBase*)PDef.Object)->GetSubDef(&effects) && effects->CalcAndLookup("CAST_EFFECT", &effect);
        if (found)
            CreateParticleEffect(effect, parent->GetPosition());
    }
    CTCSoundPlayer* player;
    if (parent->GetInterface(&player, 0x10)) {
        CCharString sound = g_DefStringTable.GetString(PDef->CastSound);
        player->PlaySound(sound, 1, 0, 0, -1);
    }
}