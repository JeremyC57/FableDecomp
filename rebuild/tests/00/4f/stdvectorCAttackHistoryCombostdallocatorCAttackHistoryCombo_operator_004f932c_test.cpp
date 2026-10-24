// Behaviour test for 0x004F932C vector<CAttackHistoryCombo>::operator=: the three STLport branches (reallocate,
// shrink with destroy, grow within capacity) copy every combo (its pair vector and multiplier) and keep no leaks.
#include <stdio.h>
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif

enum EAttackHistoryType { EAttackHistoryType_Force32 = 0x7fffffff };
enum EDamageAttribute { EDamageAttribute_Force32 = 0x7fffffff };

namespace std {

struct __true_type { };
struct __false_type { };

template <class T> struct __type_traits {
    typedef __true_type this_dummy_member_must_be_first;
    typedef __false_type has_trivial_default_constructor;
    typedef __false_type has_trivial_copy_constructor;
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type has_trivial_destructor;
    typedef __false_type is_POD_type;
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

template <class T1, class T2>
__forceinline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class InputIter, class ForwardIter>
__forceinline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template <class InputIter, class OutputIter>
OutputIter __fastcall __copy_ptrs(InputIter first, InputIter last, OutputIter result, const __false_type&);   // 0x004F94D2

template <class T>
__forceinline void _Destroy(T* p)
{
    p->~T();
}

template <class ForwardIter>
void __fastcall __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)   // folded: 0x004E41C0
{
    for (; first != last; ++first)
        _Destroy(&*first);
}

template <class ForwardIter, class T>
__forceinline void __destroy(ForwardIter first, ForwardIter last, T*)
{
    typedef typename __type_traits<T>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(first, last, _Trivial_destructor());
}

template <class ForwardIter>
__forceinline void _Destroy(ForwardIter first, ForwardIter last)
{
    __destroy(first, last, first);
}

template <class T> class allocator {
public:
    allocator() { }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

template <class T, class A> class _Vector_base {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;          // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    typedef typename __type_traits<T>::has_trivial_assignment_operator _TrivialAss;
    typedef typename __type_traits<T>::is_POD_type _IsPODType;
    vector() { this->_M_start = this->_M_finish = 0; this->_M_end_of_storage._M_data = 0; }   // test only
    void test_fill(unsigned int n, unsigned int cap, const T& v);                                // test only
    T& at(unsigned int i) { return this->_M_start[i]; }                                       // test only
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    unsigned int capacity() const { return (unsigned int)(this->_M_end_of_storage._M_data - this->_M_start); }
    vector(const vector& x);                             // pair vector: 0x004F9408
    ~vector();                                           // pair vector (frees the block)
    template <class ForwardIter>
    T* _M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last);   // 0x004F9484
    void _M_clear();                                     // 0x004F963D
    vector<T, A>& operator=(const vector<T, A>& x);
};

template <class T, class A>
vector<T, A>& vector<T, A>::operator=(const vector<T, A>& x)
{
    if (&x != this) {
        const unsigned int xlen = x.size();
        if (xlen > capacity()) {
            T* tmp = _M_allocate_and_copy(xlen, (const T*)x._M_start + 0, (const T*)x._M_finish + 0);
            _M_clear();
            this->_M_start = tmp;
            this->_M_end_of_storage._M_data = this->_M_start + xlen;
        }
        else if (size() >= xlen) {
            T* i = __copy_ptrs((const T*)x._M_start + 0, (const T*)x._M_finish + 0, (T*)this->_M_start, _TrivialAss());
            _Destroy(i, this->_M_finish);
        }
        else {
            __copy_ptrs((const T*)x._M_start, (const T*)x._M_start + size(), (T*)this->_M_start, _TrivialAss());
            __uninitialized_copy((const T*)x._M_start + size(), (const T*)x._M_finish + 0, this->_M_finish, _IsPODType());
        }
        this->_M_finish = this->_M_start + xlen;
    }
    return *this;
}

}

class CAttackHistoryCombo {                         // Ego_r.pdb (retail 0x10 bytes)
public:
    std::vector<std::pair<EAttackHistoryType, EDamageAttribute> > AttackCombo;   // +0x00
    float Multiplier;                                                           // +0x0C
    CAttackHistoryCombo() : Multiplier(0.0f) { }                                // test only
    __forceinline CAttackHistoryCombo(const CAttackHistoryCombo& x) : AttackCombo(x.AttackCombo), Multiplier(x.Multiplier) { }
};


