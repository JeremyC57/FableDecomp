// _global::UninitCopy_PairVectorMapRecord_0086d870(const SVectorMapRecord24* first, const SVectorMapRecord24* last, SVectorMapRecord24* result, const __false_type&) -- STLport __uninitialized_copy over 24-byte {long, long, CVectorMap} records: placement copy-construct [first, last), return the advanced result
// Leaf: UninitCopy_PairVectorMapRecord_0086d870
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086D870, length 66 bytes (+14 int3 pad), `ret 8`.
// Callers: 0x0086E418 / 0x0086E47D (landed vector insert-grow 0x0086E3B0) and 0x0086E656 / 0x0086E6B0 (landed resize
//   0x0086E5C0, catalog "Resize_CSharerParserEntry_24byte").
// Callees: the out-of-line vector copy constructor @ 0x00673E10 (catalog "std::vector_CopyRange").
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated. The tail copy
//   pattern -- a 12-byte vector copied out of line at +8, the byte at +0x14 skipped, the byte at +0x15 copied -- is a
//   retail CVectorMap (vector base 12 bytes, empty Compare +0x0C, bool Dirty +0x0D; GOTCHAS 00a85360 / 00a85740) at +8.
//   The two leading words and the map's key/value types are not named by a landed relative -- descriptive names.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx -> esi,
//   last in edx -> ebx, result + the tag reference on the stack -> edi, loaded before the empty test through the named
//   `cur` copy and returned from the shared epilogue). Implicit memberwise copies: the two words, the vector's
//   out-of-line copy on `this+8`, the empty Compare (no code), Dirty; placement-new null test on the destination.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SVector_0086d870 {                       // retail 12-byte STLport vector
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    SVector_0086d870(const SVector_0086d870& other);   // 0x00673E10
};

struct SCompare_0086d870 { };                   // empty comparator (1 byte, never copied)

struct SVectorMap_0086d870 : SVector_0086d870 { // CVectorMap<K, V, Compare>: vector base + Compare + Dirty
    SCompare_0086d870 Compare;                  // +0x0C
    bool              Dirty;                    // +0x0D
};

struct SVectorMapRecord24 {                     // 24 bytes
    long                m_0;                    // +0x00
    long                m_4;                    // +0x04
    SVectorMap_0086d870 m_map;                  // +0x08
};

SVectorMapRecord24* __fastcall UninitCopy_PairVectorMapRecord_0086d870(const SVectorMapRecord24* first, const SVectorMapRecord24* last, SVectorMapRecord24* result, const __false_type&)
{
    SVectorMapRecord24* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SVectorMapRecord24(*first);
    return cur;
}