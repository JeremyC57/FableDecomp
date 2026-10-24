// std::sort_heap(SDefClassInfoEntry* first, SDefClassInfoEntry* last, CKeyPairCompareLess comp) -- SGI sort_heap over the definition manager's (CRC, CDefClassInfo) vector map: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B8CA0, length 233 bytes, `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned long,C3DHelperPoint>*,...>` is propagated over the
// byte-identical skeleton. Stride 0x18 (0x2AAAAAAB / sar 2); the callee 0x009B8770 (manifest `_Pop_heap<CTestQuest*,
// ...>`, also propagated) sits among the CDefClassInfo map's algorithm instantiations (0x009B77B0 _Push_heap,
// 0x009B8220 _Adjust_heap of pair<unsigned long, CDefinitionManager::CDefClassInfo>), and the inline element copy
// is exactly that pair: key, Name (CCharString copy), AllocFunc, GlobalIndices (STLport vector<unsigned long> copy:
// zero, malloc(n*4), memmove). Layout from the landed 0x009B8070 / 0x009B0AC0 sources (Ego_r.pdb names).
// Callees: __pop_heap @ 0x009B8770 (out of line, __fastcall), CCharString copy constructor @ 0x0099EC30,
// malloc @ 0x00BFEA0E, memmove @ 0x00BFEAE6.
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline; the value copy is built in the argument area by the implicit pair / CDefClassInfo copy
// constructors with the real STLport vector copy inline (`_Vector_base(n, a)`: three zero stores, allocate,
// start/finish/end-of-storage, then the POD `__uninitialized_copy` -> `__copy_trivial` memmove). Vector model of
// the landed 0x00A85420 source.
extern "C" void* __cdecl malloc(unsigned int size);                                    // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);     // 0x00BFEAE6

struct __true_type { __true_type() { } };

inline unsigned long* __uninitialized_copy(const unsigned long* first, const unsigned long* last, unsigned long* result, const __true_type&)
{
    return (last == first) ? result
        : (unsigned long*)((char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first));
}

template <class T> struct allocator {
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    __forceinline _Vector_base(unsigned int n, const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
};
template <class T> struct vector : _Vector_base<T> {
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    allocator<T> get_allocator() const { return allocator<T>(); }
    __forceinline vector(const vector& x) : _Vector_base<T>(x.size(), x.get_allocator())
    {
        __true_type tag;
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, tag);
    }
};
template <class T> struct CArray : vector<T> { };

struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);             // 0x0099EC30
};
struct CDefClassBase;
struct CDefinitionManager {
    struct CDefClassInfo {                      // 0x14 bytes in retail
        CCharString Name;                       // +0x00
        CDefClassBase* (__cdecl* AllocFunc)();  // +0x04
        CArray<unsigned long> GlobalIndices;    // +0x08
    };
};
struct SDefClassInfoEntry {                     // std::pair<unsigned long, CDefinitionManager::CDefClassInfo> (0x18 bytes)
    unsigned long                     first;    // +0x00 CRC of the class name
    CDefinitionManager::CDefClassInfo second;   // +0x04
};
struct CKeyPairCompareLess {                    // empty, passed by value
    bool operator()(const SDefClassInfoEntry& a, const SDefClassInfoEntry& b) const { return a.first < b.first; }
};

void __fastcall __pop_heap(SDefClassInfoEntry* first, SDefClassInfoEntry* last, SDefClassInfoEntry* result,
                           SDefClassInfoEntry value, CKeyPairCompareLess comp, int*);   // 0x009B8770

inline void __pop_heap_aux(SDefClassInfoEntry* first, SDefClassInfoEntry* last, SDefClassInfoEntry*,
                           CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, SDefClassInfoEntry(*(last - 1)), comp, (int*)0);
}

inline void pop_heap(SDefClassInfoEntry* first, SDefClassInfoEntry* last, CKeyPairCompareLess comp)
{
    __pop_heap_aux(first, last, (SDefClassInfoEntry*)0, comp);
}

void __fastcall sort_heap(SDefClassInfoEntry* first, SDefClassInfoEntry* last, CKeyPairCompareLess comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}