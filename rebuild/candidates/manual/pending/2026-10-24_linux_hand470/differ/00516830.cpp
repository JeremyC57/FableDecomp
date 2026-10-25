// CThingSearchTools::GetAllLeafThings(const CFilter_Bind2<IsInQuestOrNullQuest, OverlappingArea>& filter, list<const CThingPhysical*>& result) -- appends every scan-visible AI creature (the leaf type list 1) that is in the quest (or no quest) and whose ceil-rounded bounding circle overlaps the area
// Grade: DIFFER(372v370) -- retail_asm candidate: retail rounds the bounding radius with an inline-assembly `fistp` ceil (hence its ebp frame and the 0.5f/1.0f constants homed in frame locals); this C++ reference body uses _ftol and no frame pointer (graded once)
// Leaf: GetAllLeafThings
// Retail: Fable.exe (TLC, VC7.1) @ 0x00516830, 368 bytes (+ padding to 370), `ret 8`, ebp frame; one caller 0x0050AA06.
// Callees: PeekTypeList(long) 0x0049C780, CThingFilter_IsInQuestOrNullQuest::operator() 0x006A7C60, CTCPhysicsBase::
//   GetBoundingInfo(C3DVector*, float*) 0x006AF640, _ftol 0x00BFEA70 (six conversions), malloc 0x00BFEA0E (list node).
// Shape: the landed GetAllLeafThings 0x006AE830 with a by-reference Bind2; the OverlappingArea filter inline: thing
//   PhysicsTC +0x60, bounding info, the inline-asm ceil, the filter's C2DBoxI copied, grown by the radius (X1, X0, Y0, Y1
//   order of the _ftol calls), then the centre's (long) X/Y tested inside [X0, X1) x [Y0, Y1).
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
struct CThingAICreature : CThing { enum { ThingTypeMask = 0x1f }; enum { ThingListType = 1 }; };

// ---- the overlapping-area filter (inline in retail) ----------------------------------------------------------------
struct C3DVector { float X, Y, Z; };
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    C2DBoxI() {}
    C2DBoxI(const C2DBoxI& o) : X0(o.X0), Y0(o.Y0), X1(o.X1), Y1(o.Y1) {}
};
class CTCPhysicsBase {
public:
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

struct CThingFilter_IsInQuestOrNullQuest {
    const void* QuestSectionName;
    bool operator()(const CThing& thing) const;       // 0x006A7C60
};
template <class A, class B> struct CFilter_Bind2 {     // both filters, held by reference
    const A& First;
    const B& Second;
    template <class T> __forceinline bool operator()(const T& thing) const { return First(thing) && Second(thing); }
};

struct CThingSearchToolsMethods : CThingSearchTools {
    CThingList* PeekTypeList(long type);              // 0x0049C780
    template <class TThing, class TContainer, class TFilter> void GetAllLeafThings(const TFilter& filter, TContainer& result);
};

// Every scan-visible thing of the whole thing list that is of type TThing and passes the filter.
template <class TThing, class TContainer, class TFilter>
void CThingSearchToolsMethods::GetAllLeafThings(const TFilter& filter, TContainer& result)
{
    CThingList* things = PeekTypeList(TThing::ThingListType);
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        CThing* thing = *it;
        if (thing->IsIncludedInThingScans() && filter(*(TThing*)thing))
            result.push_back((const CThingPhysical*)thing);
    }
}

typedef CFilter_Bind2<CThingFilter_IsInQuestOrNullQuest, CThingFilter_OverlappingArea> CLeafFilter;
template void CThingSearchToolsMethods::GetAllLeafThings<CThingAICreature, CResultList<const CThingPhysical*>, CLeafFilter>(const CLeafFilter&,
    CResultList<const CThingPhysical*>&);
