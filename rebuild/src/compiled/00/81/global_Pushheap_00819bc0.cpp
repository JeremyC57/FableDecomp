// _global::_Push_heap(SLongKeyedRecord40*, int, int, SLongKeyedRecord40, SKeyLess) -- STLport heap sift-up: move the hole toward the top while the parent orders before the value, then drop the value in
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00819BC0, 95 bytes, `ret 0x30`.  No callees.
// Element: unnamed 40-byte record keyed by a signed long at +0 (its sort is reached from 0x0081D620, manifest-labelled OnInitLeaf; the PDB's 40-byte CEntry {EBodyLocation Location, ...} has the right size and key type -- unconfirmed; label propagated).
// Link-time __fastcall (the algorithm TUs are /Gr): first in ecx, hole in edx, then top, the
// 40-byte record by value and the empty 4-byte comparator on the stack (4 + 40 + 4 = 0x30).
// The record is trivially copyable, so both element moves are `rep movsd` x10.
// Predicate: `cmp [parent],value.key ; jge` = signed `parent.m_key < value.m_key`.
// Shape: SGI/STLport `__push_heap` -- `parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`
// (the Dinkumware `_Top < _Hole` spelling flips the compare to `cmp ebp,edi ; jge`).
namespace N00819bc0 {
struct SLongKeyedRecord40 {
    long m_key;                   // +0x0 sort key
    long m_words[9];              // +0x4 payload
};

struct SKeyLess {
    char m_tag;
    bool operator()(const SLongKeyedRecord40& a, const SLongKeyedRecord40& b) const { return a.m_key < b.m_key; }
};

void __fastcall _Push_heap(SLongKeyedRecord40* first, int hole, int top, SLongKeyedRecord40 value, SKeyLess comp)
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