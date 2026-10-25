// CThingSearchTools::GetAllThings(const CFilter_Bind3<OverlappingArea, HasNavigationInformation, IsInQuestOrNullQuest>& filter, list<const CThingPhysical*>& result) -- appends every scan-visible physical thing whose ceil-rounded bounding circle overlaps the area, that has navigation information and is in the quest (or no quest)
// Grade: DIFFER(440v423) -- retail_asm candidate: retail rounds the bounding radius with an inline-assembly `fistp` ceil (hence its ebp frame and the 0.5f/1.0f constants homed in frame locals); this C++ reference body uses _ftol and no frame pointer (graded once)
// Leaf: GetAllThings
// Retail: Fable.exe (TLC, VC7.1) @ 0x00516200, 423 bytes (+ padding to 432), `ret 8`, ebp frame; one caller 0x00509A98.
// Callees: PeekAllThings 0x0049C770, CTCPhysicsBase::GetBoundingInfo(C3DVector*, float*) 0x006AF640, _ftol 0x00BFEA70 (six
//   conversions), CTCPhysicsBase virtual +0xa8, CThing virtual +0xdc (both arms of the navigation test), CThingFilter_
//   IsInQuestOrNullQuest::operator() 0x006A7C60, malloc 0x00BFEA0E (list node).
// Shape: the landed GetAllThings 0x004ACD70 (list result) with a by-reference Bind3; the OverlappingArea filter as in
//   0x00516830 (the inline-asm ceil, the area copy grown by the radius, the centre tested inside); the navigation test
//   calls the same thing slot on both arms of the physics-obstacle test.
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
    unsigned char _pad_0x44[0x60 - 0x44];
    class CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned char _pad_0x64[0x6c - 0x64];
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

struct CThingPhysical : CThing { enum { ThingTypeMask = 3 }; };

// ---- the overlapping-area filter (inline in retail) ----------------------------------------------------------------
struct C3DVector { float X, Y, Z; };
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    C2DBoxI() {}
    C2DBoxI(const C2DBoxI& o) : X0(o.X0), Y0(o.Y0), X1(o.X1), Y1(o.Y1) {}
};
#define VSLOTS10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
class CTCPhysicsBase {
public:
    VSLOTS10(0) VSLOTS10(1) VSLOTS10(2) VSLOTS10(3)
    virtual void v40(); virtual void v41();
    virtual bool IsNavigationObstacle() const;        // slot 42 (+0xa8)
    void GetBoundingInfo(C3DVector* centre, float* radius) const;   // 0x006AF640
};
// Retail rounds the bounding radius up with the engine's INLINE-ASSEMBLY ceil: `fld x ; fadd half ; fistp i ; fild i ;
// fsub one ; fstp t` then `if (bits(x) == bits(t)) --i` and `fild i`, over the constants 0.5f / 1.0f homed in frame
// locals (that asm is also why the function has an ebp frame). VC7.1 cannot emit the bare `fistp` from C++; this is the
// C++ reference of the same arithmetic (round-to-nearest of x + 0.5, one less when that lands exactly one above x).
__forceinline float CeilRadius(float x)
{
    const float half = 0.5f;
    const float one = 1.0f;
    long i = (long)(x + half);
    float t = (float)i - one;
    if (*(const long*)&x == *(const long*)&t)
        --i;
    return (float)i;
}
class CThingFilter_OverlappingArea {
public:
    const C2DBoxI* Area;                              // +0x00
    __forceinline bool operator()(const CThing& thing) const
    {
        const CTCPhysicsBase* physics = thing.PhysicsTC;
        if (!physics)
            return false;
        C3DVector centre;
        float radius;
        physics->GetBoundingInfo(&centre, &radius);
        radius = CeilRadius(radius);
        C2DBoxI area(*Area);
        long maxX = (long)((float)area.X1 + radius);
        long minX = (long)((float)area.X0 - radius);
        long minY = (long)((float)area.Y0 - radius);
        long maxY = (long)((float)area.Y1 + radius);
        long x = (long)centre.X;
        long y = (long)centre.Y;
        return x >= minX && x < maxX && y >= minY && y < maxY;
    }
};
// The thing's navigation query (virtual slot 55, +0xdc) reached through two inline paths (physics obstacle or not)
// that both end in the same virtual call, as in retail.
struct CThingVirtuals {
    VSLOTS10(0) VSLOTS10(1) VSLOTS10(2) VSLOTS10(3) VSLOTS10(4)
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual bool HasNavigationInformation() const;    // slot 55 (+0xdc)
};
class CThingFilter_HasNavigationInformation {
public:
    __forceinline bool operator()(const CThing& thing) const
    {
        const CTCPhysicsBase* physics = thing.PhysicsTC;
        const CThingVirtuals& v = *(const CThingVirtuals*)&thing;
        if (physics && physics->IsNavigationObstacle())
            return v.HasNavigationInformation();
        return v.HasNavigationInformation();
    }
};
struct CThingFilter_IsInQuestOrNullQuest {
    const void* QuestSectionName;
    bool operator()(const CThing& thing) const;       // 0x006A7C60
};
template <class A, class B, class C> struct CFilter_Bind3 {   // the three filters, held by reference
    const A& First;
    const B& Second;
    const C& Third;
    template <class T> __forceinline bool operator()(const T& thing) const { return First(thing) && Second(thing) && Third(thing); }
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
            result.push_back((const TThing*)thing);
    }
}

typedef CFilter_Bind3<CThingFilter_OverlappingArea, CThingFilter_HasNavigationInformation, CThingFilter_IsInQuestOrNullQuest> CNavFilter;
template void CThingSearchToolsMethods::GetAllThings<CThingPhysical, CResultList<const CThingPhysical*>, CNavFilter>(const CNavFilter&,
    CResultList<const CThingPhysical*>&);
