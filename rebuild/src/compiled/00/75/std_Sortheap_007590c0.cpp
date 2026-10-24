// std::_Sort_heap(SKeyedBlock44* first, SKeyedBlock44* last, CKeyPairCompareLess pred) -- Dinkumware _Sort_heap over a (long key, 40-byte POD) vector: pop the root to the back until one element remains
// Leaf: _Sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007590C0, length 201 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned long,C3DDummyObject>*,...>` is propagated over the
// byte-identical skeleton (Ego_r.pdb C3DDummyObject is 52 bytes; this stride is 0x2C, the 0x2E8BA2E9 / sar 3
// divide by 44). The callee 0x00758910 (manifest `_Adjust_heap<CDebugBox*,int,CDebugBox>`, also propagated:
// PDB CDebugBox is 84 bytes) sifts with a SIGNED compare of the first dword and takes `ret 34h` (count, the
// 0x2C-byte value, the comparator), so the element is a pair of a long key and a 40-byte POD payload whose
// real type is not recoverable from these bodies (the only caller, 0x007594C0, is a partial_sort front end).
// Callees: _Adjust_heap @ 0x00758910 (out of line, __fastcall: first in ecx, hole 0 in edx).
// Shape: /Gr Dinkumware TU with _Pop_heap_0 / _Pop_heap inline and the loop calling _Pop_heap_0 directly (no pop_heap `1 < last - first` guard: VC7.1 does not fold it). `_Ty(*(_Last - 1))` is a frame temporary
// built by the pair's memberwise user copy constructor (key dword, then `rep movsd` x10 for the POD second);
// `*_Dest = *_First` is the implicit (trivial) assignment, one `rep movsd` x11; the value argument is copied
// from the temporary the same split way. The comparator dword is re-read from the stack parameter per pass.
struct SPayload40 {                         // 40-byte POD payload (copied as one block)
    unsigned long m_words[10];
};
struct SKeyedBlock44 {                      // std::pair<long, payload> (0x2C bytes)
    long       first;                       // +0x00 sort key (signed compare in _Adjust_heap)
    SPayload40 second;                      // +0x04
    SKeyedBlock44(const SKeyedBlock44& other) : first(other.first), second(other.second) { }
};
struct CKeyPairCompareLess {                // empty, passed by value
    bool operator()(const SKeyedBlock44& a, const SKeyedBlock44& b) const { return a.first < b.first; }
};

void __fastcall _Adjust_heap(SKeyedBlock44* first, int hole, int bottom, SKeyedBlock44 value,
                             CKeyPairCompareLess pred);   // 0x00758910

inline void _Pop_heap(SKeyedBlock44* first, SKeyedBlock44* last, SKeyedBlock44* dest, SKeyedBlock44 value,
                      CKeyPairCompareLess pred, int*)
{
    *dest = *first;
    _Adjust_heap(first, 0, int(last - first), value, pred);
}

inline void _Pop_heap_0(SKeyedBlock44* first, SKeyedBlock44* last, CKeyPairCompareLess pred, SKeyedBlock44*)
{
    _Pop_heap(first, last - 1, last - 1, SKeyedBlock44(*(last - 1)), pred, (int*)0);
}

void __fastcall _Sort_heap(SKeyedBlock44* first, SKeyedBlock44* last, CKeyPairCompareLess pred)
{
    for (; 1 < last - first; --last)
        _Pop_heap_0(first, last, pred, (SKeyedBlock44*)0);
}