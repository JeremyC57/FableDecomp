// CVtable_012b2f0c::GetItemByIndex(long) -- the index-th item of the tree in pre-order: a zeroed counter and the recursive walk from the first root child
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C190F0, length 31 bytes (+ int3 padding).
// Callees: CVtable_012b2f0c::FindNthInSubtree(SNode*, long, long*) @ 0x00C18D90 (`__fastcall` on the
// same object; the manifest calls it "Tree_PreorderFind": it walks the sibling chain from the given
// node, recursing into each node's children first, bumps *counter per node and returns the node's
// item (+0x10) when the counter reaches the index, null when the chain ends at the sentinel &this+0xc).
// Slot 9 of the RTTI-less vtable 0x012B2F0C (the no-RTTI module around 0x00C09000..; the manifest's
// "CComponent::AddChildFront" label is propagated). The counter is a frame local (`push ecx`) whose
// zero store is scheduled after the argument pushes; the walk starts at the node pointer at +0x20.
struct SNode {
    SNode* firstChild;                                          // +0x00
    SNode* nextSibling;                                         // +0x04
    unsigned char _pad[0x08];
    void*  item;                                                // +0x10
};

struct CVtable_012b2f0c {
    virtual void v00();
    unsigned char _body[0x1c];                                  // +0x04..+0x1f (the sentinel node sits at +0x0c)
    SNode* m_pFirstChild;                                       // +0x20
    void* FindNthInSubtree(SNode* node, long index, long* counter);   // 0x00C18D90
    void* GetItemByIndex(long index);
};

void* CVtable_012b2f0c::GetItemByIndex(long index)
{
    long counter = 0;
    return FindNthInSubtree(m_pFirstChild, index, &counter);
}