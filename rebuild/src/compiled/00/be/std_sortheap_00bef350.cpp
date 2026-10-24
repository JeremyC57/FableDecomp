// std::sort_heap(SDecalEntry* first, SDecalEntry* last, CKeyPairCompareLess comp) -- SGI sort_heap over the (pair<long,long>, CEngineTextureDecal) decal table: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEF350, length 231 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned long,C3DHelperPoint>*,...>` is propagated over the
// byte-identical skeleton. Stride 0x1C (0x92492493 / add / sar 4); the callee 0x00BEEDA0 (manifest `_Cons_val<...
// pair<CCharString,C3DVector>>`, propagated) is the SGI `__pop_heap` of a 0x1C-byte pair: four dword moves and the
// blend vector's operator= 0x006CADE0, then __adjust_heap 0x00BEDD70. Its neighbours carry the manifest names
// `_Push_heap / _Adjust_heap / _Move_backward<pair<pair<long,long>,CEngineTextureDecal>*,...>`, and Ego_r.pdb
// CEngineTextureDecal = OriginalTextureIndex, DecalID, vector<CCompositeBlendDesc> OverlaidBlends (12-byte STLport
// vector in retail), so the element is pair<pair<long,long>, CEngineTextureDecal> (8 + 0x14).
// Callees: __pop_heap @ 0x00BEEDA0 (out of line, __fastcall), malloc @ 0x00BFEA0E, the blend descriptors'
// __uninitialized_copy @ 0x005BAF28 (__fastcall: first/last in ecx/edx, result and the __false_type tag on the
// stack; per-element CCompositeBlendDesc copy constructor 0x005BA48A).
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline. The value copy is the implicit pair copy: the two key dwords, OriginalTextureIndex and
// DecalID, then STLport's vector copy constructor inline (`_Vector_base(n, a)`: three zero stores, malloc(n*16) when
// n != 0, start/finish/end-of-storage, then the non-POD `__uninitialized_copy`). `push 0` for the Distance* shares
// eax with the vector's zero stores. __uninitialized_copy gets a same-TU __declspec(noinline) stand-in body so the
// empty `__false_type()` tag overlays the dead comparator slot (`lea ecx,[esp+40h]`) as retail (the 0x00A85420
// recipe). The key pair<long,long> has STLport's explicit memberwise copy constructor (`pair(const pair<_T1,_T2>&)`):
// with an implicit one the `lea ebp,[ebx-14h]` (last - 1) is scheduled two instructions early.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

struct __false_type { __false_type() { } };

struct CCompositeBlendDesc {                            // Ego_r.pdb, 16 bytes
    unsigned long TextureIndex;                         // +0x00
    long          BlendType;                            // +0x04 ECompositeBlendType
    unsigned long DiffuseColour;                        // +0x08 CRGBColour
    unsigned long SpecularColour;                       // +0x0c CRGBColour
    CCompositeBlendDesc(const CCompositeBlendDesc& other);   // 0x005BA48A
};

#pragma auto_inline(off)
// Same-TU body of the STLport __uninitialized_copy instantiation 0x005BAF28 (the tag is never read).
__declspec(noinline) CCompositeBlendDesc* __fastcall __uninitialized_copy(const CCompositeBlendDesc* first, const CCompositeBlendDesc* last,
                                                                          CCompositeBlendDesc* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) CCompositeBlendDesc(*first);
    return result;
}
#pragma auto_inline(on)

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
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, __false_type());
    }
};

struct CEngineTextureDecal {                            // 0x14 bytes in retail
    long                         OriginalTextureIndex;  // +0x00
    long                         DecalID;               // +0x04
    vector<CCompositeBlendDesc>  OverlaidBlends;        // +0x08
};
struct SLongPair {                                      // std::pair<long, long>
    SLongPair(const SLongPair& o) : first(o.first), second(o.second) { }
    long first;
    long second;
};
struct SDecalEntry {                                    // std::pair<pair<long,long>, CEngineTextureDecal> (0x1C bytes)
    SLongPair           first;                          // +0x00
    CEngineTextureDecal second;                         // +0x08
};
struct CKeyPairCompareLess { };                         // empty, passed by value

void __fastcall __pop_heap(SDecalEntry* first, SDecalEntry* last, SDecalEntry* result, SDecalEntry value,
                           CKeyPairCompareLess comp, int*);   // 0x00BEEDA0

inline void __pop_heap_aux(SDecalEntry* first, SDecalEntry* last, SDecalEntry*, CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, SDecalEntry(*(last - 1)), comp, (int*)0);
}

inline void pop_heap(SDecalEntry* first, SDecalEntry* last, CKeyPairCompareLess comp)
{
    __pop_heap_aux(first, last, (SDecalEntry*)0, comp);
}

void __fastcall sort_heap(SDecalEntry* first, SDecalEntry* last, CKeyPairCompareLess comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}