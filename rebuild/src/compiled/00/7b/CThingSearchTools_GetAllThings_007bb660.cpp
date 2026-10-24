// CThingSearchTools::GetAllThings(const TFilter& filter, TContainer& result) [TThing = CThingAICreature, TContainer = std::list<CIntelligentPointer<CThingAICreature> >, TFilter = CThingFilter_HasBuildingAsHomeOrWork] -- append an intelligent pointer to every scan-visible AI creature whose work or home building is the filter's building
// Leaf: GetAllThings
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BB660, 179 bytes (`ret 8`).
// Callees: PeekAllThings 0x0049C770, CBaseIntelligentPointer::GetPItem 0x00A01B50 (inlined filter: work building +0x170 first, then home +0x168),
// list<CIntelligentPointer<CThingAICreature> >::push_back 0x006604EB, CIntelligentPointer<CThingAICreature> vtable 0x0123E7E0.
// Model: CThingSearchTools from engine/CThingSearchTools.h (`this` is only passed through to the list getter);
// CThing, the thing lists, the result container and the filters are local retail-layout models.
// Shape: scan test, then the type-mask test (mask copied into a local), then the filter on the thing reference.
// A vector push takes a by-value ThingCast<T> temporary (homed in the dead argument slot and shared with the
// __true_type tag: the same-TU noinline overflow stand-in shows VC7.1 the tag is never read); a list push
// inserts before end() reading end()->prev once; an intelligent-pointer push builds a named temporary.
#include "engine/CThingSearchTools.h"
#include <stdlib.h>

inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() {} };

// ---- CThing (retail layout; Ego_r.pdb offsets in brackets) -------------------------------------
// Retail CThing sits 0x10 below the PDB from ThingTypeFlags on: UsingComponents +0x20 [PDB +0x20],
// ThingTypeFlags +0x6c [PDB +0x7c], the state bit byte +0x91 [PDB +0xa1] = Dead 0x01,
// IncludeInThingScans 0x04, Activated 0x08, InLimbo 0x20.
struct CThing {
    enum { ThingTypeMask = 0 };                       // every thing matches the CThing type mask
    unsigned char _pad_0x00[0x20];
    unsigned char UsingComponents[0x24];              // +0x20 bitset<274> of present thing components
    unsigned char _pad_0x44[0x6c - 0x44];
    unsigned long ThingTypeFlags;                     // +0x6c THING_TYPE_* inheritance bits
    unsigned char _pad_0x70[0x91 - 0x70];
    unsigned char StateBits;                          // +0x91 Dead / IncludeInThingScans / Activated / InLimbo

    // CThing::IsIncludedInThingScans (out of line at 0x0048BC40), inlined here.
    bool IsIncludedInThingScans() const
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
};

// The type test: the mask is copied into a local first, so the CThing (mask 0) test folds late and
// keeps retail's `xor dl,dl ; jne`, and a byte-sized mask compares through the low byte.
template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}

// The thing as the searched type, returned by value (a fresh temporary for push_back's const T&).
template <class T> inline T* ThingCast(CThing* thing) { return (T*)thing; }

// The thing manager's lists: std::list<CThing*> (STLport: the node pointer at +0, node = next, prev, value).
struct CThingList {
    struct SNode { SNode* m_pNext; SNode* m_pPrev; CThing* m_value; };
    SNode* m_pHead;
    struct iterator {
        SNode* m_pNode;
        iterator(SNode* node) : m_pNode(node) {}
        CThing* operator*() const { return m_pNode->m_value; }
        iterator& operator++() { m_pNode = m_pNode->m_pNext; return *this; }
        bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; }
    };
    iterator begin() { return iterator(m_pHead->m_pNext); }
    iterator end() { return iterator(m_pHead); }
};

// The result: STLport list<CIntelligentPointer<T> >, push_back out of line (0x006604EB); the pointer is built in a
// named stack temporary: CBaseIntelligentPointer() 0x00A01B10, SetPItem 0x00A01B90, ~CBaseIntelligentPointer 0x00A01C10.
struct CBaseObject;
struct CBaseObjectPointer { CBaseObject* Object; unsigned long RefCount; };
struct CBaseIntelligentPointer {
    CBaseObjectPointer* PData;
    CBaseIntelligentPointer();                        // 0x00A01B10
    virtual ~CBaseIntelligentPointer();               // 0x00A01C10
    void SetPItem(CBaseObject* item);                 // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x0123E7E0
    CIntelligentPointer(T* p) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)p); }
};
template <class T> struct CResultList {
    void push_back(const T& x);                       // 0x006604EB
};

struct CBaseObject;
// CThingAICreature: retail HomeBuilding +0x168, WorkBuilding +0x170 (Ego_r.pdb +0x190/+0x198), each a
// CIntelligentPointer<CThingBuilding> read through CBaseIntelligentPointer::GetPItem 0x00A01B50.
struct CBuildingPointer {
    void* _vfptr;
    void* PData;
    CBaseObject* GetPItem() const;                    // 0x00A01B50
};
struct CThingAICreature : CThing {
    enum { ThingTypeMask = 0x1f };
    unsigned char _pad_0x94[0x168 - sizeof(CThing)];
    CBuildingPointer HomeBuilding;                    // +0x168
    CBuildingPointer WorkBuilding;                    // +0x170
};
struct CThingFilter_HasBuildingAsHomeOrWork {
    CBaseObject* Building;
    bool operator()(const CThingAICreature& creature) const
    {
        return creature.WorkBuilding.GetPItem() == Building || creature.HomeBuilding.GetPItem() == Building;
    }
};

struct CThingSearchToolsMethods : CThingSearchTools {
    CThingList* PeekAllThings();                      // 0x0049C770: &ThingManager->(all-things list at +0x18)
    template <class TThing, class TContainer, class TFilter> void GetAllThings(const TFilter& filter, TContainer& result);
};

// Every scan-visible thing of the whole thing list that is of type TThing and passes the filter.
template <class TThing, class TContainer, class TFilter>
void CThingSearchToolsMethods::GetAllThings(const TFilter& filter, TContainer& result)
{
    CThingList* things = PeekAllThings();
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        CThing* thing = *it;
        if (thing->IsIncludedInThingScans() && IsThingOfType<TThing>(thing) && filter(*(TThing*)thing))
            { CIntelligentPointer<TThing> pointer((TThing*)thing); result.push_back(pointer); }
    }
}

template void CThingSearchToolsMethods::GetAllThings<CThingAICreature, CResultList<CIntelligentPointer<CThingAICreature> >, CThingFilter_HasBuildingAsHomeOrWork >(const CThingFilter_HasBuildingAsHomeOrWork&, CResultList<CIntelligentPointer<CThingAICreature> >&);