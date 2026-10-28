// Behaviour test for 0x00412330 vector<CWideString>::_M_insert_overflow (STLport model): growth to old+max(old,n), head/fill/tail copies, old strings destroyed.

#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
class CWideString {
public:
    CWideString() : m_pData((unsigned short*)std::malloc(32)) { m_pData[0] = 0; ++g_live; }
    CWideString(const CWideString& o) : m_pData((unsigned short*)std::malloc(32)) { Copy(o); ++g_live; }
    const CWideString& operator=(const CWideString& o) { Copy(o); return *this; }
    ~CWideString() { std::free(m_pData); --g_live; }
    void Set(int v) { for (int i = 0; i < 15; ++i) m_pData[i] = (unsigned short)(v * 7 + i); m_pData[15] = 0; }
    bool Same(const CWideString& o) const { for (int i = 0; i < 16; ++i) if (m_pData[i] != o.m_pData[i]) return false; return true; }
private:
    void Copy(const CWideString& o) { for (int i = 0; i < 16; ++i) m_pData[i] = o.m_pData[i]; }
    unsigned short* m_pData;
};
typedef CWideString T;

static T Make(int v) { T t; t.Set(v); return t; }
static bool Eq(const T& a, const T& b) { return a.Same(b); }

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

static int Run(int size, int cap, int pos, int n, bool atEnd)
{
    vector<T> v; Init(v, size, cap);
    T x = Make(555);
    v._M_insert_overflow(v._M_start + pos, x, __false_type(), n, atEnd);
    int ok = atEnd ? ((int)(v._M_finish - v._M_start) == pos + n) : Check(v, size, pos, n, 555);
    if (atEnd) for (int i = 0; ok && i < pos + n; ++i) ok = Eq(v._M_start[i], Make(i < pos ? i * 10 + 1 : 555));
    int expectCap = size + (size < n ? n : size);
    if ((int)(v._M_end_of_storage - v._M_start) != expectCap) ok = 0;
    Kill(v);
    return ok;
}
int main()
{
    int ok = 1;
    ok &= Run(4, 4, 2, 1, false);
    ok &= Run(4, 4, 0, 6, false);
    ok &= Run(3, 3, 3, 1, true);
    ok &= Run(0, 0, 0, 2, false);
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("00412330_TEST FAIL\n"); return 1; }
    std::printf("00412330_TEST PASS\n");
    return 0;
}