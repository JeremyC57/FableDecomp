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

template <class T> struct CResultVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend);
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
// test stand-in for the out-of-line STLport overflow: grow, copy, fill, (copy the tail)
template <class T> void CResultVector<T>::_M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend)
{
    unsigned long size = (unsigned long)(_M_finish - _M_start);
    unsigned long cap = size + (size > fill_len ? size : fill_len);
    T* mem = (T*)malloc(cap * sizeof(T));
    T* out = mem;
    T* p;
    unsigned long i;
    for (p = _M_start; p != position; ++p, ++out) *out = *p;
    for (i = 0; i < fill_len; ++i, ++out) *out = x;
    if (!atend) for (p = position; p != _M_finish; ++p, ++out) *out = *p;
    free(_M_start);
    _M_start = mem; _M_finish = out; _M_end_of_storage = mem + cap;
}
struct CThingAICreature : CThing { enum { ThingTypeMask = 0x1f }; };
struct CThingFilter_None {
    bool operator()(const CThing&) const { return true; }
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
            result.push_back(ThingCast<TThing>(thing));
    }
}

CBaseObject g_building;
static const int N = 128;
CThing g_things[N];
CThingList::SNode g_nodes[N + 1];
bool useSecond;

static bool Expected(const CThing& t)
{
    return (t.StateBits & 4) && (t.StateBits & 8) && !(t.StateBits & 1) && !(t.StateBits & 0x20) && (t.ThingTypeFlags & CThingAICreature::ThingTypeMask) == CThingAICreature::ThingTypeMask && true;
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
    CThingFilter_None filt1;
    CResultVector<CThingAICreature*> result;
    result._M_start = (CThingAICreature**)malloc(2 * sizeof(CThingAICreature*));
    result._M_finish = result._M_start;
    result._M_end_of_storage = result._M_start + 2;
    tools.GetAllThings<CThingAICreature >(filt1, result);

    int count = 0;
    int got = 0;
    {
        int k = 0;
        int idx = 0;
        for (idx = 0; result._M_start + idx != result._M_finish; ++idx) {
            const CThing* item = (const CThing*)result._M_start[idx];
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
    if (a <= 0 || b <= 0) { std::printf("0052b270_TEST FAIL\n"); return 1; }
    std::printf("0052b270_TEST PASS\n");
    return 0;
}