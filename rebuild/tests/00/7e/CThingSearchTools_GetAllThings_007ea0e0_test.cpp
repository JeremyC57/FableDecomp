#include <cstdio>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
struct __true_type { __true_type() {} };
struct CBaseObject { int Tag; };

struct CThing {
    enum { ThingTypeMask = 0 };
    unsigned char UsingComponents[0x24];
    unsigned long ThingTypeFlags;
    unsigned char StateBits;                          // Dead 1 / IncludeInThingScans 4 / Activated 8 / InLimbo 0x20
    unsigned long Accept;                             // test: which out-of-line filter instances accept the thing

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
template <class T> inline T* ThingCast(CThing* thing) { return (T*)thing; }
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

struct CBaseIntelligentPointer {
    CBaseObject* Item;
    CBaseIntelligentPointer() : Item(0) {}
    virtual ~CBaseIntelligentPointer() {}
    void SetPItem(CBaseObject* item) { Item = item; }
    CBaseObject* GetPItem() const { return Item; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* p) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)p); }
};
template <class T> struct CResultList {
    CBaseObject* Items[128];
    int Count;
    void push_back(const T& x) { Items[Count++] = x.GetPItem(); }
};
struct CThingFilter_HasTC {
    long Data;
    bool operator()(const CThing& thing) const { return ((thing.Accept >> Data) & 1) != 0; }
};

struct CThingSearchToolsMethods {
    CThingList* AllThings;
    CThingList* TypeList;
    long        LastType;
    CThingList* PeekAllThings() { return AllThings; }
    CThingList* PeekTypeList(long type) { LastType = type; return TypeList; }
    template <class TThing, class TContainer, class TFilter> void GetAllThings(const TFilter& filter, TContainer& result);
    template <class TThing, class TContainer, class TFilter> void GetAllLeafThings(const TFilter& filter, TContainer& result);
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

CBaseObject g_building;
static const int N = 128;
CThing g_things[N];
CThingList::SNode g_nodes[N + 1];
bool useSecond;

static bool Expected(const CThing& t)
{
    return (t.StateBits & 4) && (t.StateBits & 8) && !(t.StateBits & 1) && !(t.StateBits & 0x20) && (t.ThingTypeFlags & CThing::ThingTypeMask) == CThing::ThingTypeMask && (((t.Accept >> 0) & 1) != 0);
}

static int RunOnce(bool second)
{
    static const unsigned long masks[9] = { 0, 3, 7, 0xf, 0x1f, 0x47, 0x87, 0x8007, 0xffff };
    static const unsigned char states[6] = { 0x0c, 0x04, 0x08, 0x0d, 0x2c, 0x1e };
    int i = 0;
    useSecond = second;
    CThingList::SNode* head = &g_nodes[N];
    head->m_pNext = head; head->m_pPrev = head;
    for (i = 0; i < N; ++i) {
        CThing& t = g_things[i];
        int j = 0;
        for (j = 0; j < 0x24; ++j) t.UsingComponents[j] = 0;
        t.UsingComponents[0xc] = (unsigned char)((i * 5) & 0x18);
        t.ThingTypeFlags = masks[i % 9];
        t.StateBits = (i % 5 == 4) ? states[(i / 5) % 6] : 0x0c;
        t.Accept = (unsigned long)(i % 64);

        CThingList::SNode* n = &g_nodes[i];
        n->m_value = &t;
        n->m_pNext = head; n->m_pPrev = head->m_pPrev;
        head->m_pPrev->m_pNext = n; head->m_pPrev = n;
    }
    CThingList list;
    list.m_pHead = head;
    CThingSearchToolsMethods tools;
    tools.AllThings = &list; tools.TypeList = &list; tools.LastType = -1;
    CThingFilter_HasTC filt1 = { 0 };
    CResultList<CIntelligentPointer<CThing> > result;
    result.Count = 0;
    tools.GetAllThings<CThing >(filt1, result);

    int count = 0;
    int got = 0;
    {
        int k = 0;
        int idx = 0;
        for (idx = 0; idx < result.Count; ++idx) {
            const CThing* item = (const CThing*)result.Items[idx];
            while (k < N && !Expected(g_things[k])) ++k;
            if (k >= N || item != &g_things[k]) return 0;
            ++k; ++got;
        }
    }
    for (i = 0; i < N; ++i)
        if (Expected(g_things[i])) ++count;
    if (count != got) return 0;
    return count;
}

int main()
{
    int a = RunOnce(false);
    int b = RunOnce(true);
    if (a <= 0 || b <= 0) { std::printf("007ea0e0_TEST FAIL\n"); return 1; }
    std::printf("007ea0e0_TEST PASS\n");
    return 0;
}