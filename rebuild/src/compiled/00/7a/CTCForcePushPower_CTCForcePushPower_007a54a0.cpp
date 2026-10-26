// CTCForcePushPower::CTCForcePushPower(CThing& parent) -- constructor: CTCBase, a null def pointer, spell level and removed-item pointer, cleared cast state; load SPECIAL_ABILITIES_FORCE_PUSH_DEF, take the spell level from the owner's ability inventory and convert the level's cast delay to frames
// Leaf: ??0CTCForcePushPower
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A54A0, length 222 bytes. `ret 4`; vtable 0x0126C9BC.
//         Ego_r.pdb: `??0CTCForcePushPower@@QAE@AAVCThing@@@Z`.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800; CBaseIntelligentPointer ctor/SetPItem @ 0x00A01B10/0x00A01B90
//          (CIntelligentPointer<CThing> vtable 0x01238C6C); CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @
//          0x0099EAE0; CGameDefinitionManager::Get @ 0x0044C6B0 (__fastcall, twice);
//          CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString&, long) const @ 0x009AD390;
//          CDefinitionManager::GetDef<CSpecialAbilitiesForcePushDef>(long, CDefPointer<const ..>*) const @ 0x00798C10;
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (interface 0x6F, word 3 bit 15: `test ch,ch ; jns`);
//          CTCInventoryAbilities::GetAbilityLevel(EHeroAbility) @ 0x005EDE93 (ability 1); _ftol2 @ 0x00BFEA70.
//          Global g_framesPerSecond (long) @ 0x01375550.
// Shape: the CTCDrainLife constructor's twin (0x007C30E0): init-list members after the vtable store sharing the ebx zero,
//        the def name a named CCharString homed in the dead `parent` slot; here the ability inventory is the HasTC + GetTC
//        entry select (component straight into ecx) and the delay reads TimeDelayBetweenCastsSec[SpellLevel].
// Layout: Ego_r.pdb CTCForcePushPower matches retail (PDef +0x0C, SpellLevel +0x10, PItemRemoved +0x14, Casted +0x1C,
//         CurTimeDelayBetweenCastsFrames +0x20, IgnoreFriendlies +0x24); retail CSpecialAbilitiesForcePushDef::
//         TimeDelayBetweenCastsSec +0x98 (PDB +0xA4: 4-byte smaller def base, 12-byte vectors).
class CThing;
class CBaseObject;
class CTCBase;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
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

class CSpecialAbilitiesForcePushDef {
public:
    virtual void v00();
    long RefCount;                                  // +0x04
    unsigned char _pad08[0x98 - 0x08];
    CRetailVector<float> TimeDelayBetweenCastsSec;  // +0x98
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
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;            // 0x00798C10 for the force-push def
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
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
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

class CTCForcePushPower : public CTCBase {          // vtable 0x0126C9BC
public:
    CDefPointer<const CSpecialAbilitiesForcePushDef> PDef;   // +0x0C
    long SpellLevel;                                // +0x10
    CIntelligentPointer<CThing> PItemRemoved;       // +0x14
    bool Casted;                                    // +0x1C
    long CurTimeDelayBetweenCastsFrames;            // +0x20
    bool IgnoreFriendlies;                          // +0x24
    CTCForcePushPower(CThing& parent);
    virtual void v00();
};

CTCForcePushPower::CTCForcePushPower(CThing& parent)
    : CTCBase(&parent), SpellLevel(0), PItemRemoved(0), Casted(false), CurTimeDelayBetweenCastsFrames(0),
      IgnoreFriendlies(false)
{
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_FORCE_PUSH_DEF", -1);
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &PDef);
    if (ParentThing->HasTC(0x6f))
        SpellLevel = ((CTCInventoryAbilities*)ParentThing->GetTC(0x6f))->GetAbilityLevel(1);
    CurTimeDelayBetweenCastsFrames = (long)(g_framesPerSecond * PDef->TimeDelayBetweenCastsSec._M_start[SpellLevel]);
}