static int g_live = 0;                 // live element blocks (malloc'd by the stand-ins below)
namespace std {
template <class T, class A> void vector<T, A>::test_fill(unsigned int n, unsigned int cap, const T& v)
{
    this->_M_start = (T*)malloc(cap * sizeof(T) + 1); ++g_live;
    this->_M_finish = this->_M_start;
    for (unsigned int k = 0; k < n; ++k) { new (this->_M_finish) T(v); ++this->_M_finish; }
    this->_M_end_of_storage._M_data = this->_M_start + cap;
}
template <class T, class A> vector<T, A>::vector(const vector<T, A>& x)
{
    unsigned int n = x.size();
    this->_M_start = (T*)malloc(n * sizeof(T) + 1); ++g_live;
    this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, __false_type());
    this->_M_end_of_storage._M_data = this->_M_start + n;
}
template <class T, class A> vector<T, A>::~vector()
{
    _Destroy(this->_M_start, this->_M_finish);
    if (this->_M_start) { free(this->_M_start); --g_live; }
}
template <class T, class A> template <class ForwardIter>
T* vector<T, A>::_M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last)
{
    T* r = (T*)malloc(n * sizeof(T) + 1); ++g_live;
    __uninitialized_copy(first, last, r, __false_type());
    return r;
}
template <class T, class A> void vector<T, A>::_M_clear()
{
    _Destroy(this->_M_start, this->_M_finish);
    if (this->_M_start) { free(this->_M_start); --g_live; }
}
template <class InputIter, class OutputIter>
OutputIter __fastcall __copy_ptrs(InputIter first, InputIter last, OutputIter result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        *result = *first;
    return result;
}
}

template std::vector<CAttackHistoryCombo>& std::vector<CAttackHistoryCombo>::operator=(const std::vector<CAttackHistoryCombo>& x);


typedef std::pair<EAttackHistoryType, EDamageAttribute> TStep;
typedef std::vector<TStep> TSteps;
typedef std::vector<CAttackHistoryCombo> TCombos;

static CAttackHistoryCombo MakeCombo(unsigned int steps, int tag, float mult)
{
    CAttackHistoryCombo c;
    TStep s; s.first = (EAttackHistoryType)tag; s.second = (EDamageAttribute)(tag + 1);
    c.AttackCombo.test_fill(steps, steps + 1, s);
    c.Multiplier = mult;
    return c;
}

static int Same(TCombos& a, TCombos& b)
{
    if (a.size() != b.size()) return 0;
    for (unsigned int i = 0; i < a.size(); ++i) {
        if (a.at(i).Multiplier != b.at(i).Multiplier) return 0;
        if (a.at(i).AttackCombo.size() != b.at(i).AttackCombo.size()) return 0;
        for (unsigned int k = 0; k < a.at(i).AttackCombo.size(); ++k)
            if (a.at(i).AttackCombo.at(k).first != b.at(i).AttackCombo.at(k).first ||
                a.at(i).AttackCombo.at(k).second != b.at(i).AttackCombo.at(k).second) return 0;
    }
    return 1;
}

int main()
{
    int ok = 1;
    {
        CAttackHistoryCombo c1 = MakeCombo(2, 10, 1.5f);
        CAttackHistoryCombo c2 = MakeCombo(3, 20, 2.0f);
        TCombos big;  big.test_fill(3, 3, c1);  big.at(2).Multiplier = 4.0f;
        TCombos small; small.test_fill(1, 4, c2);
        TCombos empty;

        // grow within capacity: size 1 < 3 <= capacity? (cap 4): copy overlap + uninitialised tail
        TCombos& r1 = (small = big);
        if (&r1 != &small || !Same(small, big) || small.capacity() != 4) ok = 0;

        // shrink: size 3 >= 1 -> assign overlap, destroy the surplus
        TCombos one; one.test_fill(1, 1, c2);
        small = one;
        if (!Same(small, one) || small.capacity() != 4) ok = 0;

        // reallocate: 3 > capacity 1
        TCombos& r3 = (one = big);
        if (&r3 != &one || !Same(one, big) || one.capacity() != 3) ok = 0;

        // self-assignment is a no-op
        big = big;
        if (big.size() != 3 || big.at(2).Multiplier != 4.0f) ok = 0;

        // assign empty
        small = empty;
        if (small.size() != 0) ok = 0;
    }
    if (g_live != 0) ok = 0;
    printf("004f932c_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}