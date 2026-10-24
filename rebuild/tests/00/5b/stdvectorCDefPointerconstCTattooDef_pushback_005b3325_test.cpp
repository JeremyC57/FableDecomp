// behaviour test for 005b3325: STLport vector<CDefPointer<const CTattooDef>>::push_back (in-place add-ref copy or overflow grow)
#include <cstdio>
#include <stddef.h>
#include <new>
struct CDefPointee { int m_refCount; };
struct CDefPointer {
    CDefPointee* Object;
    CDefPointer(CDefPointee* p) : Object(p) {}
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->m_refCount; }
};
static int g_overflows = 0;
struct VectorModel {
    CDefPointer* _M_start;
    CDefPointer* _M_finish;
    CDefPointer* _M_end_of_storage;
    void _M_insert_overflow(CDefPointer* pos, const CDefPointer& x, size_t fill_len, bool atend)
    {
        ++g_overflows;
        if (pos == _M_finish && fill_len == 1 && atend) { /* model: grow by one slot of the backing store */
            ++_M_end_of_storage;
            new (_M_finish) CDefPointer(x);
            ++_M_finish;
        }
    }
    void push_back(const CDefPointer& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CDefPointer(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, 1, true);
    }
};
int main()
{
    unsigned char raw[8 * sizeof(CDefPointer)];
    CDefPointer* store = (CDefPointer*)raw;
    VectorModel v; v._M_start = v._M_finish = store; v._M_end_of_storage = store + 2;
    CDefPointee a = { 1 }, b = { 5 };
    CDefPointer pa(&a), pb(&b), pn(0);
    int bad = 0;
    v.push_back(pa);
    v.push_back(pn);
    if (v._M_finish != store + 2 || a.m_refCount != 2 || store[0].Object != &a || store[1].Object != 0 || g_overflows != 0) ++bad;
    v.push_back(pb);   /* full: the overflow path takes it */
    if (g_overflows != 1 || v._M_finish != store + 3 || b.m_refCount != 6 || store[2].Object != &b) ++bad;
    if (bad) { std::printf("005b3325_TEST FAIL\n"); return 1; }
    std::printf("005b3325_TEST PASS\n");
    return 0;
}