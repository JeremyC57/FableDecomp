// CTCHeroExperience::CTCHeroExperience(CThing* parent) -- hero-experience component ctor: fishing level 1, zero experience, the three per-stat vectors (3, 10 and 10 zeroed longs), the pickup and attack-history lists, able to gain experience, and the hero-stats def taken from the definition manager
// Leaf: ??0CTCHeroExperience
//
// Retail 0x006D7170, 344 bytes (+8 int3 padding, `ret 4`). Vtable 0x0125F6C4 (the landed dtor 0x006DB7E0 names the class).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, malloc @ 0x00BFEA0E (STLport __malloc_alloc: the three vector
// buffers and the two list header nodes; one deferred `add esp,14h`), CTCBase::GetDefinitionManager @ 0x006869D0, the def's
// virtual slot 1 (destroy at zero count).
// Shape: members initialised in declaration order through the STLport bases: `_Vector_base(n, alloc)` (pointers nulled, the
// buffer malloc'd, end of storage set) + STLport's scalar uninitialized_fill_n (plain fill_n stores, no placement new: its null
// test would add a loop) unrolled for the constant counts (zero register edx/ecx);
// `_List_base(alloc)` (header pointer nulled, the 0x10-byte node malloc'd and self-linked in a local, then stored). The
// hero-stats def comes back by value from an inline getter (temporary copy with AddRef), is assigned through
// CDefPointer::operator= (identity test, release + clear, AddRef) and the temporary is released with the short `dec [ebx+4]` form.
// Layout: retail STLport vector 12 bytes and list 4 bytes (PDB 16/12), as in the landed dtor 0x006DB7E0 (the generated
// engine/CTCHeroExperience.h keeps the PDB offsets, so a local model is used): FishingLevel +0x0c, TotalExperiencePoints +0x10,
// ExperiencePointsAvailableToSpend +0x14, StatExperiencePoints +0x18, TrainableStatLevels +0x24, ExperienceMultiplierParam +0x30,
// LastExperienceMultiplierParam +0x34, ExperienceAttenuationTimer +0x38, AbleToGainExperience +0x3c, Pickups +0x40,
// ExperienceSpentOnStats +0x44, AttackHistory +0x50, PDef +0x54, PHeroCombatDef +0x58, PHeroStatsDef +0x5c; the definition
// manager's hero-stats def pointer at +0xdc.
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);       // 0x00BFEA0E

struct CThing;

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long m_count;                                   // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->m_count; }
    ~CDefPointer() { if (Object) { if (--Object->m_count == 0) Object->Destroy(); } }
    CDefPointer& operator=(const CDefPointer& o)
    {
        if (Object != o.Object) {
            T* object = Object;
            if (object) {
                if (--object->m_count == 0)
                    object->Destroy();
                Object = 0;
            }
            Object = o.Object;
            if (Object)
                ++Object->m_count;
        }
        return *this;
    }
};
struct CHeroExperienceDef : CDefBase { };
struct CHeroCombatDef : CDefBase { };
struct CHeroStatsDef : CDefBase { };

struct CDefinitionManager {
    unsigned char _pad00[0xdc];
    CDefPointer<CHeroStatsDef> HeroStatsDef;        // +0xdc
    CDefPointer<CHeroStatsDef> GetHeroStatsDef() const { return HeroStatsDef; }
};

template <class T> __forceinline T* UninitializedFillN(T* first, unsigned long count, const T& value)
{                                                   // STLport __uninitialized_fill_n(.., __true_type): fill_n, plain stores
    for (; count > 0; --count, ++first)
        *first = value;
    return first;
}
template <class T> struct CAllocator {              // STLport allocator<T>: empty, __malloc_alloc underneath
    CAllocator() { }
    T* allocate(unsigned long count) { return count != 0 ? static_cast<T*>(malloc(count * sizeof(T))) : 0; }
};
template <class T> struct CAllocProxy : CAllocator<T> {     // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {             // STLport _Vector_base<T, allocator<T> >
    T*             m_pStart;                        // +0
    T*             m_pFinish;                       // +4
    CAllocProxy<T> m_endOfStorage;                  // +8
    CVectorBase(unsigned long count, const CAllocator<T>& alloc)
        : m_pStart(0), m_pFinish(0), m_endOfStorage(alloc, 0)
    {
        m_pStart = m_endOfStorage.allocate(count);
        m_pFinish = m_pStart;
        m_endOfStorage.m_data = m_pStart + count;
    }
};
template <class T> struct CRetailVector : CVectorBase<T> {  // STLport vector<T> (12 bytes)
    explicit CRetailVector(unsigned long count)
        : CVectorBase<T>(count, CAllocator<T>())
    {
        this->m_pFinish = UninitializedFillN(this->m_pStart, count, T());
    }
};

struct CListNodeBase {
    CListNodeBase* _M_next;
    CListNodeBase* _M_prev;
};
template <class T> struct CListNode : CListNodeBase { T _M_data; };
template <class T> struct CRetailList {             // STLport list<T>: the header-node pointer only (4 bytes)
    CAllocProxy<CListNode<T> > _M_node;
    CRetailList() : _M_node(CAllocator<CListNode<T> >(), 0)
    {
        CListNode<T>* n = _M_node.allocate(1);
        n->_M_next = n;
        n->_M_prev = n;
        _M_node.m_data = n;
    }
};
struct SPickup { long Type; long Amount; };             // pair<long,long>
struct SAttack { long Attribute; long Count; };         // pair<EDamageAttribute,long>

struct CTCBase {
    CTCBase(CThing* parent);                        // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0
};

struct CTCHeroExperience : CTCBase {                // vtable 0x0125F6C4
    long                 FishingLevel;              // +0x0C
    long                 TotalExperiencePoints;     // +0x10
    long                 ExperiencePointsAvailableToSpend;  // +0x14
    CRetailVector<long>  StatExperiencePoints;      // +0x18
    CRetailVector<long>  TrainableStatLevels;       // +0x24
    float                ExperienceMultiplierParam; // +0x30
    float                LastExperienceMultiplierParam;     // +0x34
    long                 ExperienceAttenuationTimer;        // +0x38
    bool                 AbleToGainExperience;      // +0x3C
    CRetailList<SPickup> Pickups;                   // +0x40
    CRetailVector<long>  ExperienceSpentOnStats;    // +0x44
    CRetailList<SAttack> AttackHistory;             // +0x50
    CDefPointer<CHeroExperienceDef> PDef;           // +0x54
    CDefPointer<CHeroCombatDef>     PHeroCombatDef; // +0x58
    CDefPointer<CHeroStatsDef>      PHeroStatsDef;  // +0x5C
    CTCHeroExperience(CThing* parent);
    virtual ~CTCHeroExperience();
};

CTCHeroExperience::CTCHeroExperience(CThing* parent)
    : CTCBase(parent),
      FishingLevel(1),
      TotalExperiencePoints(0),
      ExperiencePointsAvailableToSpend(0),
      StatExperiencePoints(3),
      TrainableStatLevels(10),
      ExperienceMultiplierParam(0.0f),
      LastExperienceMultiplierParam(0.0f),
      ExperienceAttenuationTimer(0),
      AbleToGainExperience(true),
      ExperienceSpentOnStats(10)
{
    PHeroStatsDef = GetDefinitionManager()->GetHeroStatsDef();
}
