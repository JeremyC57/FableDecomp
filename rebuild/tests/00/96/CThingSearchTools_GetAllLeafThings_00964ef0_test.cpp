#include <cstdio>
#include <cstdlib>
#include <new>
struct CThing {
    unsigned char StateBits;          // Dead 0x01, IncludeInThingScans 0x04, Activated 0x08, InLimbo 0x20
    long Tag;
    bool IsIncludedInThingScans() const
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
};
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
// Vector stand-in: two slots of capacity, the overflow path grows into a bigger buffer.
template <class T> struct CVector {
    T Storage[16];
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long Overflows;
    CVector() : _M_start(Storage), _M_finish(Storage), _M_end_of_storage(Storage + 2), Overflows(0) {}
    void _M_insert_overflow(T* position, const T& x, unsigned long fill_len)
    {
        ++Overflows;
        _M_end_of_storage = Storage + 16;
        *position = x;
        _M_finish = position + fill_len;
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, 1);
    }
    long size() const { return (long)(_M_finish - _M_start); }
};
// List stand-in: push_back is insert(end(), x) through a malloc'd node, as in retail.
template <class T> struct CList {
    struct SNode { SNode* _M_next; SNode* _M_prev; T _M_data; };
    SNode* _M_node;
    CList() { _M_node = (SNode*)std::malloc(sizeof(SNode)); _M_node->_M_next = _M_node; _M_node->_M_prev = _M_node; }
    ~CList() { SNode* n = _M_node->_M_next; while (n != _M_node) { SNode* next = n->_M_next; std::free(n); n = next; } std::free(_M_node); }
    void push_back(const T& x)
    {
        SNode* tmp = (SNode*)std::malloc(sizeof(SNode));
        new (&tmp->_M_data) T(x);
        SNode* n = _M_node;
        SNode* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
    }
};
struct CThingMarker : CThing { enum { ThingListType = 0xb }; };
struct CThingFilter_MarkerHasProperty {
    long Property;
    bool operator()(CThingMarker* marker) const { return (marker->Tag & 1) == Property; }
};
// The candidate logic.
struct CThingSearchTools {
    CThingList* Lists[16];
    CThingList* PeekTypeList(long type) { return Lists[type]; }
    template <class TThing, class TContainer, class TFilter> void GetAllLeafThings(const TFilter& filter, TContainer& things)
    {
        CThingList* list = PeekTypeList(TThing::ThingListType);
        for (CThingList::iterator it = list->begin(); it != list->end(); ++it)
        {
            CThing* thing = *it;
            if (thing->IsIncludedInThingScans() && filter((TThing*)thing))
                things.push_back((TThing*)thing);
        }
    }
};
int main()
{
    // six things: tags 0..5; 1 is dead, 3 is in limbo, 5 is not activated; the rest are scan-visible
    CThingMarker things[6];
    unsigned char bits[6] = { 0x0c, 0x0d, 0x0c, 0x2c, 0x0c, 0x04 };
    CThingList::SNode nodes[7];
    long i = 0;
    for (; i < 6; ++i) {
        things[i].StateBits = bits[i];
        things[i].Tag = i;
        nodes[i + 1].m_value = &things[i];
    }
    for (i = 0; i < 7; ++i) {
        nodes[i].m_pNext = &nodes[(i + 1) % 7];
        nodes[i].m_pPrev = &nodes[(i + 6) % 7];
    }
    CThingList list;
    list.m_pHead = &nodes[0];
    CThingSearchTools tools;
    for (i = 0; i < 16; ++i)
        tools.Lists[i] = 0;
    tools.Lists[CThingMarker::ThingListType] = &list;
    bool ok = true;
    CThingFilter_MarkerHasProperty filter = { 0 };
    CVector<CThingMarker*> out;
    tools.GetAllLeafThings<CThingMarker>(filter, out);
    // visible: 0, 2, 4, all even -> all three, the third through the overflow path
    if (out.size() != 3 || out.Storage[0]->Tag != 0 || out.Storage[2]->Tag != 4 || out.Overflows != 1) ok = false;
    CThingFilter_MarkerHasProperty odd = { 1 };
    CVector<CThingMarker*> none;
    tools.GetAllLeafThings<CThingMarker>(odd, none);
    if (none.size() != 0) ok = false;
    if (!ok) { std::printf("00964ef0_TEST FAIL\n"); return 1; }
    std::printf("00964ef0_TEST PASS\n");
    return 0;
}