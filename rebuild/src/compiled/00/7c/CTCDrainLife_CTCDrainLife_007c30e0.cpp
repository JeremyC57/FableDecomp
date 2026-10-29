// CTCDrainLife::CTCDrainLife(CThing& parent) -- constructor: CTCBase, a null def pointer, spell level and removed-item pointer, an empty shot vector, JustReleased set; load SPECIAL_ABILITIES_DRAIN_LIFE_DEF, take the spell level from the owner's ability inventory and convert SpellDelaySecs to frames
// Leaf: ??0CTCDrainLife
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C30E0, length 219 bytes (+ int3 padding). `ret 4`; vtable 0x0126E284.
//         Ego_r.pdb: `??0CTCDrainLife@@QAE@AAVCThing@@@Z`.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800; CBaseIntelligentPointer ctor/SetPItem @ 0x00A01B10/0x00A01B90
//          (CIntelligentPointer<CThing> vtable 0x01238C6C); CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @
//          0x0099EAE0; CGameDefinitionManager::Get @ 0x0044C6B0 (__fastcall, twice);
//          CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString&, long) const @ 0x009AD390;
//          CDefinitionManager::GetDef<CSpecialAbilitiesDrainLifeDef>(long, CDefPointer<const ..>*) const @ 0x007C3A60;
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (interface 0x6F, word 3 bit 15: `test ch,ch ; jns`);
//          CTCInventoryAbilities::GetAbilityLevel(EHeroAbility) @ 0x005EDE93 (ability 6); _ftol2 @ 0x00BFEA70.
//          Global g_framesPerSecond (long) @ 0x01375550.
// Shape: init-list members after the vtable store (one ebp zero register); the def name is a named CCharString in its own
//        block, homed in the dead `parent` argument slot like the interface lookup key that follows.
// Layout: Ego_r.pdb CTCDrainLife matches retail (PDef +0x0C, SpellLevel +0x10, PItemRemoved +0x14, Shots +0x1C (12-byte
//         STLport vector), JustReleased +0x28, SpellDelayFrames +0x2C); retail CSpecialAbilitiesDrainLifeDef::SpellDelaySecs
//         +0x6C (PDB +0x80: the def base is 4 bytes smaller and the four vectors 12 bytes each).
class CThing;
class CBaseObject;
class CTCBase;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class CSpecialAbilitiesDrainLifeDef {
public:
    virtual void v00();
    long RefCount;                                  // +0x04
    unsigned char _pad08[0x6c - 0x08];
    float SpellDelaySecs;                           // +0x6C
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
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;            // 0x007C3A60 for the drain-life def
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

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;     // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
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

class CTCDrainLife : public CTCBase {               // vtable 0x0126E284
public:
    CDefPointer<const CSpecialAbilitiesDrainLifeDef> PDef;   // +0x0C
    long SpellLevel;                                // +0x10
    CIntelligentPointer<CThing> PItemRemoved;       // +0x14
    CRetailVector< CIntelligentPointer<CThing> > Shots;      // +0x1C
    bool JustReleased;                              // +0x28
    long SpellDelayFrames;                          // +0x2C
    CTCDrainLife(CThing& parent);
    virtual void v00();
};

CTCDrainLife::CTCDrainLife(CThing& parent)
    : CTCBase(&parent), SpellLevel(0), PItemRemoved(0), JustReleased(true)
{
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_DRAIN_LIFE_DEF", -1);
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &PDef);
    CTCInventoryAbilities* abilities;
    if (ParentThing->GetInterface(&abilities, 0x6f))
        SpellLevel = abilities->GetAbilityLevel(6);
    SpellDelayFrames = (long)(g_framesPerSecond * PDef->SpellDelaySecs);
}