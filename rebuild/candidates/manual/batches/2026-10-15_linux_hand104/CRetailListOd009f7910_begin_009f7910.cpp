#pragma optimize("", off)
// CRetailListOd_009f7910::begin() -- begin() of a retail (VC6-era Dinkumware) std::list in an unoptimised translation unit: the iterator built from the sentinel's next node
// Leaf: begin
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F7910, length 41 bytes.
// Label: the catalog name "CTCVillage::GetBestTaskForThing<CVillageTaskCarryCrate>" is propagated; the body is the
// list begin() of the landed unoptimised list family (CRetailListOd_009f7aa0 / _009f7ce0 next to it) and has no
// callers or vtable references. Its twin 0x009F7940 is byte-identical (another instantiation).
// Callees: the list iterator's constructor from a node pointer @ 0x0041C780 (`mov eax,ecx ; mov [eax],arg ; ret 4`).
// Pragma: #pragma optimize("", off) -- `this` homed at [ebp-8] and reloaded, the first node homed at [ebp-4] (a named
// local), the iterator returned through the hidden result pointer ([ebp+8]) and that pointer returned in eax.
struct CRetailListOd_009f7910 {
    struct SNode {
        SNode* m_pNext;
        SNode* m_pPrev;
    };
    struct iterator {
        SNode* m_pNode;
        iterator(SNode* node);                               // 0x0041C780
    };
    SNode* m_pHead;                                          // +0x00 sentinel node
    iterator begin();
};

CRetailListOd_009f7910::iterator CRetailListOd_009f7910::begin()
{
    SNode* first = m_pHead->m_pNext;
    return iterator(first);
}
