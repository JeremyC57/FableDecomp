// std::vector::reserve(unsigned int n) -- grow the CVectorMap<long, CCompositeSurfaceInfo> entry vector to capacity n: copy the entries into a fresh block, destroy and free the old one
// Leaf: ?reserve@?$vector@U?$pair@JVCCompositeSurfaceInfo@@@std@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEEB80, 186 bytes, `__fastcall`, `ret 4`.
// Callees: malloc @ 0x00BFEA0E (inline allocate), __uninitialized_copy @ 0x00BEDB60 (landed), __destroy_aux @ 0x00BA9FD0
// (the inline _Destroy's out-of-line loop, a folded COMDAT), free @ 0x00BFEA14 (inline deallocate).
// Label: the manifest's "Std_Vector_ReallocateForComposite" is descriptive; Ego_r.pdb name set
// ?reserve@?$vector@U?$pair@JVCCompositeSurfaceInfo@@@std@@...@QAEXI@Z (Dinkumware there, STLport in retail).
// Element: see 0x00BED790. Shape: STLport 4.6 reserve with _M_allocate_and_copy, _M_clear and _M_set inline; the old
// finish is kept in the `push ecx` frame word and the new block in the dead n slot (its address doubles as the tags').
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
ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&);   // 0x00BEDBB0

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
    protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    public:
    protected:
    void _M_set(pointer s, pointer f, pointer e)
    {
        this->_M_start = s;
        this->_M_finish = f;
        this->_M_end_of_storage._M_data = e;
    }
    public:
    protected:
    pointer _M_allocate_and_copy(size_type n, const_pointer first, const_pointer last)
    {
        pointer result = this->_M_end_of_storage.allocate(n);
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
    public:
    void reserve(size_type n);
    protected:
    void _M_insert_overflow(pointer position, const T& x, const __false_type&, size_type fill_len, bool atend);
    public:
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
void std::vector<T, Alloc>::reserve(size_type n)
{
    if (capacity() < n) {
        const size_type old_size = size();
        pointer tmp;
        if (this->_M_start) {
            tmp = _M_allocate_and_copy(n, this->_M_start, this->_M_finish);
            _M_clear();
        }
        else
            tmp = this->_M_end_of_storage.allocate(n);
        _M_set(tmp, tmp + old_size, tmp + n);
    }
}

template void CSurfaceEntryVector::reserve(unsigned int);