// std::vector<std::pair<CCharString, C3DMeshVolume>>::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend) -- STLport growth insert for the 16-byte named mesh-volume entries: allocate size + max(size, fill_len), copy the head, x once or fill_len times, the tail unless atend, destroy + free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@U?$pair@VCCharString@@VC3DMeshVolume@@@std@@V?$allocator@U?$pair@VCCharString@@VC3DMeshVolume@@@std@@@2@@std@@IAEXPAU?$pair@VCCharString@@VC3DMeshVolume@@@2@ABU32@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A72DD0, 262 bytes, thiscall `ret 14h` (five arguments).
// Identity (probable): the element is pair<CCharString, C3DMeshVolume> -- CPhysicsMesh::ContainmentVolumes' CVectorMap entry
//   (Ego_r.pdb), the type the landed callees 0x00A6FED0 (__uninitialized_copy) and 0x00A70EE0 (the destroy loop) model, and
//   this row sits in the CPhysicsMesh TU. The landed push_back 0x00A73530 binds this address under an invented one-argument
//   `vector<pair<CCharString, SNamedChunk> >` with a public member; STLport 4.6 declares _M_insert_overflow protected in
//   the two-argument vector<_Tp, _Alloc> (see NOTES.md, Review at landing).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14; __uninitialized_copy 0x00A6FED0 (head and tail) and
//   __uninitialized_fill_n 0x00A6FF40 (both __fastcall, landed); __destroy_aux 0x00A70EE0 (the inlined _M_clear's
//   _Destroy; __fastcall first/last + the tag reference); CCharString copy ctor 0x0099EC30 (the single x, inline).
// Shape: STLport 4.6 vector.c verbatim (no EH), the 0x00A75010 model with _M_clear inline (_Destroy + deallocate).
//   The empty tags are homed on the atend argument slot because the copy loops are same-TU noinline bodies.
inline void* operator new(unsigned int, void* p) { return p; }
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
};

struct SCountBlock {
    long m_references;                                     // +0
    void (__fastcall* m_pfnDestroy)(void*);                // +4
    void* m_pObject;                                       // +8
};

template <class T>
class CCountedPointer {
public:
    T* m_pObject;                                          // +0
    SCountBlock* m_pCount;                                 // +4
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount) ++m_pCount->m_references;
    }
};

class CContainmentVolume;

class C3DMeshVolume {                                      // 12 bytes (Ego_r.pdb), implicit copy
public:
    CCharString                         Name;              // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;      // +0x04
};

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        __first->first.PStringData = 0;
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }

template <class T> class allocator { };
template <class T, class _Alloc = allocator<T> > class vector {
public:
    typedef T* pointer;
    typedef unsigned int size_type;
    T* _M_start;                                           // +0x00
    T* _M_finish;                                          // +0x04
    T* _M_end_of_storage;                                  // +0x08
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    T* allocate(size_type __n) { return __n != 0 ? (T*)malloc(__n * sizeof(T)) : 0; }
    void deallocate(T* __p) { if (__p != 0) free(__p); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        deallocate(this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage = __e;
    }
protected:
    void _M_insert_overflow(pointer __position, const T& __x, const __false_type&, size_type __fill_len, bool __atend);
};
template <class T, class _Alloc>
void vector<T, _Alloc>::_M_insert_overflow(pointer __position, const T& __x, const __false_type&, size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = allocate(__len);
    pointer __new_finish = __new_start;
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
}

typedef std::pair<CCharString, C3DMeshVolume> SStringKeyedChunk;    // 0x10 bytes
template void std::vector<SStringKeyedChunk>::_M_insert_overflow(SStringKeyedChunk*, const SStringKeyedChunk&, const std::__false_type&, unsigned int, bool);