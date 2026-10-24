// _global::_Push_heap(SLongKeyedRecord44*, int, int, SLongKeyedRecord44, SKeyLess) -- STLport heap sift-up: move the hole toward the top while the parent orders before the value, then drop the value in
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007583F0, 95 bytes, `ret 0x34`.  No callees.
// Element: unnamed 44-byte record keyed by a signed long at +0 (its sort is reached from 0x00757B30 / 0x00757990, manifest-labelled AddWound / TransferBinaryIn; the CDegradableInfo label is propagated).
// Link-time __fastcall (the algorithm TUs are /Gr): first in ecx, hole in edx, then top, the
// 44-byte record by value and the empty 4-byte comparator on the stack (4 + 44 + 4 = 0x34).
// The record is trivially copyable, so both element moves are `rep movsd` x11.
// Predicate: `cmp [parent],value.key ; jge` = signed `parent.m_key < value.m_key`.
// Shape: SGI/STLport `__push_heap` -- `parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`
// (the Dinkumware `_Top < _Hole` spelling flips the compare to `cmp ebp,edi ; jge`).
namespace N007583f0 {
struct SLongKeyedRecord44 {
    long m_key;                   // +0x0 sort key
    long m_words[10];             // +0x4 payload
};

struct SKeyLess {
    char m_tag;
    bool operator()(const SLongKeyedRecord44& a, const SLongKeyedRecord44& b) const { return a.m_key < b.m_key; }
};

void __fastcall _Push_heap(SLongKeyedRecord44* first, int hole, int top, SLongKeyedRecord44 value, SKeyLess comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
}