// Behaviour test for 0x00770D60 vector<list<CWeaponTrailEntry> >::resize(n)
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_lists = 0;       // live sentinels
struct SNodeBase { SNodeBase* m_next; SNodeBase* m_prev; };
struct CTrailList {
    SNodeBase* m_node;
    CTrailList()
    {
        SNodeBase* n = (SNodeBase*)std::malloc(0x24);
        n->m_next = n;
        n->m_prev = n;
        m_node = n;
        ++g_lists;
    }
    CTrailList(const CTrailList&)
    {
        SNodeBase* n = (SNodeBase*)std::malloc(0x24);
        n->m_next = n; n->m_prev = n; m_node = n; ++g_lists;
    }
    ~CTrailList() { std::free(m_node); --g_lists; }
};
struct CTrailListVector {
    CTrailList* _M_start;
    CTrailList* _M_finish;
    CTrailList* _M_end_of_storage;
    int erases, inserts;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CTrailList* erase(CTrailList* first, CTrailList* last)
    {   // stand-in for 0x00770790 (tail erase only)
        ++erases;
        for (CTrailList* p = first; p != last; ++p) p->~CTrailList();
        _M_finish = first;
        return first;
    }
    void _M_fill_insert(CTrailList* pos, unsigned int n, const CTrailList& x)
    {   // stand-in for 0x00770990 (append within capacity)
        ++inserts;
        for (; n > 0; --n, ++pos) new (pos) CTrailList(x);
        _M_finish = pos;
    }
    void resize(unsigned int newSize, const CTrailList& x)
    {
        if (newSize < size())
            erase(_M_start + newSize, _M_finish);
        else
            _M_fill_insert(_M_finish, newSize - size(), x);
    }
    void resize(unsigned int newSize) { resize(newSize, CTrailList()); }
};

int main()
{
    static unsigned char raw[sizeof(CTrailList) * 8];
    CTrailListVector v = { (CTrailList*)raw, (CTrailList*)raw, (CTrailList*)raw + 8, 0, 0 };
    v.resize(5);
    if (v.size() != 5 || g_lists != 5 || v.inserts != 1 || v.erases != 0) { std::printf("00770d60_TEST FAIL\n"); return 1; }
    v.resize(2);
    if (v.size() != 2 || g_lists != 2 || v.erases != 1) { std::printf("00770d60_TEST FAIL\n"); return 1; }
    v.resize(2);                       // equal size: the insert path with n == 0
    if (v.size() != 2 || g_lists != 2 || v.inserts != 2 || v.erases != 1) { std::printf("00770d60_TEST FAIL\n"); return 1; }
    for (CTrailList* p = v._M_start; p != v._M_finish; ++p)
        if (p->m_node->m_next != p->m_node || p->m_node->m_prev != p->m_node) { std::printf("00770d60_TEST FAIL\n"); return 1; }
    std::printf("00770d60_TEST PASS\n");
    return 0;
}