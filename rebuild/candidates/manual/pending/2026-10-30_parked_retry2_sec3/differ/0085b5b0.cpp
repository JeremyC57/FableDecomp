// CThingSearchTools::GetAllThings(const CFilter_Bind4<IsNot, CombatIsStrikeToStrike, CombatOverlappingSphere, CombatCanCollideOrInteractWithThing>& filter, list<CThingGameObject*>& result) -- appends every scan-visible game object that is not the attacker, is strike-to-strike with it, whose hit-location bounding sphere overlaps the strike sphere and that it can collide or interact with
// Grade: DIFFER(365v365) -- GetBoundingSphere this-load after the pushes + ecx/edx swaps; tried (this sweep): inline CThing GetTC returning the component (param key 368; local key = base), named component local (inert)
// Leaf: GetAllThings
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085B5B0, 365 bytes (+ int3 padding to 368), `ret 8`; one caller 0x00859392.
// Callees: PeekAllThings 0x0049C770, CThingFilter_IsNot::operator() 0x006A76A0, (anonymous)::CThingFilter_CombatIsStrikeToStrike
//   ::operator() 0x008565E0, CVectorMap::LowerBound 0x004365B0 (the inlined CTCHitLocations lookup, interface 0x7f),
//   CTCHitLocations::GetBoundingSphere(C3DVector*, float*, bool) 0x00815420, CThingFilter_CombatCanCollideOrInteractWithThing
//   ::operator() 0x00857250, malloc 0x00BFEA0E (the 12-byte list node).
// Shape: the landed GetAllThings 0x004ACD70 (list result) with a by-reference Bind4; the overlapping-sphere filter is
//   inline: the interface bit (word 3, bit 31: `test eax,eax ; jns`), the lookup through a named map reference (kept in
//   edi across the call, which pushes the node iterator to the stack as retail) with the type local homed
//   in the dead filter-argument slot, the bounding sphere into two locals, the squared distance accumulated z, y, x.
#include "engine/CThingSearchTools.h"
#include <stdlib.h>

inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() {} };

struct C3DVector { float X, Y, Z; };
class CTCHitLocations {
public:
    void GetBoundingSphere(C3DVector* centre, float* radius, bool useAll) const;   // 0x00815420
};
struct CInterfaceEntry { long Type; CTCHitLocations* Component; };
struct CInterfaceMap {
    CInterfaceEntry* First;
    CInterfaceEntry* Last;
    const CInterfaceEntry* LowerBound(const long* type) const;   // 0x004365B0
};

// ---- CThing (retail layout; Ego_r.pdb offsets in brackets) -------------------------------------
// Retail CThing sits 0x10 below the PDB from ThingTypeFlags on: UsingComponents +0x20 [PDB +0x20],
// ThingTypeFlags +0x6c [PDB +0x7c], the state bit byte +0x91 [PDB +0xa1] = Dead 0x01,
// IncludeInThingScans 0x04, Activated 0x08, InLimbo 0x20.
struct CThing {
    enum { ThingTypeMask = 0 };                       // every thing matches the CThing type mask
    unsigned char _pad_0x00[0x20];
    unsigned long UsingComponents[9];                 // +0x20 bitset<274> of present thing components
    CInterfaceMap Components;                         // +0x44 CVectorMap<ETCInterfaceType, CTCBase*>
    unsigned char _pad_0x4c[0x6c - 0x4c];
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

struct CThingPhysical : CThing { enum { ThingTypeMask = 3 }; };
struct CThingGameObject : CThingPhysical { enum { ThingTypeMask = 7 }; };

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

// The result: STLport list<T*>; push_back is insert(end(), x), the node comes from the CRT malloc 0x00BFEA0E.
template <class T> struct CResultList {
    struct SNode { SNode* _M_next; SNode* _M_prev; T _M_data; };
    struct iterator {
        SNode* _M_node;
        iterator(SNode* node) : _M_node(node) {}
        iterator(const iterator& o) : _M_node(o._M_node) {}
    };
    SNode* _M_node;
    iterator end() { return iterator(_M_node); }
    SNode* _M_create_node(const T& x)
    {
        SNode* p = (SNode*)malloc(sizeof(SNode));
        new (&p->_M_data) T(x);
        return p;
    }
    iterator insert(iterator position, const T& x)
    {
        SNode* tmp = _M_create_node(x);
        SNode* n = position._M_node;
        SNode* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return iterator(tmp);
    }
    void push_back(const T& x) { insert(end(), x); }
};

struct CThingFilter_IsNot {
    CThing* Thing;
    bool operator()(const CThing& thing) const;       // 0x006A76A0
};
namespace {
struct CThingFilter_CombatIsStrikeToStrike {
    const CThing* Attacker;
    bool operator()(const CThing& thing) const;       // 0x008565E0
};
struct CThingFilter_CombatOverlappingSphere {
    C3DVector Centre;                                 // +0x00
    float     Radius;                                 // +0x0c
    __forceinline bool operator()(const CThing& thing) const
    {
        if (!(thing.UsingComponents[3] & (1UL << 31)))   // interface 0x7f (CTCHitLocations) present
            return false;
        const long type = 0x7f;
        const CInterfaceMap& map = thing.Components;
        const CInterfaceEntry* entry = map.LowerBound(&type);
        if (entry == map.Last || type < entry->Type)
            entry = map.Last;
        C3DVector centre;
        float radius;
        entry->Component->GetBoundingSphere(&centre, &radius, true);
        float dx = centre.X - Centre.X;
        float dy = centre.Y - Centre.Y;
        float dz = centre.Z - Centre.Z;
        float r = radius + Radius;
        float l = dz * dz;
        l += dy * dy;
        l += dx * dx;
        return l < r * r;
    }
};
}
struct CThingFilter_CombatCanCollideOrInteractWithThing {
    const CThing* Attacker;
    bool operator()(const CThing& thing) const;       // 0x00857250
};
template <class A, class B, class C, class D> struct CFilter_Bind4 {   // the four filters, held by reference
    const A& First;
    const B& Second;
    const C& Third;
    const D& Fourth;
    template <class T> __forceinline bool operator()(const T& thing) const
    { return First(thing) && Second(thing) && Third(thing) && Fourth(thing); }
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
            result.push_back((TThing*)thing);
    }
}

typedef CFilter_Bind4<CThingFilter_IsNot, CThingFilter_CombatIsStrikeToStrike, CThingFilter_CombatOverlappingSphere,
    CThingFilter_CombatCanCollideOrInteractWithThing> CStrikeFilter;
template void CThingSearchToolsMethods::GetAllThings<CThingGameObject, CResultList<CThingGameObject*>, CStrikeFilter>(const CStrikeFilter&,
    CResultList<CThingGameObject*>&);
