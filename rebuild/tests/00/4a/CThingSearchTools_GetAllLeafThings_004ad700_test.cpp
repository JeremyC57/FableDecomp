// Behaviour test for 0x004AD700 GetAllLeafThings<const CThingBuilding, vector<const CThingBuilding*>, CThingFilter_None>:
// every scan-visible building of type list 3 is appended (in list order); dead / inactive / limbo / not-scanned ones are skipped.
#include <cstdio>
inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() {} };
struct CThing {
    enum { ThingTypeMask = 0 };
    unsigned char _pad_0x00[0x20];
    unsigned char UsingComponents[0x24];
    unsigned char _pad_0x44[0x6c - 0x44];
    unsigned long ThingTypeFlags;
    unsigned char _pad_0x70[0x91 - 0x70];
    unsigned char StateBits;
    bool IsIncludedInThingScans() const
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
};
template <class T> inline bool IsThingOfType(const CThing* thing) { unsigned long mask = T::ThingTypeMask; return (thing->ThingTypeFlags & mask) == mask; }
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
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    // test stand-in for the out-of-line growth path: a fixed backing store of 8
    T Store[8];
    CResultVector() : _M_start(Store), _M_finish(Store), _M_end_of_storage(Store) {}
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend)
    {
        if (atend && fill_len == 1 && position < Store + 8) { *position = x; _M_finish = position + 1; _M_end_of_storage = _M_finish; }
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) T(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
struct CThingBuilding : CThing { enum { ThingListType = 0x3 }; };
struct CThingFilter_None { bool operator()(const CThing&) const { return true; } };
struct CThingSearchTools { CThingList Lists[8]; };
struct CThingSearchToolsMethods : CThingSearchTools {
    CThingList* PeekTypeList(long type) { return &Lists[type]; }
    template <class TThing, class TContainer, class TFilter> void GetAllLeafThings(const TFilter& filter, TContainer& result)
    {
        CThingList* things = PeekTypeList(TThing::ThingListType);
        for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
        {
            CThing* thing = *it;
            if (thing->IsIncludedInThingScans() && filter(*(TThing*)thing))
                result.push_back(ThingCast<TThing>(thing));
        }
    }
};
int main()
{
    CThingBuilding b[6];
    unsigned char states[6] = { 0x0c, 0x0d, 0x04, 0x2c, 0x0c, 0x08 };   // visible, dead, inactive, limbo, visible, not-scanned
    CThingList::SNode nodes[7];                                           // nodes[6] is the head sentinel
    int i = 0;
    for (; i < 6; ++i) { b[i].StateBits = states[i]; b[i].ThingTypeFlags = 0; nodes[i].m_value = &b[i]; }
    for (i = 0; i < 7; ++i) { nodes[i].m_pNext = &nodes[(i + 1) % 7]; nodes[i].m_pPrev = &nodes[(i + 6) % 7]; }
    nodes[6].m_value = 0;
    CThingSearchToolsMethods tools;
    for (i = 0; i < 8; ++i) tools.Lists[i].m_pHead = &nodes[6];            // every list shares the same ring here
    CResultVector<const CThingBuilding*> result;
    CThingFilter_None none;
    tools.GetAllLeafThings<const CThingBuilding, CResultVector<const CThingBuilding*>, CThingFilter_None>(none, result);
    bool ok = (result._M_finish - result._M_start) == 2 && result._M_start[0] == &b[0] && result._M_start[1] == &b[4];
    CThingList empty; CThingList::SNode lone; lone.m_pNext = lone.m_pPrev = &lone; lone.m_value = 0; empty.m_pHead = &lone;
    tools.Lists[3] = empty;
    CResultVector<const CThingBuilding*> result2;
    tools.GetAllLeafThings<const CThingBuilding, CResultVector<const CThingBuilding*>, CThingFilter_None>(none, result2);
    ok = ok && result2._M_finish == result2._M_start;
    if (!ok) { std::printf("004ad700_TEST FAIL\n"); return 1; }
    std::printf("004ad700_TEST PASS\n");
    return 0;
}