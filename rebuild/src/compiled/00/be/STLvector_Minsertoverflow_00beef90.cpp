// std::vector::_M_insert_overflow(CSurfaceEntry* position, const CSurfaceEntry& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport non-POD growth of the CVectorMap<long, CCompositeSurfaceInfo> entry vector: allocate old + max(old, n), copy the head, place the n copies, copy the tail, destroy and free the old block
// Leaf: ?_M_insert_overflow@?$vector@U?$pair@JVCCompositeSurfaceInfo@@@std@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEEF90, 231 bytes, `__fastcall`, `ret 14h`.
// Callees: malloc @ 0x00BFEA0E (inline allocate), __uninitialized_copy @ 0x00BEDB60 (twice) and __uninitialized_fill_n
// @ 0x00BEDBB0 (both landed; same-TU bodies here), __destroy_aux @ 0x00BA9FD0 (the inline _Destroy's loop, a folded
// COMDAT), free @ 0x00BFEA14 (inline deallocate). Caller: CVectorMap<long, CCompositeSurfaceInfo>::Add @ 0x00BEF440.
// Label: the manifest's "CCompositeSurfaceMap_Reallocate" is descriptive.
// Element: see 0x00BED790. Shape: STLport 4.x verbatim (as the landed vector<CQuake> twin 0x0069D300): max by const
// reference (`lea ecx,[esp+24h] ; jb ; lea ecx,[esp+10h]`), the tag temporaries homed in the atend slot, the fill_len
// == 1 arm's _Construct inline (null-checked placement copy).
#include <new>
#include <stddef.h>

// CBaseClass: the polymorphic engine root (out-of-line virtual destructor 0x0099A2E0, vtable 0x01231710).
class CBaseClass {
public:
    virtual ~CBaseClass();                          // 0x0099A2E0
};

class CCPPointerInfo {                              // the shared count block of a CCountedPointer
public:
    long  RefCount;                                 // +0x00
    void (__fastcall* DeleteFunction)(void* data);  // +0x04
    void* Data;                                     // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                           // +0x00
    CCPPointerInfo* Info;                           // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { DeleteData(); Data = 0; Info = 0; }
    void DeleteData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunction(Info->Data);
                operator delete(Info);
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            DeleteData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};

class CCompositeSurfaceCache;
// Ego_r.pdb CCompositeSurfaceInfo (vtable 0x012A8154, 12 bytes): implicit copy, assignment and (virtual) destructor.
class CCompositeSurfaceInfo : public CBaseClass {
public:
    CCountedPointer<CCompositeSurfaceCache> Cache;  // +0x04
};

namespace std {
struct __false_type { __false_type() { } };
struct __true_type { __true_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class T1, class T2> struct pair {
    T1 first;                                       // +0x00
    T2 second;                                      // +0x04
};
}   // namespace std

typedef std::pair<long, CCompositeSurfaceInfo> CSurfaceEntry;   // 0x10 bytes

// The vector map's key order (an empty functor passed by value).
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<long, CCompositeSurfaceInfo> CSurfaceEntryLess;

extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14

namespace std {
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class ForwardIter>
void __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&);   // 0x00BA9FD0 (folded COMDAT)
template <class ForwardIter, class T>
inline void __destroy(ForwardIter first, ForwardIter last, T*) { __destroy_aux(first, last, __false_type()); }
template <class ForwardIter>
inline void _Destroy(ForwardIter first, ForwardIter last) { __destroy(first, last, (CSurfaceEntry*)0); }

template <class InputIter, class ForwardIter>
__declspec(noinline) ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result,
                                                      const __false_type&)   // 0x00BEDB60 (same-TU body)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class ForwardIter, class Size, class T>
__declspec(noinline) ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& x,
                                                        const __false_type&)   // 0x00BEDBB0 (same-TU body)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

template <class T> class allocator {
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class Value, class T, class Alloc> class _STLP_alloc_proxy : public Alloc {
public:
    Value _M_data;
};
template <class T, class Alloc> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;
};

template <class T, class Alloc> class vector : public _Vector_base<T, Alloc> {
public:
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer s, pointer f, pointer e)
    {
        this->_M_start = s;
        this->_M_finish = f;
        this->_M_end_of_storage._M_data = e;
    }
    pointer _M_allocate_and_copy(size_type n, const_pointer first, const_pointer last)
    {
        pointer result = this->_M_end_of_storage.allocate(n);
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
    void reserve(size_type n);
    void _M_insert_overflow(pointer position, const T& x, const __false_type&, size_type fill_len, bool atend);
    void push_back(const T& x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, x, __false_type(), 1UL, true);
    }
};
}   // namespace std

typedef std::vector<CSurfaceEntry, std::allocator<CSurfaceEntry> > CSurfaceEntryVector;

template <class T, class Alloc>
void std::vector<T, Alloc>::_M_insert_overflow(pointer position, const T& x, const __false_type&, size_type fill_len,
                                                bool atend)
{
    const size_type old_size = size();
    const size_type len = old_size + (max)(old_size, fill_len);
    pointer new_start = this->_M_end_of_storage.allocate(len);
    pointer new_finish = new_start;
    new_finish = __uninitialized_copy(this->_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    }
    else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy(position, this->_M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}

template void CSurfaceEntryVector::_M_insert_overflow(CSurfaceEntry*, const CSurfaceEntry&, const std::__false_type&,
                                                      unsigned int, bool);