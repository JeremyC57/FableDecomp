// _global::_Push_heap(SULongKeyedRecord56*, int, int, SULongKeyedRecord56, SKeyLess) -- STLport heap sift-up: move the hole toward the top while the parent orders before the value, then drop the value in
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A31410, 95 bytes, `ret 0x40`.  No callees.
// Element: unnamed 56-byte record keyed by an unsigned long at +0, i.e. a pair<unsigned long, 52-byte POD> (the caller chain reaches 0x00A2F2D0, manifest-labelled LoadBinary; label propagated).
// Link-time __fastcall (the algorithm TUs are /Gr): first in ecx, hole in edx, then top, the
// 56-byte record by value and the empty 4-byte comparator on the stack (4 + 56 + 4 = 0x40).
// The record is trivially copyable, so both element moves are `rep movsd` x14.
// Predicate: `cmp [parent],value.key ; jae` = unsigned `parent.m_key < value.m_key`.
// Shape: SGI/STLport `__push_heap` -- `parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`
// (the Dinkumware `_Top < _Hole` spelling flips the compare to `cmp ebp,edi ; jge`).
namespace N00a31410 {
struct SULongKeyedRecord56 {
    unsigned long m_key;          // +0x0 sort key
    long m_words[13];             // +0x4 payload (52 bytes: a C3DDummyObject-sized value)
};

struct SKeyLess {
    char m_tag;
    bool operator()(const SULongKeyedRecord56& a, const SULongKeyedRecord56& b) const { return a.m_key < b.m_key; }
};

void __fastcall _Push_heap(SULongKeyedRecord56* first, int hole, int top, SULongKeyedRecord56 value, SKeyLess comp)
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