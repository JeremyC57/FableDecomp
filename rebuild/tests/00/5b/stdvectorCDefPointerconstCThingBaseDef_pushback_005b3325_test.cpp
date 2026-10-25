// Test for 0x005B3325 std::vector<CDefPointer<const CThingBaseDef> >::push_back: with room the source object must
// copy-construct x at _M_finish (adding a reference) and advance; when full it must call _M_insert_overflow(_M_finish,
// x, __false_type(), 1, true) (stubbed here).
#include <stdio.h>
#include <stddef.h>
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy() const;
    mutable long m_refCount;
};
void CDefPointeeBase::v00() { }
void CDefPointeeBase::Destroy() const { }
struct CThingBaseDef;
template <class T> struct CDefPointer { CDefPointeeBase* Object; };
typedef CDefPointer<const CThingBaseDef> P;
namespace std {
struct __false_type { __false_type() {} };
template <class T> class allocator { public: allocator() {} };
template <class Value, class Tp, class MaybeReboundAlloc>
class _STLP_alloc_proxy : public MaybeReboundAlloc { public: Value _M_data; };
template <class T, class Alloc = allocator<T> >
class vector {
public:
    typedef T* iterator;
    typedef size_t size_type;
    T* _M_start; T* _M_finish;
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;
    void _M_insert_overflow(iterator pos, const T& x, const __false_type&, size_type fill_len, bool atend);
    void push_back(const T& x);
};
static int g_over; static const void* g_pos; static const void* g_x; static size_t g_fill; static bool g_atend;
template <> void vector<P>::_M_insert_overflow(iterator pos, const P& x, const __false_type&, size_type fill_len, bool atend)
{
    ++g_over; g_pos = pos; g_x = &x; g_fill = fill_len; g_atend = atend;
}
}
int main()
{
    CDefPointeeBase d; d.m_refCount = 1;
    P slots[2];
    std::vector<P> v; v._M_start = v._M_finish = slots; v._M_end_of_storage._M_data = slots + 2;
    P x; x.Object = &d;
    v.push_back(x);
    bool ok = v._M_finish == slots + 1 && slots[0].Object == &d && d.m_refCount == 2 && std::g_over == 0;
    P n; n.Object = 0;
    v.push_back(n);
    ok = ok && v._M_finish == slots + 2 && slots[1].Object == 0 && d.m_refCount == 2;
    v.push_back(x);                                                        // full: overflow path
    ok = ok && std::g_over == 1 && std::g_pos == slots + 2 && std::g_x == &x && std::g_fill == 1 && std::g_atend
        && d.m_refCount == 2 && v._M_finish == slots + 2;
    if (!ok) { puts("005b3325_TEST FAIL"); return 1; }
    puts("005b3325_TEST PASS");
    return 0;
}
