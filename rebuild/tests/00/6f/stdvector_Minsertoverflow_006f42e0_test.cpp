// behaviour test for 0x006F42E0 std::vector<CCountedPointer<CCombatSequenceBase>>::_M_insert_overflow (__false_type form): head copy, single construct or n-fold fill, tail copy unless atend, growth to size + max(size, n), old elements released and old block freed, every live copy holding one reference
#include <cstdio>
#include <cstdlib>
#include <stddef.h>
inline void* operator new(size_t, void* p, int) { return p; }
class CCPPointerInfo {
public:
    unsigned int RefCount;
    void* DeleteFunc;
    void* Data;
};
class CPointee { public: int Key; };
class CCountedPointer {
public:
    CPointee* Data;
    CCPPointerInfo* Info;
    CCountedPointer(CPointee* data, CCPPointerInfo* info) : Data(data), Info(info) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info)
            --Info->RefCount;
        Data = 0;
        Info = 0;
    }
};
static CPointee gObjects[6];
static CCPPointerInfo gInfos[6];
static CCountedPointer MkCP(int k) { return CCountedPointer(&gObjects[k], &gInfos[k]); }

namespace stlp_test {
struct __false_type { __false_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
static int gFrees = 0;
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p, 0) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur) _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIterator> void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)std::malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) { ++gFrees; std::free(__p); } }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start; _Tp* _M_finish; _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e) { this->_M_start = __s; this->_M_finish = __f; this->_M_end_of_storage._M_data = __e; }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false)
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);
        pointer __new_start = this->_M_end_of_storage.allocate(__len);
        pointer __new_finish = __new_start;
        __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
        if (__fill_len == 1) { _Construct(__new_finish, __x); ++__new_finish; }
        else __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
        if (!__atend)
            __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
};
}
typedef CCountedPointer T;
static const CCountedPointer& cp(const T& t) { return t; }
static int key(const T& t) { return cp(t).Data ? cp(t).Data->Key : -1; }
class CProbe : public stlp_test::vector<T> {
public:
    CProbe() { this->_M_start = 0; this->_M_finish = 0; this->_M_end_of_storage._M_data = 0; }
    ~CProbe() { stlp_test::_Destroy(this->_M_start, this->_M_finish); std::free(this->_M_start); }
    void Overflow(unsigned int pos, const T& x, unsigned int n, bool atend)
    {
        _M_insert_overflow(this->_M_start + pos, x, stlp_test::__false_type(), n, atend);
    }
    unsigned int Cap() const { return (unsigned int)(this->_M_end_of_storage._M_data - this->_M_start); }
    int Count(int k) const { int c = 0; for (const T* p = this->_M_start; p != this->_M_finish; ++p) if (key(*p) == k) ++c; return c; }
};
static int RefsOk(const CProbe& v)
{
    for (int k = 1; k <= 5; ++k)
        if (gInfos[k].RefCount != (unsigned int)(1 + v.Count(k))) return 0;
    return 1;
}
static int Check()
{
    for (int k = 0; k < 6; ++k) { gObjects[k].Key = k; gInfos[k].RefCount = 1; gInfos[k].DeleteFunc = 0; gInfos[k].Data = &gObjects[k]; }
    T e1 = MkCP(1), e2 = MkCP(2), e3 = MkCP(3), e4 = MkCP(4), e5 = MkCP(5);
    CProbe* v = new CProbe;
    v->Overflow(0, e1, 1, true);                           // empty: len = 0 + max(0, 1) = 1
    if (v->size() != 1 || v->Cap() != 1 || key(v->_M_start[0]) != 1 || stlp_test::gFrees != 0 || !RefsOk(*v)) return 1;
    v->Overflow(1, e2, 1, true);                           // push_back growth: len = 2, old block released and freed
    if (v->size() != 2 || v->Cap() != 2 || key(v->_M_start[1]) != 2 || stlp_test::gFrees != 1 || !RefsOk(*v)) return 2;
    v->Overflow(1, e3, 3, false);                          // middle fill of 3: len = 2 + max(2, 3) = 5
    int want[5] = { 1, 3, 3, 3, 2 };
    if (v->size() != 5 || v->Cap() != 5 || stlp_test::gFrees != 2 || !RefsOk(*v)) return 3;
    for (int i = 0; i < 5; ++i) if (key(v->_M_start[i]) != want[i]) return 4;
    v->Overflow(4, e4, 1, false);                          // single middle insert: len = 10
    int want2[6] = { 1, 3, 3, 3, 4, 2 };
    if (v->size() != 6 || v->Cap() != 10 || !RefsOk(*v)) return 5;
    for (int i = 0; i < 6; ++i) if (key(v->_M_start[i]) != want2[i]) return 6;
    v->Overflow(2, e5, 2, true);                           // atend: the tail is dropped (and released)
    if (v->size() != 4 || key(v->_M_start[2]) != 5 || key(v->_M_start[3]) != 5 || key(v->_M_start[1]) != 3 || !RefsOk(*v)) return 7;
    delete v;
    for (int k = 1; k <= 5; ++k) if (gInfos[k].RefCount != 1) return 8;   // only e1..e5 hold a reference now
    return 0;
}
int main()
{
    int r = Check();
    if (r != 0) { std::printf("006f42e0_TEST FAIL %d\n", r); return 1; }
    std::printf("006f42e0_TEST PASS\n");
    return 0;
}