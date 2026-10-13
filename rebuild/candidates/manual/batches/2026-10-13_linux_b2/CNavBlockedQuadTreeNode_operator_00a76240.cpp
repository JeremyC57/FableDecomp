// CNavBlockedQuadTreeNode::operator=(const CNavBlockedQuadTreeNode&)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A76240, oracle length 37 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,ecx ; mov ecx,[esp+4] ; cmp eax,ecx ; je R ; (copy +4,+8 dwords, +0Ch,+0Dh bytes) ; R: ret 4
// engine/CNavBlockedQuadTreeNode.h (PDB) gives only the vptr for this 0x10-byte node. The
// user-written assignment skips self-assignment, copies the two dword members and the two flag
// bytes, leaves the vptr alone and returns *this. Member names are inferred from the quad tree.

struct CNavBlockedQuadTreeNode_Retail {
    void*         __vftable;         // +0x00 (not copied)
    long          BlockedCount;      // +0x04 (name inferred)
    long          ChildMask;         // +0x08 (name inferred)
    bool          Blocked;           // +0x0C (name inferred)
    bool          Partial;           // +0x0D (name inferred)
    unsigned char _pad_0x0E[2];
    CNavBlockedQuadTreeNode_Retail& operator=(const CNavBlockedQuadTreeNode_Retail& other);
};

CNavBlockedQuadTreeNode_Retail& CNavBlockedQuadTreeNode_Retail::operator=(const CNavBlockedQuadTreeNode_Retail& other)
{
    if (this != &other) {
        this->BlockedCount = other.BlockedCount;
        this->ChildMask    = other.ChildMask;
        this->Blocked      = other.Blocked;
        this->Partial      = other.Partial;
    }
    return *this;
}
