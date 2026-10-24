// std::fill(SNavContainerRecord* first, SNavContainerRecord* last, const SNavContainerRecord& val) -- assign val to every 40-byte {vector, vector, vector map} record of [first, last)
// Leaf: fill
// Retail: Fable.exe @ 0x00A85810, 96 bytes, `ret 4`: __fastcall (first ecx -> esi, last edx -- spilled to the `push ecx`
// frame word -- and the value reference on the stack -> ebx). Only call site 0x00A85DDD in the vector assign path at
// 0x00A85CC0 (catalogued Vector_Assign_QuestPersistInfo).
// Element (from the body, 40 bytes): an STLport vector of 4-byte elements at +0 (operator= @ 0x00A80EE0), a second one
// at +0xC (operator= @ 0x00A83D70) and a CVectorMap at +0x18 = {vector of 16-byte pairs (operator= @ 0x00A84940) +0x18,
// the empty comparator +0x24, Dirty +0x25}. The catalog's pair<CCharString, CPersistInfo> is propagated (no CCharString
// is touched); the 4-byte element types are not recoverable from this body and are modelled as long / unsigned long.
// Callees: the three out-of-line vector::operator= @ 0x00A80EE0, 0x00A83D70, 0x00A84940.
// Shape: the record's implicit copy assignment is memberwise: three operator= calls with the source member addresses
// hoisted out of the loop (ebx, `lea ebp,[ebx+0Ch]`, `lea edi,[ebx+18h]`), nothing for the empty comparator, then
// the Dirty byte (`mov al,[edi+0Dh] ; mov [esi+25h],al`); `last` re-read from the frame word.
namespace std {
template <class T>
struct vector {                                 // STLport vector: 12 bytes
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    vector<T>& operator=(const vector<T>& x);   // out of line
};
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
};
}
template <class K, class V>
struct CKeyPairCompareLess {
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
template <class K, class V, class Compare>
struct CVectorMap {                             // retail 16 bytes
    std::vector<std::pair<K, V> > Data;         // +0x00
    Compare                       Comparator;   // +0x0C (empty)
    bool                          Dirty;        // +0x0D
};

struct SMapValue12 { long m_0, m_4, m_8; };    // the map's 12-byte mapped value (pair stride 16, `sar 4` in 0x00A84940)

struct SNavContainerRecord {                    // 40 bytes
    std::vector<long>          m_first;         // +0x00 (operator= 0x00A80EE0)
    std::vector<unsigned long> m_second;        // +0x0C (operator= 0x00A83D70)
    CVectorMap<long, SMapValue12, CKeyPairCompareLess<long, SMapValue12> > m_map;   // +0x18 (vector operator= 0x00A84940)
};

namespace std {
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
template void __fastcall fill(SNavContainerRecord*, SNavContainerRecord*, const SNavContainerRecord&);
}