// CCreatureAction_HeroLoadRangedWeapon::Initialise() -- base Initialise; for a hero with an experience component (0x68)
// and a carrying component, register a loading-speed haste multiplier for this action: the primary weapon's ability def
// gives the multiplier range, the hero's stats def the ability level (adding a CTCHaste when missing)
// Leaf: ?Initialise@CCreatureAction_HeroLoadRangedWeapon
// Layout: CThing interface bitset +0x20, map +0x44, def +0x70; CAbilityDef multiplier max +0xbc / min +0xc0; the stats def's
//   ability table at +0x9c; CTCHaste multipliers map at +0x18.
// Shape: the 0x68 / 0x46 lookups are const (LowerBound 0x004365B0), the 0x8C one non-const (0x0040F020); the stats def is
//   returned by value; the ability def pointer is declared before the single `stats && GetSubDef` test (that order gives
//   retail's slots and the zero register); CDefPointer's destructor nulls the pointer after the release.
class CTCBase;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CDefPointeeBase { public:
    virtual void v000();
    virtual void DestroyDef() const;                  // slot 1
    void Release() const
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
    mutable long m_refCount;                          // +4
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
};
class CTCHeroExperience;
struct CAbilityTable {
    float GetAbilityMultiplier(const CTCHeroExperience* experience) const;   // 0x00851BD0
};
class CHeroStatsDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x9c - 8];
    CAbilityTable Abilities;                          // +0x9c
};
class CAbilityDef : public CDefPointeeBase { public:
    unsigned char _pad08[0xbc - 8];
    float MaxMultiplier;                              // +0xbc
    float MinMultiplier;                              // +0xc0
};
class CDefClassBase { public:
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long m_refCount;                                  // +0x04
    bool GetSubDef(CDefPointer<const CAbilityDef>* out);   // 0x0042B1A6
};
struct CDefClassReference {                           // a counted CDefClassBase reference (AddRef / Release)
    CDefClassBase* m_p;
    CDefClassReference(CDefClassBase* p) : m_p(p) { if (m_p) ++m_p->m_refCount; }
    ~CDefClassReference() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    CDefClassBase* operator->() const { return m_p; }
};
class CTCParamsBase;
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                  // +0x20 interface-present bitset
    CInterfaceMap  m_interfaces;                      // +0x44
    unsigned char  _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                            // +0x70
    CDefPointer<const CHeroStatsDef> GetHeroStatsDef();   // 0x00663340
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    template <class T> __forceinline bool GetInterface(const T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (const T*)it->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
class CTCCarrying { public:                                  // interface 0x46
    CThing* GetThingInPrimarySlot() const;            // 0x006A1440
};
struct CHasteMultiplier {                             // std::pair<void* const, float>
    void* first;
    float second;
    CHasteMultiplier(void* key, float value) : first(key), second(value) {}
};
struct CHasteMap {                                    // map<void*, float>
    struct iterator { void* node; };
    struct pairib { iterator first; bool second; };
    pairib insert(const CHasteMultiplier& value);     // 0x00580D21
};
class CTCHaste { public:                                     // interface 0x8C
    unsigned char _pad00[0x18];
    CHasteMap Multipliers;                            // +0x18
    void RecalculateSpeed();                          // 0x00800020
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                         // 0x00A01B50
};
class CCreatureActionBase { public:                          // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    void Initialise();                                // 0x00694D10
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
class CCreatureAction_HeroLoadRangedWeapon : public CCreatureActionBase { public:
    CBaseIntelligentPointer PRangedWeapon;            // +0xa8
    CBaseIntelligentPointer PAmmo;                    // +0xb0
    void Initialise();
};

void CCreatureAction_HeroLoadRangedWeapon::Initialise()
{
    CCreatureActionBase::Initialise();
    const CTCHeroExperience* experience;
    if (((const CThing*)GetCreature())->GetInterface(&experience, 0x68)) {
        const CTCCarrying* carrying;
        if (((const CThing*)GetCreature())->GetInterface(&carrying, 0x46)) {
            CDefClassReference weaponDef(carrying->GetThingInPrimarySlot()->m_pDef);
            CDefPointer<const CHeroStatsDef> stats = GetCreature()->GetHeroStatsDef();
            CDefPointer<const CAbilityDef> ability;
            if (stats.Object && weaponDef->GetSubDef(&ability)) {
                float level = stats->Abilities.GetAbilityMultiplier(experience);
                float multiplier = (ability->MaxMultiplier - ability->MinMultiplier) * level + ability->MinMultiplier;
                CTCHaste* haste;
                if (!GetCreature()->GetInterface(&haste, 0x8c))
                    haste = (CTCHaste*)GetCreature()->AddTC("CTCHaste", false, 0);
                haste->Multipliers.insert(CHasteMultiplier(this, multiplier));
                haste->RecalculateSpeed();
            }
        }
    }
}