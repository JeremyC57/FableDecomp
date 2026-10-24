// std::_Sort_heap(SKeyedBlock40* first, SKeyedBlock40* last, CKeyPairCompareLess pred) -- Dinkumware _Sort_heap over a (long key, 36-byte POD) vector: pop the root to the back until one element remains
// Leaf: _Sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081C700, length 201 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned long,C3DDummyObject>*,...>` is propagated over the
// byte-identical skeleton (stride 0x28: the 0x66666667 / sar 4 divide by 40; Ego_r.pdb C3DDummyObject is 52
// bytes). The callee 0x0081AB40 (manifest `_Adjust_heap<CDebugCone*,int,CDebugCone>`, propagated: PDB CDebugCone
// starts with a float vector) sifts with a SIGNED compare of the first dword, so the element is a pair of a long
// key and a 36-byte POD payload whose real type is not recoverable here (sole caller 0x0081D570 is a
// partial_sort front end).
// Callees: _Adjust_heap @ 0x0081AB40 (out of line, __fastcall: first in ecx, hole 0 in edx, `ret 30h`).
// Shape: /Gr Dinkumware TU, the 0x007590C0 shape with a 9-dword payload: `_Ty(*(_Last - 1))` is a frame
// temporary built by the pair's memberwise user copy constructor (key, then `rep movsd` x9), `*_Dest = *_First`
// the trivial assignment (`rep movsd` x10), the value argument copied from the temporary the same split way.
// The loop calls _Pop_heap_0 directly: Dinkumware's pop_heap `1 < _Last - _First` guard would be a second,
// unfolded test inside the loop.
struct SPayload36 {                         // 36-byte POD payload (copied as one block)
    unsigned long m_words[9];
};
struct SKeyedBlock40 {                      // std::pair<long, payload> (0x28 bytes)
    long       first;                       // +0x00 sort key (signed compare in _Adjust_heap)
    SPayload36 second;                      // +0x04
    SKeyedBlock40(const SKeyedBlock40& other) : first(other.first), second(other.second) { }
};
struct CKeyPairCompareLess {                // empty, passed by value
    bool operator()(const SKeyedBlock40& a, const SKeyedBlock40& b) const { return a.first < b.first; }
};

void __fastcall _Adjust_heap(SKeyedBlock40* first, int hole, int bottom, SKeyedBlock40 value,
                             CKeyPairCompareLess pred);   // 0x0081AB40

inline void _Pop_heap(SKeyedBlock40* first, SKeyedBlock40* last, SKeyedBlock40* dest, SKeyedBlock40 value,
                      CKeyPairCompareLess pred, int*)
{
    *dest = *first;
    _Adjust_heap(first, 0, int(last - first), value, pred);
}

inline void _Pop_heap_0(SKeyedBlock40* first, SKeyedBlock40* last, CKeyPairCompareLess pred, SKeyedBlock40*)
{
    _Pop_heap(first, last - 1, last - 1, SKeyedBlock40(*(last - 1)), pred, (int*)0);
}

void __fastcall _Sort_heap(SKeyedBlock40* first, SKeyedBlock40* last, CKeyPairCompareLess pred)
{
    for (; 1 < last - first; --last)
        _Pop_heap_0(first, last, pred, (SKeyedBlock40*)0);
}