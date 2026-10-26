// Behaviour test for 0x005FA1BA std::vector<pair<long, CDefPointer<const CHeroAbilityDef> > >::operator=(const vector&):
// the three STLport arms with (key, counted def pointer) pairs (CTCInventoryAbilities::Abilities).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_destroyed = 0;

class CDefPointeeBase { public:
    virtual void v00() { }
    virtual void Destroy() const { ++g_destroyed; }
    mutable long m_refCount;
    CDefPointeeBase() : m_refCount(1) { }
    virtual ~CDefPointeeBase() { }
};
class CHeroAbilityDef : public CDefPointeeBase { public: };

template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    explicit CDefPointer(T* p) : Object(p) { if (Object) ++Object->m_refCount; }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer() { if (Object && --Object->m_refCount == 0) Object->Destroy(); }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->m_refCount == 0) Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_refCount;
        }
        return *this;
    }
};
typedef CDefPointer<const CHeroAbilityDef> DP;
namespace stl {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
}
typedef stl::pair<long, DP> P;
#define T_ELEM P
namespace stl {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

template <class RandomAccessIter, class OutputIter, class Distance>
OutputIter __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result, const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
template <class ForwardIter> void __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)
{
    for (; first != last; ++first) (&*first)->~T_ELEM();
}
template <class InputIter, class ForwardIter>
ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur) new (&*cur) T_ELEM(*first);
    return cur;
}
template <class InputIter, class OutputIter>
inline OutputIter __copy_ptrs(InputIter first, InputIter last, OutputIter result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}
template <class ForwardIter> inline void _Destroy(ForwardIter first, ForwardIter last) { __destroy_aux(first, last, __false_type()); }

template <class T> class vector {
public:
    typedef T* pointer;
    typedef const T* const_pointer;
    pointer _M_start;
    pointer _M_finish;
    pointer _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    pointer _M_allocate_and_copy(unsigned int n, const_pointer first, const_pointer last)
    {
        pointer result = (pointer)std::malloc(n * sizeof(T));
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
    void _M_clear() { _Destroy(_M_start, _M_finish); std::free(_M_start); }
    vector& operator=(const vector& x)
    {
        if (&x != this) {
            const unsigned int xlen = x.size();
            if (xlen > capacity()) {
                pointer tmp = _M_allocate_and_copy(xlen, (const_pointer)x._M_start + 0, (const_pointer)x._M_finish + 0);
                _M_clear();
                _M_start = tmp;
                _M_end_of_storage = _M_start + xlen;
            }
            else if (size() >= xlen) {
                pointer i = __copy_ptrs((const_pointer)x._M_start + 0, (const_pointer)x._M_finish + 0, (pointer)_M_start, __false_type());
                _Destroy(i, _M_finish);
            }
            else {
                __copy_ptrs((const_pointer)x._M_start, (const_pointer)x._M_start + size(), (pointer)_M_start, __false_type());
                __uninitialized_copy((const_pointer)x._M_start + size(), (const_pointer)x._M_finish + 0, _M_finish, __false_type());
            }
            _M_finish = _M_start + xlen;
        }
        return *this;
    }
};
}
typedef stl::vector<P> V;
static P MakeElem(int i, CHeroAbilityDef* d) { return P(100 + i, DP(d)); }
static const CHeroAbilityDef* DefOf(const P& p) { return p.second.Object; }
static int KeyOk(const P& p, int i) { return p.first == 100 + i; }
static void Make(V& v, int n, int cap, CHeroAbilityDef* defs)
{
    v._M_start = (T_ELEM*)std::malloc(cap * sizeof(T_ELEM));
    v._M_finish = v._M_start + n;
    v._M_end_of_storage = v._M_start + cap;
    for (int i = 0; i < n; ++i) new (v._M_start + i) T_ELEM(MakeElem(i, defs + i));
}
static void Free(V& v) { for (T_ELEM* p = v._M_start; p != v._M_finish; ++p) p->~T_ELEM(); std::free(v._M_start); }

int main()
{
    int ok = 1;
    CHeroAbilityDef a[4], b[4];
    V src, dst;
    // grow arm: 3 into an empty vector with no capacity -> fresh block, three add-refs
    Make(src, 3, 3, a);
    dst._M_start = dst._M_finish = dst._M_end_of_storage = 0;
    dst = src;
    if (dst.size() != 3 || dst.capacity() != 3 || DefOf(dst._M_start[2]) != &a[2] || a[0].m_refCount != 3) ok = 0;
    if (!KeyOk(dst._M_start[1], 1)) ok = 0;
    // shrink arm: assign 2 over 4 -> two assignments, two destroyed tails
    V two;
    Make(two, 2, 2, b);
    V four;
    Make(four, 4, 4, a);
    int destroyedBefore = g_destroyed;
    four = two;
    if (four.size() != 2 || DefOf(four._M_start[0]) != &b[0] || DefOf(four._M_start[1]) != &b[1] ||
        a[3].m_refCount != 1 || a[0].m_refCount != 3 || b[0].m_refCount != 3 || g_destroyed != destroyedBefore) ok = 0;
    // middle arm: 3 into size 2 / capacity 4 -> two assignments plus one copy-construct at the end
    V mid;
    Make(mid, 2, 4, b);
    mid = src;
    if (mid.size() != 3 || mid.capacity() != 4 || DefOf(mid._M_start[0]) != &a[0] || DefOf(mid._M_start[2]) != &a[2] ||
        a[2].m_refCount != 4) ok = 0;
    // self-assignment: no change
    mid = mid;
    if (mid.size() != 3 || a[2].m_refCount != 4) ok = 0;
    Free(mid); Free(four); Free(two); Free(dst); Free(src);
    if (a[0].m_refCount != 1 || b[0].m_refCount != 1 || a[3].m_refCount != 1) ok = 0;
    if (!ok) { std::printf("005fa1ba_TEST FAIL\n"); return 1; }
    std::printf("005fa1ba_TEST PASS\n");
    return 0;
}