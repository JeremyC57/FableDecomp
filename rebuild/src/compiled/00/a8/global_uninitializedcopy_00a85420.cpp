// __uninitialized_copy(const SNavLayerRecord* first, const SNavLayerRecord* last, SNavLayerRecord* result, const __false_type&) -- placement-copy a range of 40-byte records (three STLport vectors -- of indices, of lists, of pairs inside a CVectorMap) and return the end
// Grade: RELOCATION_MATCH (retry helper C) -- the two element __uninitialized_copy callees (0x00A82730, 0x00A83340) that take the __false_type tag by const reference get same-TU noinline bodies; the tags then overlay the dead `result` slot as retail (was DIFFER(320v316), frame-only: tag bytes in their own frame words).
// Leaf: ?__uninitialized_copy@@YIPAUSNavLayerRecord@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A85420, 316 bytes (+ int3 padding), `__fastcall` (first ecx, last edx), `ret 8`.
// Callees: malloc 0x00BFEA0E, memmove 0x00BFEAE6 (the POD vector's __copy_trivial), the list-element
//   __uninitialized_copy 0x00A82730 (4-byte STLport list<16-byte value>, landed as UninitCopy_List16_00a82730) and the
//   16-byte-pair __uninitialized_copy 0x00A83340 (both `__fastcall`, tag by reference).
// Fixed (rename1, 2026-10-24): the middle vector's element was modelled as a 4-byte int (`SNavCell`) whose same-TU
//   __uninitialized_copy stand-in copied one word; 0x00A82730 placement copy-constructs STLport lists (fresh 0x18-byte
//   self-linked header via malloc, then _M_insert_dispatch 0x005152C0 of the source range), so the element is now that
//   list (the model of the landed 0x00A82730) and the stand-in body is the real one. Bytes unchanged.
// Label note: "CNavQuadTree::CopyAllLayers" is a manifest label; the body is the STLport uninitialized_copy of a
//   0x28-byte element (CNavQuadTree::CNavigationLayer is 52 bytes in the PDB, so not that type); modelled by shape.
// Shape: SGI `for (; first != last; ++first, ++result) _Construct(&*result, *first)` with the placement-new null test;
//   the element copy is compiler-generated: three STLport vector copy ctors (_Vector_base(n, a) zero/allocate/set,
//   then __uninitialized_copy into _M_start), the map's empty comparator copied as nothing, its Dirty byte copied.
extern "C" void* __cdecl malloc(unsigned int size);                                    // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);     // 0x00BFEAE6
inline void* operator new(unsigned int, void* where) { return where; }

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
struct SValue16 { long m_0, m_4, m_8, m_c; };          // the list's 16-byte value (not named)
struct _List_node16 : _List_node_base {                 // 0x18 bytes
    SValue16 _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
};
struct _List_iterator16 : _List_iterator_base {         // iterator: user copy constructor
    _List_iterator16(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator16(const _List_iterator16& x) : _List_iterator_base(x._M_node) { }
};
struct _List_const_iterator16 : _List_iterator_base {   // const_iterator: implicit (trivial) copy
    _List_const_iterator16(_List_node_base* x) : _List_iterator_base(x) { }
};
struct SNavList {                                       // _STL::list<SValue16>: one header-node pointer (4 bytes)
    _List_node16* _M_node;
    _List_iterator16 begin() { return _List_iterator16(_M_node->_M_next); }
    _List_const_iterator16 begin() const { return _List_const_iterator16(_M_node->_M_next); }
    _List_const_iterator16 end() const { return _List_const_iterator16(_M_node); }
    void _M_insert_dispatch(_List_iterator16 pos, _List_const_iterator16 first, _List_const_iterator16 last, const __false_type&);   // 0x005152C0
    SNavList(const SNavList& x) : _M_node(0)
    {
        _List_node16* n = (_List_node16*)malloc(sizeof(_List_node16));
        n->_M_next = n;
        n->_M_prev = n;
        _M_node = n;
        _M_insert_dispatch(begin(), x.begin(), x.end(), __false_type());
    }
};
struct SNavPair {                                                                       // 16-byte non-POD pair
    unsigned __int64 first;
    bool             second;
    SNavPair(const SNavPair& o) : first(o.first), second(o.second) { }
};
#pragma auto_inline(off)
// Same-TU bodies of the element __uninitialized_copy instantiations (0x00A82730, 0x00A83340; the tag is taken by
// const reference and never read): placement-copy each element.
__declspec(noinline) SNavList* __fastcall __uninitialized_copy(const SNavList* first, const SNavList* last, SNavList* result, const __false_type&)
{
    SNavList* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SNavList(*first);
    return cur;
}
__declspec(noinline) SNavPair* __fastcall __uninitialized_copy(const SNavPair* first, const SNavPair* last, SNavPair* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SNavPair(*first);
    return result;
}
#pragma auto_inline(on)

inline unsigned int* __uninitialized_copy(const unsigned int* first, const unsigned int* last, unsigned int* result, const __true_type&)
{
    return (last == first) ? result
        : (unsigned int*)((char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first));
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
template <class T, class IsPOD> struct vector : _Vector_base<T> {
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    allocator<T> get_allocator() const { return allocator<T>(); }
    __forceinline vector(const vector& x) : _Vector_base<T>(x.size(), x.get_allocator())
    {
        IsPOD tag;
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, tag);
    }
};
struct CKeyPairCompareLess { };
struct CNavPairMap {                                    // CVectorMap<unsigned __int64, bool, CKeyPairCompareLess<...> >
    vector<SNavPair, __false_type> Entries;             // +0x00
    CKeyPairCompareLess            Compare;             // +0x0c
    bool                           Dirty;               // +0x0d
    __forceinline CNavPairMap(const CNavPairMap& o) : Entries(o.Entries), Compare(o.Compare), Dirty(o.Dirty) { }
};
struct SNavLayerRecord {                                // 0x28 bytes
    vector<unsigned int, __true_type> Indices;          // +0x00
    vector<SNavList, __false_type>    Lists;            // +0x0c
    CNavPairMap                       Map;              // +0x18
    __forceinline SNavLayerRecord(const SNavLayerRecord& o) : Indices(o.Indices), Lists(o.Lists), Map(o.Map) { }
};

SNavLayerRecord* __fastcall __uninitialized_copy(const SNavLayerRecord* first, const SNavLayerRecord* last, SNavLayerRecord* result, const __false_type&)
{
    SNavLayerRecord* cur = result;
    {
        for (; first != last; ++first, ++cur)
            new (cur) SNavLayerRecord(*first);
        return cur;
    }
}