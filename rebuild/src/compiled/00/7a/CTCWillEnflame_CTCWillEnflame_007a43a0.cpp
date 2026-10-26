// CTCWillEnflame::CTCWillEnflame(CThing* parent) -- constructor: CTCBase, cleared flags, null def pointer and spell level, null removed-item and target pointers; arm the owner's spell-cast state component, load SPECIAL_ABILITIES_ENFLAME_DEF and take the spell level from the owner's ability inventory
// Leaf: ??0CTCWillEnflame
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A43A0, 281 bytes (+7 int3), `ret 4`; vtable 0x0126C824.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800; CBaseIntelligentPointer ctor/SetPItem @ 0x00A01B10/0x00A01B90 (x2,
//   CIntelligentPointer<CThing> vtable 0x01238C6C); CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (non-const,
//   interface 0xA7 = word 5 bit 7: `test cl,cl ; jns`) and the const copy @ 0x004365B0 (interface 0x6F, word 3 bit 15);
//   CCharString(const char*, long) @ 0x0099EBF0 ("SPECIAL_ABILITIES_ENFLAME_DEF" @ 0x01235C1C), ~CCharString @ 0x0099EAE0;
//   CGameDefinitionManager::Get @ 0x0044C6B0 (__fastcall, twice); CDefinitionManager::GetGlobalIndexFromClassIndex
//   (const CCharString&, long) const @ 0x009AD390; CDefinitionManager::GetDef<CSpecialAbilitiesEnflameDef> @ 0x007A5080;
//   CTCInventoryAbilities::GetAbilityLevel(EHeroAbility) @ 0x005EDE93 (ability 3).
// Shape: the landed CTCForcePushPower ctor 0x007A54A0 twin: init-list members through the ebx zero (the def pointer through
//   its address in ebp, reused for GetDef), the def name a named CCharString in the dead `parent` slot; the spell-cast
//   state lookup goes through the non-const GetTC (component in eax, two stores), the ability lookup through a const
//   parent (const LowerBound, component straight into ecx).
// Layout: Ego_r.pdb CTCWillEnflame (0x2C) matches retail: EnflameActionInitialised +0x0C, Released +0x0D, PDef +0x10,
//   SpellLevel +0x14, PItemRemoved +0x18, IsZTargetting +0x20, PTarget +0x21 (unaligned).
class CThing;
class CBaseObject;
class CTCBase;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class CSpecialAbilitiesEnflameDef {
public:
    virtual void v00();
    long RefCount;                                  // +0x04
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
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;            // 0x007A5080 for the enflame def
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
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
};

class CTCInventoryAbilities {
public:
    long GetAbilityLevel(long ability);             // 0x005EDE93 (EHeroAbility)
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
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
};

struct CTCSpellCastState {                          // interface type 0xA7 (class not pinned)
    unsigned char _pad00[0x0c];
    bool Active;                                    // +0x0c
    unsigned char _pad0d[3];
    long Mode;                                      // +0x10
};

class CTCBase {
public:
    CTCBase(CThing* parent);                        // 0x00686800
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x4];                      // ToBeRemoved, IsInGlobalUpdateTCs
};

#pragma pack(push, 1)
class CTCWillEnflame : public CTCBase {             // vtable 0x0126C824
public:
    bool EnflameActionInitialised;                  // +0x0C
    bool Released;                                  // +0x0D
    unsigned char _pad0e[2];
    CDefPointer<const CSpecialAbilitiesEnflameDef> PDef;   // +0x10
    long SpellLevel;                                // +0x14
    CIntelligentPointer<CThing> PItemRemoved;       // +0x18
    bool IsZTargetting;                             // +0x20
    CIntelligentPointer<CThing> PTarget;            // +0x21 (unaligned, as the PDB)
    CTCWillEnflame(CThing* parent);
    virtual void v00();
};
#pragma pack(pop)

CTCWillEnflame::CTCWillEnflame(CThing* parent)
    : CTCBase(parent), EnflameActionInitialised(false), Released(false), SpellLevel(0), PItemRemoved(0),
      IsZTargetting(false), PTarget(0)
{
    if (ParentThing->HasTC(0xa7)) {
        CTCSpellCastState* state = (CTCSpellCastState*)ParentThing->GetTC(0xa7);
        state->Active = true;
        state->Mode = 3;
    }
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_ENFLAME_DEF", -1);
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &PDef);
    const CThing* parentThing = ParentThing;
    if (parentThing->HasTC(0x6f))
        SpellLevel = ((CTCInventoryAbilities*)parentThing->GetTC(0x6f))->GetAbilityLevel(3);
}