// Behaviour test for 00a85420: src/00a85420.cpp copied verbatim; malloc/memmove are the real CRT, the two element
// copies are the source's own same-TU bodies. __uninitialized_copy<SNavLayerRecord>
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// __uninitialized_copy(const SNavLayerRecord* first, const SNavLayerRecord* last, SNavLayerRecord* result, const __false_type&) -- placement-copy a range of 40-byte records (three STLport vectors, the last inside a CVectorMap) and return the end
// Grade: RELOCATION_MATCH (retry helper C) -- the two element __uninitialized_copy callees (0x00A82730, 0x00A83340) that take the __false_type tag by const reference get same-TU noinline bodies; the tags then overlay the dead `result` slot as retail (was DIFFER(320v316), frame-only: tag bytes in their own frame words).
// Leaf: ?__uninitialized_copy@@YIPAUSNavLayerRecord@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A85420, 316 bytes (+ int3 padding), `__fastcall` (first ecx, last edx), `ret 8`.
// Callees: malloc 0x00BFEA0E, memmove 0x00BFEAE6 (the POD vector's __copy_trivial), the 4-byte-element
//   __uninitialized_copy 0x00A82730 and the 16-byte-pair __uninitialized_copy 0x00A83340 (both `__fastcall`, tag by reference).
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

struct SNavCell { int Value; SNavCell(const SNavCell& o) : Value(o.Value) { } };        // 4-byte non-POD element
struct SNavPair {                                                                       // 16-byte non-POD pair
    unsigned __int64 first;
    bool             second;
    SNavPair(const SNavPair& o) : first(o.first), second(o.second) { }
};
#pragma auto_inline(off)
// Same-TU bodies of the element __uninitialized_copy instantiations (0x00A82730, 0x00A83340; the tag is taken by
// const reference and never read): placement-copy each element.
__declspec(noinline) SNavCell* __fastcall __uninitialized_copy(const SNavCell* first, const SNavCell* last, SNavCell* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SNavCell(*first);
    return result;
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
    vector<SNavCell, __false_type>    Cells;            // +0x0c
    CNavPairMap                       Map;              // +0x18
    __forceinline SNavLayerRecord(const SNavLayerRecord& o) : Indices(o.Indices), Cells(o.Cells), Map(o.Map) { }
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

// ---- driver ----
template <class T> static void fill(T*& s, T*& f, T*& e, const T* v, int n)
{
    s = n ? (T*)malloc(n * sizeof(T)) : 0; memcpy(s, v, n * sizeof(T)); f = s + n; e = f;
}
int main()
{
    SNavLayerRecord* src = (SNavLayerRecord*)malloc(2 * sizeof(SNavLayerRecord));
    memset(src, 0, 2 * sizeof(SNavLayerRecord));
    unsigned int idx[3] = { 1, 2, 3 };
    fill(src[0].Indices._M_start, src[0].Indices._M_finish, src[0].Indices._M_end_of_storage._M_data, idx, 3);
    SNavCell* cells = (SNavCell*)malloc(2 * sizeof(SNavCell)); cells[0].Value = 7; cells[1].Value = 8;
    src[0].Cells._M_start = cells; src[0].Cells._M_finish = cells + 2; src[0].Cells._M_end_of_storage._M_data = cells + 2;
    SNavPair* pairs = (SNavPair*)malloc(sizeof(SNavPair)); memset(pairs, 0, sizeof(SNavPair)); pairs[0].first = 0x123456789ULL; pairs[0].second = true;
    src[0].Map.Entries._M_start = pairs; src[0].Map.Entries._M_finish = pairs + 1; src[0].Map.Entries._M_end_of_storage._M_data = pairs + 1;
    src[0].Map.Dirty = true;
    src[1].Map.Dirty = false;                                  // all-empty record
    SNavLayerRecord* dst = (SNavLayerRecord*)malloc(2 * sizeof(SNavLayerRecord));
    memset(dst, 0xAB, 2 * sizeof(SNavLayerRecord));
    // empty range: nothing constructed, result returned
    CHECK(__uninitialized_copy(src, src, dst, __false_type()) == dst);
    CHECK(((unsigned char*)dst)[0] == 0xAB);
    SNavLayerRecord* end = __uninitialized_copy(src, src + 2, dst, __false_type());
    CHECK(end == dst + 2);
    CHECK(dst[0].Indices.size() == 3 && dst[0].Indices._M_start != src[0].Indices._M_start);
    CHECK(dst[0].Indices._M_start[0] == 1 && dst[0].Indices._M_start[2] == 3);
    CHECK(dst[0].Indices._M_end_of_storage._M_data == dst[0].Indices._M_finish);
    CHECK(dst[0].Cells.size() == 2 && dst[0].Cells._M_start != cells && dst[0].Cells._M_start[1].Value == 8);
    CHECK(dst[0].Map.Entries.size() == 1 && dst[0].Map.Entries._M_start != pairs);
    CHECK(dst[0].Map.Entries._M_start[0].first == 0x123456789ULL && dst[0].Map.Entries._M_start[0].second == true);
    CHECK(dst[0].Map.Dirty == true);
    CHECK(dst[1].Indices._M_start == 0 && dst[1].Indices._M_finish == 0 && dst[1].Indices._M_end_of_storage._M_data == 0);
    CHECK(dst[1].Cells._M_start == 0 && dst[1].Cells._M_finish == 0);
    CHECK(dst[1].Map.Entries._M_start == 0 && dst[1].Map.Entries._M_finish == 0);
    CHECK(dst[1].Map.Dirty == false);
    if (fails == 0) printf("00a85420_TEST PASS\n");
    return fails;
}