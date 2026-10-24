#pragma optimize("", off)
// CRetailListOd_009f7c20::back() -- back() of a retail (VC6-era Dinkumware) std::list in an unoptimised translation unit: step an end() iterator back once and return the last element
// Leaf: back
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F7C20, length 47 bytes (+ one int3; the 48-byte catalog row includes it), plain `ret`.
// Label: the catalog name "CPersistTraits<CObjectFamilyEntry>::TransferBinaryOut" is propagated; the body belongs to
//   the landed unoptimised list family around it (begin() 0x009F7910, dtors 0x009F7AA0 / 0x009F7CE0). No callers.
// Callees: the list iterator's constructor from a node pointer @ 0x0041C780, its operator-- @ 0x0041C7B0
//   (`m_pNode = m_pNode->m_pPrev`).
// Pragma: #pragma optimize("", off) -- `this` homed at [ebp-0Ch], the head node at [ebp-8] (a named local), the
//   iterator at [ebp-4]; the element (node +8) is addressed inline from the iterator's node.
struct CRetailListOd_009f7c20 {
    struct SNode {
        SNode* m_pNext;
        SNode* m_pPrev;
        long   m_value;                                      // +0x08
    };
    struct iterator {
        SNode* m_pNode;
        iterator(SNode* node);                               // 0x0041C780
        iterator& operator--();                              // 0x0041C7B0
    };
    SNode* m_pHead;                                          // +0x00 sentinel node
    long& back();
};

long& CRetailListOd_009f7c20::back()
{
    SNode* head = m_pHead;
    iterator last(head);
    --last;
    return last.m_pNode->m_value;
}