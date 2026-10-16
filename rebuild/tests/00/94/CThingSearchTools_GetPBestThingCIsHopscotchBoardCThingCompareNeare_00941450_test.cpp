#include <cstdio>
struct CThingSearchTools { void* WorldMap; void* PThingMapManager; void* ThingManager; };

struct CThing {
    enum { ThingTypeMask = 0 };                       // every thing matches the CThing type mask
    virtual void VSlot00(); virtual void VSlot01(); virtual void VSlot02(); virtual void VSlot03();
    virtual void VSlot04(); virtual void VSlot05(); virtual void VSlot06(); virtual void VSlot07();
    virtual void VSlot08(); virtual void VSlot09(); virtual void VSlot10(); virtual void VSlot11();
    virtual void VSlot12(); virtual void VSlot13(); virtual void VSlot14(); virtual void VSlot15();
    virtual void VSlot16(); virtual void VSlot17(); virtual void VSlot18(); virtual void VSlot19();
    virtual void VSlot20(); virtual void VSlot21(); virtual void VSlot22(); virtual void VSlot23();
    virtual void VSlot24(); virtual void VSlot25(); virtual void VSlot26(); virtual void VSlot27();
    virtual void VSlot28(); virtual void VSlot29(); virtual void VSlot30(); virtual void VSlot31();
    virtual void VSlot32(); virtual void VSlot33(); virtual void VSlot34(); virtual void VSlot35();
    virtual void VSlot36(); virtual void VSlot37(); virtual void VSlot38(); virtual void VSlot39();
    virtual void VSlot40(); virtual void VSlot41(); virtual void VSlot42(); virtual void VSlot43();
    virtual void VSlot44(); virtual void VSlot45(); virtual void VSlot46(); virtual void VSlot47();
    virtual unsigned long GetVirtualFlags() const;    // slot 48 (+0xc0): thing flag word (bit 15 tested)
    unsigned char _pad_0x04[0x1c];
    unsigned long UsingComponents[9];                 // +0x20 bitset of present thing components
    unsigned char _pad_0x44[0x68 - 0x44];
    long WorldFrameCreatedAt;
    unsigned long ThingTypeFlags;                     // +0x6c THING_TYPE_* inheritance bits
    unsigned char _pad_0x70[0x91 - 0x70];
    unsigned char StateBits;                          // +0x91 Dead / IncludeInThingScans / Activated / InLimbo
    int TestPass, TestCategory, TestDist; unsigned long TestVFlags;   // test-only fields

    // CThing::IsIncludedInThingScans (out of line at 0x0048BC40), inlined here.
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1u << (type & 31)))
            return true;
        return false;
    }
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
};

struct CIsHopscotchBoard {                           // inline: virtual flags bit 15 and thing component 0x2d present
    bool operator()(const CThing* thing) const
    {
        return ((unsigned char)(thing->GetVirtualFlags() >> 15) & 1) && thing->HasTC(0x2d);
    }
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

template bool CThingSearchToolsMethods::GetPBestThing<CIsHopscotchBoard, CThingCompare_Nearest, CThingObject >(const CIsHopscotchBoard&, const CThingCompare_Nearest&, CThingObject**);

static CThingList g_list;
static CThingList g_empty;
CThingList* CThingSearchToolsMethods::PeekAllThings() { return &g_list; }
CThingList* CThingSearchToolsMethods::PeekTypeList(long) { return &g_list; }
void CThing::VSlot00() {}
void CThing::VSlot01() {}
void CThing::VSlot02() {}
void CThing::VSlot03() {}
void CThing::VSlot04() {}
void CThing::VSlot05() {}
void CThing::VSlot06() {}
void CThing::VSlot07() {}
void CThing::VSlot08() {}
void CThing::VSlot09() {}
void CThing::VSlot10() {}
void CThing::VSlot11() {}
void CThing::VSlot12() {}
void CThing::VSlot13() {}
void CThing::VSlot14() {}
void CThing::VSlot15() {}
void CThing::VSlot16() {}
void CThing::VSlot17() {}
void CThing::VSlot18() {}
void CThing::VSlot19() {}
void CThing::VSlot20() {}
void CThing::VSlot21() {}
void CThing::VSlot22() {}
void CThing::VSlot23() {}
void CThing::VSlot24() {}
void CThing::VSlot25() {}
void CThing::VSlot26() {}
void CThing::VSlot27() {}
void CThing::VSlot28() {}
void CThing::VSlot29() {}
void CThing::VSlot30() {}
void CThing::VSlot31() {}
void CThing::VSlot32() {}
void CThing::VSlot33() {}
void CThing::VSlot34() {}
void CThing::VSlot35() {}
void CThing::VSlot36() {}
void CThing::VSlot37() {}
void CThing::VSlot38() {}
void CThing::VSlot39() {}
void CThing::VSlot40() {}
void CThing::VSlot41() {}
void CThing::VSlot42() {}
void CThing::VSlot43() {}
void CThing::VSlot44() {}
void CThing::VSlot45() {}
void CThing::VSlot46() {}
void CThing::VSlot47() {}
unsigned long CThing::GetVirtualFlags() const { return TestVFlags; }
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
    CIsHopscotchBoard filter = CIsHopscotchBoard();
    CThingCompare_Nearest compare = CThingCompare_Nearest();
    CThingSearchToolsMethods tools;
    CThingObject* out = 0;
    bool ok = tools.GetPBestThing(filter, compare, &out);
    if (!ok || out != (CThingObject*)things[4]) { std::printf("00941450_TEST FAIL\n"); return 1; }
    g_list = g_empty;
    out = (CThingObject*)things[0];
    ok = tools.GetPBestThing(filter, compare, &out);
    if (ok || out != 0) { std::printf("00941450_TEST FAIL\n"); return 1; }
    std::printf("00941450_TEST PASS\n");
    return 0;
}