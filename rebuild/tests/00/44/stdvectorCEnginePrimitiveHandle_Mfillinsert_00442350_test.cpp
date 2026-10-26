// Behaviour test for 0x00442350 vector<CEnginePrimitiveHandle>::_M_fill_insert (STLport model): in-place paths with add-ref and release, x_copy lifetime, overflow hand-off.

#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
class CEnginePersistentPrimitiveBase { public: long RefCount; int Id; };
static CEnginePersistentPrimitiveBase g_prims[2048];
class CBaseClass { public: CBaseClass() { ++g_live; } CBaseClass(const CBaseClass&) { ++g_live; } virtual ~CBaseClass() { --g_live; } };
class CEnginePrimitiveHandle : public CBaseClass {
public:
    CEnginePersistentPrimitiveBase* P;
    CEnginePrimitiveHandle() : P(0) { }
    CEnginePrimitiveHandle(const CEnginePrimitiveHandle& x) : CBaseClass(x), P(x.P) { if (P) ++P->RefCount; }
    CEnginePrimitiveHandle& operator=(const CEnginePrimitiveHandle& x) { if (x.P) ++x.P->RefCount; if (P) --P->RefCount; P = x.P; return *this; }
    ~CEnginePrimitiveHandle() { if (P) --P->RefCount; }
};
typedef CEnginePrimitiveHandle T;

static T Make(int v) { T t; t.P = &g_prims[v]; t.P->Id = v; ++t.P->RefCount; return t; }
static bool Eq(const T& a, const T& b) { return a.P == b.P && a.P->Id == b.P->Id; }

static int g_overflow_calls = 0;
struct __false_type { };

template <class _Tp> _Tp* __uninitialized_copy(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    _Tp* __cur = __result;
    for (; __first != __last; ++__first, ++__cur) new (__cur) _Tp(*__first);
    return __cur;
}
template <class _Tp> _Tp* __uninitialized_fill_n(_Tp* __first, unsigned int __n, const _Tp& __x, const __false_type&)
{
    _Tp* __cur = __first;
    for (; __n > 0; --__n, ++__cur) new (__cur) _Tp(__x);
    return __cur;
}
template <class _Tp> _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    for (int __n = int(__last - __first); __n > 0; --__n) *--__result = *--__last;
    return __result;
}
template <class _Tp> void fill(_Tp* __first, _Tp* __last, const _Tp& __val) { for (; __first != __last; ++__first) *__first = __val; }
template <class _Tp> void _Destroy(_Tp* first, _Tp* last) { for (; first != last; ++first) first->~_Tp(); }

