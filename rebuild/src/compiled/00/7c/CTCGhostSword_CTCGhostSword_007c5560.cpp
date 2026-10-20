// CTCGhostSword::CTCGhostSword(CThing& parent) -- constructor: CTCBase, a null def pointer, empty minion / effect / fade-timer vectors, no spell level or removed item, not cancelled; load SPECIAL_ABILITIES_GHOST_SWORD_DEF, convert its fade-in seconds to frames and take the spell level from the owner's ability inventory
// Leaf: ??0CTCGhostSword
//
// Retail 0x007C5560, 256 bytes incl. padding (`ret 4`); vtable 0x0126E494. Ego_r.pdb `??0CTCGhostSword@@QAE@AAVCThing@@@Z`.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800; CBaseIntelligentPointer ctor/SetPItem @ 0x00A01B10/0x00A01B90
// (CIntelligentPointer<CThing> vtable 0x01238C6C); CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0;
// CGameDefinitionManager::Get @ 0x0044C6B0 (__fastcall, twice); CDefinitionManager::GetGlobalIndexFromClassIndex @ 0x009AD390;
// CDefinitionManager::GetDef<CSpecialAbilitiesGhostSwordDef> @ 0x007C6600 (label "GetDef<CThingPhysicalSwitchDef>" is
// propagated); _ftol2 @ 0x00BFEA70; CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (interface 0x6F,
// word 3 bit 15: `test ch,ch ; jns`); CTCInventoryAbilities::GetAbilityLevel(EHeroAbility) @ 0x005EDE93 (ability 0xF).
// Global g_framesPerSecond (long) @ 0x01375550.
// Layout: Ego_r.pdb CTCGhostSword with retail 12-byte STLport vectors (PDB 16): PDef +0x0C, Minions +0x10, MinionEffects
// +0x1C, MinionAliveTimer +0x28 (not initialised), FadeInTimer +0x2C, CurFadeInTimer +0x30, SpellLevel +0x3C,
// PItemRemoved +0x40, CutSceneCancelled +0x48; the def's fade-in seconds at retail +0x4C.
// Shape: the landed CTCDrainLife ctor (0x007C30E0) family: init-list members after the vtable store (one ebx zero
// register), the def name a named CCharString in its own block homed in the dead `parent` slot like the lookup key; here the
// frame-count conversion precedes the ability lookup.
class CThing;
class CBaseObject;
class CTCBase;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class CSpecialAbilitiesGhostSwordDef {
public:
    virtual void v00();
    long RefCount;                                  // +0x04
    unsigned char _pad08[0x4c - 0x08];
    float FadeInSecs;                               // +0x4C
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
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;            // 0x007C6600 for the ghost-sword def
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
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

template <class T> struct SRetailAllocProxy {       // STLport _STLP_alloc_proxy
    T* _M_data;
    SRetailAllocProxy(T* data) : _M_data(data) {}
};
template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    SRetailAllocProxy<T> _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
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
};

extern long g_framesPerSecond;                      // 0x01375550

class CTCBase {
public:
    CTCBase(CThing* parent);                        // 0x00686800
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x4];                      // ToBeRemoved, IsInGlobalUpdateTCs
};

class CThingAICreature;
class CTCGhostSword : public CTCBase {              // vtable 0x0126E494
public:
    CDefPointer<const CSpecialAbilitiesGhostSwordDef> PDef;  // +0x0C
    CRetailVector< CIntelligentPointer<CThingAICreature> > Minions;   // +0x10
    CRetailVector< CIntelligentPointer<CThing> > MinionEffects;       // +0x1C
    long MinionAliveTimer;                          // +0x28
    long FadeInTimer;                               // +0x2C
    CRetailVector<long> CurFadeInTimer;             // +0x30
    long SpellLevel;                                // +0x3C
    CIntelligentPointer<CThing> PItemRemoved;       // +0x40
    bool CutSceneCancelled;                         // +0x48
    CTCGhostSword(CThing& parent);
    virtual void v00();
};

CTCGhostSword::CTCGhostSword(CThing& parent)
    : CTCBase(&parent), FadeInTimer(0), SpellLevel(0), PItemRemoved(0), CutSceneCancelled(false)
{
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_GHOST_SWORD_DEF", -1);
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &PDef);
    FadeInTimer = (long)(g_framesPerSecond * PDef->FadeInSecs);
    CTCInventoryAbilities* abilities;
    if (ParentThing->GetInterface(&abilities, 0x6f))
        SpellLevel = abilities->GetAbilityLevel(0xf);
}