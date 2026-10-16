#include <cstdio>
struct CThingSearchTools { void* WorldMap; void* PThingMapManager; void* ThingManager; };

struct CThing {
    enum { ThingTypeMask = 0 };                       // every thing matches the CThing type mask
    unsigned char _pad_0x00[0x20];
    unsigned long UsingComponents[9];                 // +0x20 bitset of present thing components
    unsigned char _pad_0x44[0x68 - 0x44];
    long WorldFrameCreatedAt;
    unsigned long ThingTypeFlags;                     // +0x6c THING_TYPE_* inheritance bits
    unsigned char _pad_0x70[0x91 - 0x70];
    unsigned char StateBits;                          // +0x91 Dead / IncludeInThingScans / Activated / InLimbo
    int TestPass, TestCategory, TestDist; unsigned long TestVFlags;   // test-only fields

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

template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
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

struct CThingGameObject : CThing {
    enum { ThingTypeMask = 0x7 };
};
struct CThingObject : CThingGameObject {
    enum { ThingTypeMask = 0x807 };
    enum { ThingListType = 0x5 };                // PeekTypeList index
};

struct CIsNearCampfireWithSpace {
    float DistanceSq; float FromX, FromY, FromZ;
    bool operator()(const CThing* thing) const;      // 0x00939A50
};
struct CThingCompare_Nearest {                       // out-of-line member template per thing type
    float X, Y, Z;
    template <class T> T* operator()(T* best, T* candidate) const;
};

struct CThingSearchToolsMethods : CThingSearchTools {
    CThingList* PeekAllThings();                      // 0x0049C770: &ThingManager->(all-things list at +0x18)
    CThingList* PeekTypeList(long type);              // 0x0049C780: the per-type thing list
    template <class TFilter, class TThing> bool GetPFirstThing(const TFilter& filter, TThing** out);
    template <class TFilter, class TThing> bool GetPFirstLeafThing(const TFilter& filter, TThing** out);
    template <class TFilter, class TCompare, class TThing> bool GetPBestThing(const TFilter& filter, const TCompare& compare, TThing** out);
    template <class TFilter, class TCompare, class TThing> bool GetPBestLeafThing(const TFilter& filter, const TCompare& compare, TThing** out);
};

template <class TFilter, class TThing>
bool CThingSearchToolsMethods::GetPFirstThing(const TFilter& filter, TThing** out)
{
    CThingList* things = PeekAllThings();
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        CThing* thing = *it;
        if (IsThingOfType<TThing>(thing) && thing->IsIncludedInThingScans() && filter((TThing*)thing))
        {
            *out = (TThing*)thing;
            return true;
        }
    }
    return false;
}

template <class TFilter, class TThing>
bool CThingSearchToolsMethods::GetPFirstLeafThing(const TFilter& filter, TThing** out)
{
    CThingList* things = PeekTypeList(TThing::ThingListType);
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        CThing* thing = *it;
        if (thing->IsIncludedInThingScans() && filter((TThing*)thing))
        {
            *out = (TThing*)thing;
            return true;
        }
    }
    return false;
}

template <class TFilter, class TCompare, class TThing>
bool CThingSearchToolsMethods::GetPBestThing(const TFilter& filter, const TCompare& compare, TThing** out)
{
    TThing* best = 0;
    CThingList* things = PeekAllThings();
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        // `*it` is re-read for each use: VC7.1 drops a null test on a pointer it has seen dereferenced,
        // and only a fresh `*it` keeps retail's `test esi,esi` after the type test.
        if ((*it)->IsIncludedInThingScans() && IsThingOfType<TThing>(*it))
        {
            TThing* candidate = (TThing*)*it;
            if (candidate && filter(candidate))
                best = compare(best, candidate);
        }
    }
    *out = best;
    return best != 0;
}

template <class TFilter, class TCompare, class TThing>
bool CThingSearchToolsMethods::GetPBestLeafThing(const TFilter& filter, const TCompare& compare, TThing** out)
{
    TThing* best = 0;
    CThingList* things = PeekTypeList(TThing::ThingListType);
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        CThing* thing = *it;
        if (thing->IsIncludedInThingScans() && filter((TThing*)thing))
            best = compare(best, (TThing*)thing);
    }
    *out = best;
    return best != 0;
}

template bool CThingSearchToolsMethods::GetPBestLeafThing<CIsNearCampfireWithSpace, CThingCompare_Nearest, CThingObject >(const CIsNearCampfireWithSpace&, const CThingCompare_Nearest&, CThingObject**);

static CThingList g_list;
static CThingList g_empty;
CThingList* CThingSearchToolsMethods::PeekAllThings() { return &g_list; }
CThingList* CThingSearchToolsMethods::PeekTypeList(long) { return &g_list; }
bool CIsNearCampfireWithSpace::operator()(const CThing* thing) const { return thing->TestPass != 0; }
template <class T> T* CThingCompare_Nearest::operator()(T* best, T* candidate) const
{
    if (!best || candidate->TestDist < best->TestDist) return candidate;
    return best;
}

static void Link(CThingList& l, CThingList::SNode* head, CThingList::SNode* nodes, CThing** things, int n)
{
    l.m_pHead = head; head->m_pNext = head; head->m_pPrev = head;
    for (int i = 0; i < n; ++i) {
        CThingList::SNode* nd = &nodes[i];
        nd->m_value = things[i];
        nd->m_pPrev = head->m_pPrev; nd->m_pNext = head;
        head->m_pPrev->m_pNext = nd; head->m_pPrev = nd;
    }
}
int main()
{
    static CThingObject objs[5];
    CThing* things[5];
    // 0 hidden from scans; 1 untyped (CThing only); 2 typed, fails the filters; 3 and 4 typed and passing
    const int vis[5] = { 0x00, 0x0c, 0x0c, 0x0c, 0x0c };
    const int typ[5] = { 0xffff, 0, 0xffff, 0xffff, 0xffff };
    const int pass[5] = { 1, 1, 0, 1, 1 };
    const int dist[5] = { 0, 9, 7, 5, 2 };
    const int born[5] = { 9, 1, 8, 3, 2 };
    for (int i = 0; i < 5; ++i) {
        CThing* t = &objs[i];
        t->StateBits = (unsigned char)vis[i];
        t->ThingTypeFlags = (unsigned long)typ[i];
        t->TestPass = pass[i];
        t->TestCategory = pass[i] ? 1 : 0;
        t->TestDist = dist[i];
        t->TestVFlags = pass[i] ? 0x8000u : 0u;
        for (int w = 0; w < 9; ++w) t->UsingComponents[w] = 0;
        if (pass[i]) t->UsingComponents[1] = 1u << 13;
        t->WorldFrameCreatedAt = born[i];
        things[i] = t;
    }
    CThingList::SNode head, nodes[5], head2;
    Link(g_list, &head, nodes, things, 5);
    Link(g_empty, &head2, nodes, things, 0);
    CIsNearCampfireWithSpace filter = CIsNearCampfireWithSpace();
    CThingCompare_Nearest compare = CThingCompare_Nearest();
    CThingSearchToolsMethods tools;
    CThingObject* out = 0;
    bool ok = tools.GetPBestLeafThing(filter, compare, &out);
    if (!ok || out != (CThingObject*)things[4]) { std::printf("00939af0_TEST FAIL\n"); return 1; }
    g_list = g_empty;
    out = (CThingObject*)things[0];
    ok = tools.GetPBestLeafThing(filter, compare, &out);
    if (ok || out != 0) { std::printf("00939af0_TEST FAIL\n"); return 1; }
    std::printf("00939af0_TEST PASS\n");
    return 0;
}