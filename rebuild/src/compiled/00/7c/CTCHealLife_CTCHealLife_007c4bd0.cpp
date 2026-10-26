// CTCHealLife::CTCHealLife(CThing& parent) -- constructor: CTCBase, a null def pointer, zero health accumulator / sound / spell level, an empty removed-item pointer, an empty heal list and two empty vectors, no healing target; load SPECIAL_ABILITIES_HEAL_LIFE_DEF, take the spell level from the owner's ability inventory, then start the healing effect
// Leaf: ??0CTCHealLife
//
// Retail 0x007C4BD0, 264 bytes (`ret 4`); vtable 0x0126E36C. Ego_r.pdb `??0CTCHealLife@@QAE@AAVCThing@@@Z`.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800; CBaseIntelligentPointer ctor/SetPItem @ 0x00A01B10/0x00A01B90 (twice,
// CIntelligentPointer<CThing> vtable 0x01238C6C); malloc @ 0x00BFEA0E (the list's 0x10-byte sentinel node);
// CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0; CGameDefinitionManager::Get @ 0x0044C6B0
// (__fastcall, twice); CDefinitionManager::GetGlobalIndexFromClassIndex @ 0x009AD390;
// CDefinitionManager::GetDef<CSpecialAbilitiesHealLifeDef> @ 0x007C4F80; CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
// const @ 0x004365B0 (interface 0x6F, `test ch,ch ; jns`); CTCInventoryAbilities::GetAbilityLevel @ 0x005EDE93 (ability
// 0xE); CTCHealLife::StartHealingEffect @ 0x007C4430.
// Layout: Ego_r.pdb CTCHealLife with retail STLport containers (list 4 bytes, vectors 12): PDef +0x0C, TotalHealthAccum
// +0x10, LoopingSound +0x14, SpellLevel +0x18, PItemRemoved +0x1C, ThingsToHeal +0x24, HealingEffects +0x28,
// HealthIncreaseAccum +0x34, PHealingTarget +0x40.
// Shape: the landed CTCDrainLife ctor (0x007C30E0) family; the list is STLport (`_M_node` alloc proxy zeroed, then the
// malloc'd sentinel linked to itself), the vectors `_Vector_base` + `_STLP_alloc_proxy`.
class CThing;
class CBaseObject;
class CTCBase;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class CSpecialAbilitiesHealLifeDef {
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
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;            // 0x007C4F80 for the heal-life def
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

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
template <class T> struct CAllocator {              // STLport allocator<T> (empty)
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> {   // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {             // STLport _Vector_base<T, allocator<T> >
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
template <class T> struct CRetailVector : CVectorBase<T> {   // STLport vector<T> (12 bytes)
    CRetailVector() : CVectorBase<T>(CAllocator<T>()) { }
};
template <class T> struct SListNode {               // _List_node<T>
    SListNode* _M_next;
    SListNode* _M_prev;
    T _M_data;
};
template <class T> struct CRetailList {             // STLport list (retail: the node pointer only)
    CAllocProxy< SListNode<T> > _M_node;
    CRetailList() : _M_node(CAllocator< SListNode<T> >(), 0)
    {
        SListNode<T>* node = (SListNode<T>*)malloc(sizeof(SListNode<T>));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node.m_data = node;
    }
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

class CTCBase {
public:
    CTCBase(CThing* parent);                        // 0x00686800
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x4];                      // ToBeRemoved, IsInGlobalUpdateTCs
};

class CThingCreatureBase;
class CTCHealLife : public CTCBase {                // vtable 0x0126E36C
public:
    CDefPointer<const CSpecialAbilitiesHealLifeDef> PDef;    // +0x0C
    float TotalHealthAccum;                         // +0x10
    unsigned long LoopingSound;                     // +0x14
    long SpellLevel;                                // +0x18
    CIntelligentPointer<CThing> PItemRemoved;       // +0x1C
    CRetailList< CIntelligentPointer<CThingCreatureBase> > ThingsToHeal;   // +0x24
    CRetailVector< CIntelligentPointer<CThing> > HealingEffects;           // +0x28
    CRetailVector<float> HealthIncreaseAccum;       // +0x34
    CIntelligentPointer<CThing> PHealingTarget;     // +0x40
    CTCHealLife(CThing& parent);
    void StartHealingEffect();                      // 0x007C4430
    virtual void v00();
};

CTCHealLife::CTCHealLife(CThing& parent)
    : CTCBase(&parent), TotalHealthAccum(0), LoopingSound(0), SpellLevel(0), PItemRemoved(0), PHealingTarget(0)
{
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_HEAL_LIFE_DEF", -1);
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &PDef);
    CTCInventoryAbilities* abilities;
    if (ParentThing->GetInterface(&abilities, 0x6f))
        SpellLevel = abilities->GetAbilityLevel(0xe);
    StartHealingEffect();
}