// _global::_Push_heap(SFloatKeyedRecord28*, int, int, SFloatKeyedRecord28, SKeyLess) -- STLport heap sift-up: move the hole toward the top while the parent orders before the value, then drop the value in
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CFEB0, 105 bytes, `ret 0x24`.  No callees.
// Element: unnamed 28-byte record keyed by a float at +0 (caller chain 0x004D0660 _Adjust_heap -> 0x004D2570, reached from 0x004CEA2A / 0x004D264E / 0x004D26E5, whose labels are propagated too; manifest label CDegradableInfo is propagated, and CDegradableInfo sorts with `>` at 0x0067E650).
// Link-time __fastcall (the algorithm TUs are /Gr): first in ecx, hole in edx, then top, the
// 28-byte record by value and the empty 4-byte comparator on the stack (4 + 28 + 4 = 0x24).
// The record is trivially copyable, so both element moves are `rep movsd` x7.
// Predicate: `fld [parent] ; fcomp [value] ; test ah,5 ; jp` = `parent.m_key < value.m_key`.
// Shape: SGI/STLport `__push_heap` -- `parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`
// (the Dinkumware `_Top < _Hole` spelling flips the compare to `cmp ebp,edi ; jge`).
namespace N004cfeb0 {
struct SFloatKeyedRecord28 {
    float m_key;                  // +0x0 sort key
    long m_words[6];              // +0x4 payload (copied as a block)
};

struct SKeyLess {
    char m_tag;
    bool operator()(const SFloatKeyedRecord28& a, const SFloatKeyedRecord28& b) const { return a.m_key < b.m_key; }
};

void __fastcall _Push_heap(SFloatKeyedRecord28* first, int hole, int top, SFloatKeyedRecord28 value, SKeyLess comp)
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