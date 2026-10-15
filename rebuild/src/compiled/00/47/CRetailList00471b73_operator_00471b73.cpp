// CRetailList_00471b73::operator=(const CRetailList_00471b73& other) -- STLport list<24-byte record>::operator=: element-wise copy over the common length, then erase the surplus tail or insert the rest of the source
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00471B73, length 118 bytes.
// Label correction: the catalog's "CEngineSceneGrid::AssignCellBoundingBoxes" is an invented label; the body is the
// STLport `list::operator=` for a 24-byte POD element (32-byte nodes), called from the memberwise assignment at
// 0x00471AA4 for the list member at +0x4c of its owner. Named by shape.
// Callees: 0x00471BE9 list::erase(iterator first, iterator last) (returns the iterator through a hidden pointer,
// `ret 0Ch`), 0x00471C19 list::insert(iterator pos, const_iterator first, const_iterator last) (`ret 0Ch`; builds
// the `__false_type` tag and dispatches).
// Shape: STLport `operator=` verbatim: `if (this != &x) { first1/last1/first2/last2; while (first1 != last1 &&
// first2 != last2) *first1++ = *first2++; if (first2 == last2) erase(first1, last1); else insert(last1, first2,
// last2); } return *this;`. The frame-pointer size TU (`push 6 ; pop ecx` for the 24-byte `rep movsd` record copy,
// `this` spilled to [ebp-4], `last2` kept in the dead argument slot, the erase result homed in it too);
// iterators are 4-byte classes with user copy constructors (built in their argument slots), const iterators are
// plain records pushed by value.
// Leaf: ??4CRetailList_00471b73
#pragma optimize("y", off)
#pragma optimize("s", on)

struct SRecord24 { unsigned long m_data[6]; };           // 24-byte POD element (C3DBoundingBox-sized)
struct SListNode {
    SListNode* m_pNext;
    SListNode* m_pPrev;
    SRecord24  m_data;
};
struct SListIterator {
    SListNode* m_pNode;
    SListIterator(SListNode* node) : m_pNode(node) { }
    SListIterator(const SListIterator& other) : m_pNode(other.m_pNode) { }
    SRecord24& operator*() const { return m_pNode->m_data; }
    SListIterator operator++(int) { SListIterator old(*this); m_pNode = m_pNode->m_pNext; return old; }
    bool operator!=(const SListIterator& other) const { return m_pNode != other.m_pNode; }
};
struct SListConstIterator {
    SListNode* m_pNode;
    const SRecord24& operator*() const { return m_pNode->m_data; }
    SListConstIterator operator++(int) { SListConstIterator old = *this; m_pNode = m_pNode->m_pNext; return old; }
    bool operator==(const SListConstIterator& other) const { return m_pNode == other.m_pNode; }
    bool operator!=(const SListConstIterator& other) const { return m_pNode != other.m_pNode; }
};

class CRetailList_00471b73 {
public:
    SListNode* m_pNode;                                  // STLport list: the sentinel node pointer
    SListIterator begin() { return SListIterator(m_pNode->m_pNext); }
    SListIterator end() { return SListIterator(m_pNode); }
    SListConstIterator begin() const { SListConstIterator it; it.m_pNode = m_pNode->m_pNext; return it; }
    SListConstIterator end() const { SListConstIterator it; it.m_pNode = m_pNode; return it; }
    SListIterator erase(SListIterator first, SListIterator last);                       // 0x00471BE9
    void insert(SListIterator pos, SListConstIterator first, SListConstIterator last);   // 0x00471C19
    CRetailList_00471b73& operator=(const CRetailList_00471b73& other);
};

CRetailList_00471b73& CRetailList_00471b73::operator=(const CRetailList_00471b73& other)
{
    if (this != &other) {
        SListIterator first1 = begin();
        SListIterator last1 = end();
        SListConstIterator first2 = other.begin();
        SListConstIterator last2 = other.end();
        while (first1 != last1 && first2 != last2)
            *first1++ = *first2++;
        if (first2 == last2)
            erase(first1, last1);
        else
            insert(last1, first2, last2);
    }
    return *this;
}