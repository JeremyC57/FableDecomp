#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

// Test model of the chunk's STLport 4.6 algorithm instantiations: string stand-ins that own a malloc'd copy and count
// their live instances, a malloc-backed vector, and the STLport algorithm bodies the sources reconstruct.
static int g_live = 0;      // live CCharString / CWideString objects
static int g_blocks = 0;    // live vector buffers

inline char* DupText(const char* s)
{
    if (!s)
        return 0;
    unsigned int n = (unsigned int)std::strlen(s) + 1;
    char* d = (char*)std::malloc(n);
    std::memcpy(d, s, n);
    return d;
}

class CCharString {
public:
    CCharString() : PStringData(0) { ++g_live; }
    CCharString(const char* s) : PStringData(DupText(s)) { ++g_live; }
    CCharString(const CCharString& other) : PStringData(DupText(other.PStringData)) { ++g_live; }
    ~CCharString() { std::free(PStringData); --g_live; }
    CCharString& operator=(const CCharString& other)
    {
        if (this != &other) {
            char* d = DupText(other.PStringData);
            std::free(PStringData);
            PStringData = d;
        }
        return *this;
    }
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData) return false;
        if (!PStringData) return true;
        if (!other.PStringData) return false;
        return std::strcmp(PStringData, other.PStringData) < 0;
    }
    bool Is(const char* s) const { return PStringData != 0 && std::strcmp(PStringData, s) == 0; }
    char* PStringData;
};

class CWideString {
public:
    CWideString() : PStringData(0) { ++g_live; }
    CWideString(const char* s) : PStringData(DupText(s)) { ++g_live; }
    CWideString(const CWideString& other) : PStringData(DupText(other.PStringData)) { ++g_live; }
    ~CWideString() { std::free(PStringData); --g_live; }
    CWideString& operator=(const CWideString& other)
    {
        if (this != &other) {
            char* d = DupText(other.PStringData);
            std::free(PStringData);
            PStringData = d;
        }
        return *this;
    }
    bool operator<(const CWideString& other) const
    {
        if (other.PStringData == PStringData) return false;
        if (!PStringData) return true;
        if (!other.PStringData) return false;
        return std::strcmp(PStringData, other.PStringData) < 0;
    }
    bool Is(const char* s) const { return PStringData != 0 && std::strcmp(PStringData, s) == 0; }
    char* PStringData;
};

namespace std {
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(), second() {}
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __p) { __p->~_Tp(); }

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(const _Tp*) { return (int*)0; }

template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n)
    {
        if (!__n)
            return 0;
        ++g_blocks;
        return (_Tp*)std::malloc(__n * sizeof(_Tp));
    }
    void deallocate(_Tp* __p, unsigned int)
    {
        if (__p) {
            --g_blocks;
            std::free(__p);
        }
    }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef __false_type _IsPODType;
    vector() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    vector(const vector<_Tp, _Alloc>& __x)
    {
        _M_start = _M_end_of_storage.allocate(__x.size());
        _M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, _M_start, _IsPODType());
        _M_end_of_storage._M_data = _M_start + __x.size();
    }
    ~vector() { _M_clear(); }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x)
    {
        if (this != &__x) {
            vector<_Tp, _Alloc> __tmp(__x);
            _Tp* __s = _M_start; _M_start = __tmp._M_start; __tmp._M_start = __s;
            _Tp* __f = _M_finish; _M_finish = __tmp._M_finish; __tmp._M_finish = __f;
            _Tp* __e = _M_end_of_storage._M_data;
            _M_end_of_storage._M_data = __tmp._M_end_of_storage._M_data;
            __tmp._M_end_of_storage._M_data = __e;
        }
        return *this;
    }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage._M_data - _M_start); }
    _Tp& operator[](unsigned int __i) { return _M_start[__i]; }
    const _Tp& operator[](unsigned int __i) const { return _M_start[__i]; }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false)
    {
        unsigned int __old_size = size();
        unsigned int __len = __old_size + (__old_size > __fill_len ? __old_size : __fill_len);
        _Tp* __new_start = _M_end_of_storage.allocate(__len);
        _Tp* __new_finish = __uninitialized_copy(_M_start, __position, __new_start, _IsPODType());
        if (__fill_len == 1) {
            _Construct(__new_finish, __x);
            ++__new_finish;
        }
        else
            __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, _IsPODType());
        if (!__atend)
            __new_finish = __uninitialized_copy(__position, _M_finish, __new_finish, _IsPODType());
        _M_clear();
        _M_start = __new_start;
        _M_finish = __new_finish;
        _M_end_of_storage._M_data = __new_start + __len;
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
private:
    void _M_clear()
    {
        for (_Tp* __p = _M_start; __p != _M_finish; ++__p)
            _Destroy(__p);
        _M_end_of_storage.deallocate(_M_start, capacity());
        _M_start = _M_finish = _M_end_of_storage._M_data = 0;
    }
};
}

