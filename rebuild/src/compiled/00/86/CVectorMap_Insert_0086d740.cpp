// CVectorMap::Insert(const long&, const CRandomAppearanceMorph::CSkeletalMorph&) -- append the (key, value) pair to the skeletal-morph vector map and mark it unsorted
// Leaf: ?Insert@?$CVectorMap@JVCSkeletalMorph@CRandomAppearanceMorph@@
// Retail 0x0086D740, 0x7F bytes (+ int3 padding to 0x90), thiscall `ret 8`. FableWin name (Dinkumware build):
// ?Insert@?$CVectorMap@JVCSkeletalMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@JVCSkeletalMorph@CRandomAppearanceMorph@@@@@@QAEXABJABVCSkeletalMorph@CRandomAppearanceMorph@@@Z
// (the same-shaped C3DMeshVolume map's Insert is in the name sets too). No direct caller (out-of-line instantiation).
// Callee: vector<pair<long, CSkeletalMorph> >::_M_insert_overflow(pos, x, __false_type, 1, true) @ 0x0086CC50 (same-TU
// STLport stand-in below, its own row in this chunk: with only a declaration the pair temporary is struct-copied through
// eax/edx, reloaded from the frame and `add dword ptr [esi+4],10h` is emitted).
// Shape: `Vector.push_back(pair(key, value)); Dirty = true;` -- the pair temporary is built in the frame (key dword, then the
// 12-byte value), the inline STLport push_back placement-copies it (the pair's user copy constructor: key, then the POD
// second struct-copied through `add eax,4`), and the Dirty store is duplicated into both push_back tails.
// Layout (retail CVectorMap): STLport vector +0x00 (12 bytes), empty Compare +0x0C, bool Dirty +0x0D.
// Manifest label "Vector_PushBack_4dword" is a descriptive auto label.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E
extern "C" void __cdecl free(void* block);           // 0x00BFEA14

class CDefString
{
public:
    long TablePos;                                   // +0x00
};

namespace std {
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
}

template <class K, class V> class CKeyPairCompareLess
{
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

template <class K, class V, class C> class CVectorMap
{
public:
    typedef std::pair<K, V> value_type;
    std::vector<value_type> Vector;                 // +0x00
    C                        Compare;                // +0x0C
    bool                     Dirty;                  // +0x0D

    void Insert(const K& key, const V& value);
};

template <class K, class V, class C> void CVectorMap<K, V, C>::Insert(const K& key, const V& value)
{
    Vector.push_back(value_type(key, value));
    Dirty = true;
}


class CRandomAppearanceMorph
{
public:
    class CSkeletalMorph                             // 12 bytes
    {
    public:
        long       GroupID;                          // +0x00
        CDefString Name;                             // +0x04
        bool       AllowVariation;                   // +0x08
    };
};

template void CVectorMap<long, CRandomAppearanceMorph::CSkeletalMorph,
    CKeyPairCompareLess<long, CRandomAppearanceMorph::CSkeletalMorph> >::Insert(const long&, const CRandomAppearanceMorph::CSkeletalMorph&);