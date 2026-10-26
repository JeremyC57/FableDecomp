// CTCCameraPointScripted::GetLookDirection(...) -- catalog label; the body is the SGI `find(first, last, value)` front end over list iterators with a user copy constructor
// Leaf: FindInListRange
//
// Retail 0x00567A40, 39 bytes (`ret 8`). Callee: the list-iterator `__find` @ 0x00567240 (manifest
// "DataList_FindEqualNode"): it walks the nodes from `first` to `last` comparing each node's +8
// string block with *value (lengths first, then the characters). Called from
// CTCHeroOnlineScoreboard::DisplayScoreboardGUI (0x00563920), so the camera-point label is
// propagated over an unrelated template body.
// Shape: /Gr `__fastcall` as the 0x0052E0A0 twin (hidden result in ecx, the `const&` value in edx
// passed through, the iterator classes on the stack), but this iterator has a USER COPY CONSTRUCTOR:
// each by-value iterator is built in its reserved argument slot (`push ecx ; mov eax,esp ;
// mov [eax],ecx`), and the empty tag's address (`last`'s own slot) is taken before either copy.

class CWideString;

struct SListNode
{
    SListNode*   _M_next;                           // +0x00
    SListNode*   _M_prev;                           // +0x04
    CWideString* _M_data;                           // +0x08
};

struct SListIterator                                // the list's 4-byte iterator class
{
    SListNode* _M_node;
    SListIterator() {}
    SListIterator(SListNode* node) : _M_node(node) {}
    SListIterator(const SListIterator& other) : _M_node(other._M_node) {}
};

struct SInputIteratorTag { SInputIteratorTag() {} };   // input_iterator_tag

SListIterator __fastcall FindInListRangeTagged(SListIterator first, SListIterator last, CWideString* const& value,
                                               const SInputIteratorTag& tag);   // 0x00567240

SListIterator __fastcall FindInListRange(SListIterator first, SListIterator last, CWideString* const& value)
{
    return FindInListRangeTagged(first, last, value, SInputIteratorTag());
}