template <class T> class CArray : public std::vector<T> {      // retail CArray: the 12-byte STLport vector
};

class CDefClassBase;
class CDefinitionManager {
public:
    class CDefClassInfo {
    public:
        CDefClassInfo() : AllocFunc(0) {}
        CCharString            Name;
        CDefClassBase* (*AllocFunc)();
        CArray<unsigned long>  GlobalIndices;
    };
};
typedef std::pair<unsigned long, CDefinitionManager::CDefClassInfo> CDefClassInfoPair;

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo> CDefClassInfoLess;

template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    CVectorMap() : Dirty(false) {}
    void Insert(const K& key, const V& value);
    C    Compare;
    bool Dirty;
};

template <class K, class V, class C> void CVectorMap<K, V, C>::Insert(const K& key, const V& value)
{
    this->push_back(std::pair<K, V>(key, value));
    Dirty = true;
}

namespace std {
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    swap(*__i1, *__i2);
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

template <class _RandomAccessIter, class _Compare>
void __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last)
        return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2)
        return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;

    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0)
            return;
        __parent--;
    }
}

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare>
inline void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, __distance_type(__first));
}

template <class _RandomAccessIterator, class _Compare>
inline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, __value_type(__first), __comp);
}

template <class _RandomAccessIterator, class _Compare>
void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}

template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, __value_type(__first), __comp);
}

template <class _Tp, class _Compare>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > 16) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}
}

// Fill a (key, class info) pair: name, and `count` global indices starting at `index0`.
inline void SetInfo(CDefClassInfoPair& p, unsigned long key, const char* name, unsigned long index0, unsigned int count)
{
    p.first = key;
    p.second.Name = CCharString(name);
    p.second.GlobalIndices = CArray<unsigned long>();
    for (unsigned int i = 0; i < count; ++i)
        p.second.GlobalIndices.push_back(index0 + i);
}

// The pair holds exactly what SetInfo(p, key, name, index0, count) stored.
inline bool InfoIs(const CDefClassInfoPair& p, unsigned long key, const char* name, unsigned long index0, unsigned int count)
{
    if (p.first != key || !p.second.Name.Is(name) || p.second.GlobalIndices.size() != count)
        return false;
    for (unsigned int i = 0; i < count; ++i)
        if (p.second.GlobalIndices[i] != index0 + i)
            return false;
    return true;
}

// __partial_sort over pair<unsigned long, long> by key (CKeyPairCompareLess): the smallest keys, sorted, with their values.
typedef std::pair<unsigned long, long> KeyPair;

int main()
{
    unsigned long keys[8] = { 9, 0x80000000UL, 7, 1, 0xFFFFFFF0UL, 2, 6, 3 };   // unsigned order: high keys sort last
    KeyPair a[8];
    for (int i = 0; i < 8; ++i) {
        a[i].first = keys[i];
        a[i].second = (long)(100 - (int)i);
    }
    std::__partial_sort(a, a + 4, a + 8, (KeyPair*)0, CKeyPairCompareLess<unsigned long, long>());
    unsigned long want[4] = { 1, 2, 3, 6 };
    for (int i = 0; i < 4; ++i)
        if (a[i].first != want[i]) { std::printf("009b6b70_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 8; ++i)                     // values travel with their keys
        for (int j = 0; j < 8; ++j)
            if (a[j].first == keys[i] && a[j].second != (long)((long)(100 - (int)i))) { std::printf("009b6b70_TEST FAIL\n"); return 1; }
    std::__partial_sort(a, a + 8, a + 8, (KeyPair*)0, CKeyPairCompareLess<unsigned long, long>());
    unsigned long all[8] = { 1, 2, 3, 6, 7, 9, 0x80000000UL, 0xFFFFFFF0UL };
    for (int i = 0; i < 8; ++i)
        if (a[i].first != all[i]) { std::printf("009b6b70_TEST FAIL\n"); return 1; }
    std::printf("009b6b70_TEST PASS\n");
    return 0;
}