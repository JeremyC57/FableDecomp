// CVectorMap::Add(const CLongMapValue8&) -- insert the value under its own first field as the key and re-sort the long-keyed map (descriptive name)
// Leaf: ?Add@?$CVectorMap@JVCLongMapValue8@@
// Retail 0x0086F600, 0x72 bytes (+ int3 padding to 0x80), thiscall `ret 4` (`this` is the map). No direct caller.
// Callees: vector<pair<long, V> >::_M_insert_overflow(pos, x, __false_type, 1, true) @ 0x0086FC00 (same-TU STLport
// stand-in below: with only a declaration the pair temporary is spilled and reloaded instead of kept in ecx/edx/edi),
// _STL::sort<pair<long, V>*, CKeyPairCompareLess<long, V> > @ 0x008701D0 (__fastcall first/last, the empty comparator
// pushed as a zero-extended byte; its insertion passes are 0x0086F3E0 / 0x0086FA00, signed key).
// Element: pair<long, 8-byte value> (0x0C). The value type is NOT identified (FableWin/Ego_r long-keyed maps of 8-byte
// values: CTCHeroMorph::CTextureLayer, CMusicManager::CRegisteredMusicEntry; neither tied to this code by a second
// source), and the PDB name sets hold no one-argument CVectorMap member: CLongMapValue8 and Add are descriptive names.
// Shape: `Insert(value.Key, value); Sort();` -- the key is the value's first dword (`mov ecx,[eax] ; mov edx,ecx`), the
// inline STLport push_back placement-copies the pair temporary from registers, then Dirty = true, sort, Dirty = false.
// Layout (retail CVectorMap): STLport vector +0x00 (12 bytes), empty Compare +0x0C, bool Dirty +0x0D.
// Manifest label "PriorityQueue_PushSift_0086f600" is a descriptive auto label.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E
extern "C" void __cdecl free(void* block);           // 0x00BFEA14

namespace _STL {
struct __false_type { __false_type() { } };          // user constructor: the tag temporary is not zero-stored

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> struct pair
{
    _T1 first;                                       // +0x00
    _T2 second;                                      // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's explicit pair copy constructor
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIter> inline void _Destroy(_ForwardIter __first, _ForwardIter __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

// Same-TU stand-ins for the out-of-line /Gr helpers (they never touch their tag, so the caller homes each __false_type
// temporary on an argument slot as retail does).
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)   // 0x0086ABF0
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)   // 0x0086AC40
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline()

template <class _Tp> class allocator
{
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Tp> class vector                    // STLport vector<_Tp, allocator<_Tp> >, 12 bytes
{
public:
    _Tp* _M_start;                                   // +0x00
    _Tp* _M_finish;                                  // +0x04
    _Tp* _M_end_of_storage;                          // +0x08

    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        allocator<_Tp>().deallocate(this->_M_start, (unsigned int)(this->_M_end_of_storage - this->_M_start));
    }
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage = __e;
    }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);
    _Tp* begin() { return this->_M_start; }
    _Tp* end() { return this->_M_finish; }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

#pragma auto_inline(off)
template <class _Tp>
__declspec(noinline) void vector<_Tp>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    _Tp* __new_start = allocator<_Tp>().allocate(__len);
    _Tp* __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
#pragma auto_inline()

template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x008701D0
}

template <class K, class V> class CKeyPairCompareLess
{
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

template <class K, class V, class C> class CVectorMap
{
public:
    typedef _STL::pair<K, V> value_type;
    _STL::vector<value_type> Vector;                 // +0x00
    C                        Compare;                // +0x0C
    bool                     Dirty;                  // +0x0D

    __forceinline void Insert(const K& key, const V& value)
    {
        Vector.push_back(value_type(key, value));
        Dirty = true;
    }
    void Sort()
    {
        _STL::sort(Vector.begin(), Vector.end(), Compare);
        Dirty = false;
    }
    void Add(const V& value);
};


template <class K, class V, class C> void CVectorMap<K, V, C>::Add(const V& value)
{
    Insert(value.Key, value);
    Sort();
}

class CLongMapValue8                                 // 8 bytes (descriptive)
{
public:
    long          Key;                               // +0x00
    unsigned long Data;                              // +0x04
};

template void CVectorMap<long, CLongMapValue8, CKeyPairCompareLess<long, CLongMapValue8> >::Add(const CLongMapValue8&);