template <class _Tp> struct vector {
    _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage;
    void _M_insert_overflow(_Tp* position, const _Tp& x, const __false_type&, unsigned int fillLength, bool atEnd = false)
    {
        ++g_overflow_calls;
        const unsigned int oldSize = _M_finish - _M_start;
        const unsigned int length = oldSize + (oldSize < fillLength ? fillLength : oldSize);
        _Tp* newStart = length ? (_Tp*)std::malloc(length * sizeof(_Tp)) : 0;
        _Tp* newFinish = __uninitialized_copy(_M_start, position, newStart, __false_type());
        if (fillLength == 1) { new (newFinish) _Tp(x); ++newFinish; }
        else newFinish = __uninitialized_fill_n(newFinish, fillLength, x, __false_type());
        if (!atEnd) newFinish = __uninitialized_copy(position, _M_finish, newFinish, __false_type());
        _Destroy(_M_start, _M_finish);
        if (_M_start) std::free(_M_start);
        _M_start = newStart; _M_finish = newFinish; _M_end_of_storage = newStart + length;
    }
    void _M_fill_insert(_Tp* __position, unsigned int __n, const _Tp& __x)
    {
        if (__n != 0) {
            if ((unsigned int)(_M_end_of_storage - _M_finish) >= __n) {
                _Tp __x_copy = __x;
                const unsigned int __elems_after = _M_finish - __position;
                _Tp* __old_finish = _M_finish;
                if (__elems_after > __n) {
                    __uninitialized_copy(_M_finish - __n, _M_finish, _M_finish, __false_type());
                    _M_finish += __n;
                    __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                    fill(__position, __position + __n, __x_copy);
                } else {
                    __uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy, __false_type());
                    _M_finish += __n - __elems_after;
                    __uninitialized_copy(__position, __old_finish, _M_finish, __false_type());
                    _M_finish += __elems_after;
                    fill(__position, __old_finish, __x_copy);
                }
            } else
                _M_insert_overflow(__position, __x, __false_type(), __n);
        }
    }
    vector<_Tp>& operator=(const vector<_Tp>& __x)
    {
        if (&__x != this) {
            const unsigned int __xlen = __x._M_finish - __x._M_start;
            if (__xlen > (unsigned int)(_M_end_of_storage - _M_start)) {
                _Tp* __tmp = __xlen ? (_Tp*)std::malloc(__xlen * sizeof(_Tp)) : 0;
                __uninitialized_copy(__x._M_start, __x._M_finish, __tmp, __false_type());
                _Destroy(_M_start, _M_finish);
                if (_M_start) std::free(_M_start);
                _M_start = __tmp; _M_end_of_storage = _M_start + __xlen;
            } else if ((unsigned int)(_M_finish - _M_start) >= __xlen) {
                _Tp* __i = _M_start;
                for (const _Tp* s = __x._M_start; s != __x._M_finish; ++s, ++__i) *__i = *s;
                _Destroy(__i, _M_finish);
            } else {
                unsigned int sz = _M_finish - _M_start;
                for (unsigned int k = 0; k < sz; ++k) _M_start[k] = __x._M_start[k];
                __uninitialized_copy(__x._M_start + sz, __x._M_finish, _M_finish, __false_type());
            }
            _M_finish = _M_start + __xlen;
        }
        return *this;
    }
};

static void Init(vector<T>& v, int size, int cap)
{
    v._M_start = cap ? (T*)std::malloc(cap * sizeof(T)) : 0;
    for (int i = 0; i < size; ++i) new (v._M_start + i) T(Make(i * 10 + 1));
    v._M_finish = v._M_start + size; v._M_end_of_storage = v._M_start + cap;
}
static void Kill(vector<T>& v) { _Destroy(v._M_start, v._M_finish); std::free(v._M_start); }
static int Check(vector<T>& v, int size, int pos, int n, int fillv)
{
    if ((int)(v._M_finish - v._M_start) != size + n) return 0;
    for (int i = 0; i < size + n; ++i) {
        int expect = i < pos ? i * 10 + 1 : (i < pos + n ? fillv : (i - n) * 10 + 1);
        if (!Eq(v._M_start[i], Make(expect))) return 0;
    }
    return 1;
}

static int Run(int size, int cap, int pos, int n)
{
    vector<T> v; Init(v, size, cap);
    T x = Make(777);
    v._M_fill_insert(v._M_start + pos, n, x);
    int ok = Check(v, size, pos, n, 777);
    Kill(v);
    return ok;
}
int main()
{
    int ok = 1;
    ok &= Run(5, 10, 1, 2);
    ok &= Run(5, 10, 3, 4);
    ok &= Run(5, 10, 5, 3);
    ok &= Run(5, 10, 2, 0);
    int before = g_overflow_calls;
    ok &= Run(5, 6, 1, 3);
    if (g_overflow_calls != before + 1) ok = 0;
    if (g_live != 0) ok = 0;
    for (int i = 0; i < 2048; ++i) if (g_prims[i].RefCount != 0) ok = 0;
    if (!ok) { std::printf("00442350_TEST FAIL\n"); return 1; }
    std::printf("00442350_TEST PASS\n");
    return 0;
}