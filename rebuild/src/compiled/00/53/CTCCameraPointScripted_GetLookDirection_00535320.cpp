// CTCCameraPointScripted::GetLookDirection(...) -- catalog label; the body is the SGI `find(first, last, value)` front end over tree iterators: forward to the out-of-line `__find` with an input-iterator tag
// Leaf: FindInTreeRange
//
// Retail 0x00535320, 31 bytes (`ret 8`). Callee: the tree-iterator `__find` @ 0x00534F20 (manifest
// "std::__find_in_tree_by_key"; a second instance of the 0x0052DEC0 body for another tree type):
// it walks in-order successors from `first` to `last` comparing
// each node's +0x10 word with *value. The camera-point label is propagated over an unrelated
// template body (no direct callers; the instance is an out-of-line copy).
// Shape: the SGI algorithm TUs are compiled /Gr, so both functions are `__fastcall`; the iterator
// has user constructors, so it is returned through a hidden pointer (a POD 4-byte iterator comes
// back in eax and the front end collapses to a 22-byte forwarder). The hidden result pointer is the first register argument (ecx), the 4-byte iterator CLASSES are never
// register arguments (they go on the stack), so the `const long&` value takes edx and is passed
// through untouched; the tag is an empty class with a user constructor bound to a const reference,
// homed in `last`'s argument slot once `last` has been read (`mov ecx,[esp+0Ch] ; lea eax,[esp+0Ch]`).
// The result pointer is returned in eax.

struct STreeNode
{
    STreeNode* _M_left;                             // +0x00
    STreeNode* _M_parent;                           // +0x04
    STreeNode* _M_right;                            // +0x08
    unsigned char _M_color[0x4];                    // +0x0c
    long       _M_value;                            // +0x10
};

struct STreeIterator                                // the tree's 4-byte iterator class (non-POD)
{
    STreeNode* _M_node;
    STreeIterator() {}
    STreeIterator(STreeNode* node) : _M_node(node) {}
};

struct SInputIteratorTag { SInputIteratorTag() {} };   // input_iterator_tag

STreeIterator __fastcall FindInTreeRangeTagged(STreeIterator first, STreeIterator last, const long& value,
                                               const SInputIteratorTag& tag);   // 0x00534F20

STreeIterator __fastcall FindInTreeRange(STreeIterator first, STreeIterator last, const long& value)
{
    return FindInTreeRangeTagged(first, last, value, SInputIteratorTag());
}