// CLandscapeBackgroundTreeNode::GetSplitBranch()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF6160, oracle length 36 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+0A0h] ; test eax,eax ; je Z ; mov ecx,eax ; mov eax,[ecx] ; call [eax+10h] ; test al,al ; jne Z ; mov eax,[esi+0A0h] ; pop esi ; ret ; Z: xor eax,eax ; pop esi ; ret
// Returns the sub-node when it is a split branch (virtual slot 4 answers false), else null.

// Retail CLandscapeBackgroundTreeNode keeps SubNode at +0xA0 where engine/CLandscapeBackgroundTreeNode.h
// (Ego_r.pdb) has AsyncLoadTask (+0xA0) / SubNode (+0xA8): two std containers inside the preceding
// members are each 4 bytes smaller in the shipped build, so a retail-offset view is declared here.
struct CLandscapeBackgroundSubNodeVirtuals {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual bool IsPatch();            // slot 4 (+0x10): true for a leaf patch, false for a split branch
};

struct CLandscapeBackgroundTreeNode_Retail {
    unsigned char                        _pad_0x00[0xA0];
    CLandscapeBackgroundSubNodeVirtuals* SubNode;      // +0xA0 (PDB +0xA8)
    CLandscapeBackgroundSubNodeVirtuals* GetSplitBranch();
};

CLandscapeBackgroundSubNodeVirtuals* CLandscapeBackgroundTreeNode_Retail::GetSplitBranch()
{
    if (this->SubNode && !this->SubNode->IsPatch())
        return this->SubNode;
    return 0;
}