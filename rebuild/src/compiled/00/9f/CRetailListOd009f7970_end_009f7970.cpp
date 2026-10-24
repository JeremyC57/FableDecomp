#pragma optimize("", off)
// CRetailListOd_009f7970::end() -- end() of a retail (VC6-era Dinkumware) std::list in an unoptimised translation unit: the iterator built from the sentinel head node
// Leaf: end
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F7970, length 38 bytes (+ int3 padding; the 48-byte catalog row includes 10 of it), `ret 4`.
// Label: the catalog name "CPersistTraits<CReactionFrequencyTraitsArray>::~CPersistTraits" is propagated; the body is
//   the list end() of the landed unoptimised list family next to it (begin() 0x009F7910 / 0x009F7940, the dtors
//   0x009F7AA0 / 0x009F7CE0). Its twin 0x009F79A0 is byte-identical (another instantiation).
// Callees: the list iterator's constructor from a node pointer @ 0x0041C780 (`mov eax,ecx ; mov [eax],arg ; ret 4`).
// Pragma: #pragma optimize("", off) -- `this` homed at [ebp-8] and reloaded, the head node homed at [ebp-4] (a named
//   local), the iterator built in the hidden result pointer ([ebp+8]) and that pointer returned in eax.
struct CRetailListOd_009f7970 {
    struct SNode {
        SNode* m_pNext;
        SNode* m_pPrev;
    };
    struct iterator {
        SNode* m_pNode;
        iterator(SNode* node);                               // 0x0041C780
    };
    SNode* m_pHead;                                          // +0x00 sentinel node
    iterator end();
};

CRetailListOd_009f7970::iterator CRetailListOd_009f7970::end()
{
    SNode* head = m_pHead;
    return iterator(head);
}