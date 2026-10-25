// Test for 0x005B3275 std::vector<CDefPointer<const CThingBaseDef> >::_M_insert_overflow: the source object must allocate
// old_size + max(old_size, fill_len) slots, copy the head, add fill_len add-ref'd copies of x, copy the tail unless
// atend, call _M_clear on the old block (stubbed here) and install the new pointers.
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();
    long m_refCount;
};
void CDefPointeeBase::v00() { }
void CDefPointeeBase::Destroy() { }
struct CThingBaseDef;
template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;
    ~CDefPointer();
};
typedef CDefPointer<const CThingBaseDef> P;
template <> CDefPointer<const CThingBaseDef>::~CDefPointer() { if (Object) --Object->m_refCount; }
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> struct allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc { _Value _M_data; };
template <class _Tp> class vector {
public:
    _Tp* _M_start; _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, allocator<_Tp> > _M_end_of_storage;
    void _M_clear();
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, size_t __fill_len, bool __atend = false);
};
static int g_clears;
template <> void vector<P>::_M_clear()
{
    ++g_clears;
    for (P* q = _M_start; q != _M_finish; ++q) q->~P();
    free(_M_start);
}
}
int main()
{
    CDefPointeeBase a, b, c; a.m_refCount = b.m_refCount = 0; c.m_refCount = 1;
    std::vector<P> v;
    v._M_start = (P*)malloc(2 * sizeof(P)); v._M_finish = v._M_start + 2; v._M_end_of_storage._M_data = v._M_start + 2;
    v._M_start[0].Object = &a; v._M_start[1].Object = &b; a.m_refCount = b.m_refCount = 1;
    P x; x.Object = &c;
    std::__false_type tag;
    v._M_insert_overflow(v._M_finish, x, tag, 1, true);                    // push_back growth
    bool ok = std::g_clears == 1 && v._M_finish - v._M_start == 3 && v._M_end_of_storage._M_data - v._M_start == 4
        && v._M_start[0].Object == &a && v._M_start[2].Object == &c && a.m_refCount == 1 && c.m_refCount == 2;
    v._M_insert_overflow(v._M_start + 1, x, tag, 3, false);                // middle insert of 3
    ok = ok && std::g_clears == 2 && v._M_finish - v._M_start == 6 && v._M_end_of_storage._M_data - v._M_start == 6
        && v._M_start[0].Object == &a && v._M_start[1].Object == &c && v._M_start[3].Object == &c
        && v._M_start[4].Object == &b && v._M_start[5].Object == &c && a.m_refCount == 1 && b.m_refCount == 1 && c.m_refCount == 5;
    x.Object = 0;
    if (!ok) { puts("005b3275_TEST FAIL"); return 1; }
    puts("005b3275_TEST PASS");
    return 0;
}
