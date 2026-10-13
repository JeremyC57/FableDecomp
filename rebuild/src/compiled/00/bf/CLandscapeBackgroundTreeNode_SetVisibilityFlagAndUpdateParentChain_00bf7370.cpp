// CLandscapeBackgroundTreeNode::SetVisibilityFlagAndUpdateParentChain(bool)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF7370, oracle length 43 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov al,[esp+4] ; cmp [ecx+0E4h],al ; je R ; mov [ecx+0E4h],al ; mov byte ptr [ecx+0E5h],1 ; mov ecx,[ecx+9Ch] ; test ecx,ecx ; je R ; call CLandscapeBackgroundTreeNode::UpdateParentChainVisibilityFlag ; R: ret 4
// engine/CLandscapeBackgroundTreeNode.h (PDB): Parent +0xA4, VisibleLastFrame +0xEC,
// VisibleLastFrameHasBeenUpdated +0xED; retail is 8 bytes earlier (see batch b2/b3). When the
// visibility changes it is recorded, flagged as updated, and the parent re-derives its own flag
// from its children (0x00BF6AC0, __fastcall on the parent).
// Layout note: in the shipped binary a std::vector / CArray / map / set / CVectorMap is 4 bytes
// smaller than in Ego_r.pdb (the editor build carries an extra word per container) and a
// std::list is 8 bytes smaller (retail keeps only the head pointer; the PDB list is 0xC bytes).
// A retail std::vector is {first, last, end} with `first` at +0. Members after such containers
// therefore sit earlier than the generated engine header says: 4 bytes per vector/map/set/CArray
// and 8 bytes per list that precede them.

struct CLandscapeBackgroundTreeNode_Retail {
    unsigned char                        _pad_0x00[0x9C];
    CLandscapeBackgroundTreeNode_Retail* Parent;                          // +0x9C (PDB +0xA4)
    unsigned char                        _pad_0xA0[0x44];
    bool                                 VisibleLastFrame;                // +0xE4 (PDB +0xEC)
    bool                                 VisibleLastFrameHasBeenUpdated;  // +0xE5 (PDB +0xED)
    void UpdateParentChainVisibilityFlag();     // 0x00BF6AC0
    void SetVisibilityFlagAndUpdateParentChain(bool visible);
};

void CLandscapeBackgroundTreeNode_Retail::SetVisibilityFlagAndUpdateParentChain(bool visible)
{
    if (this->VisibleLastFrame != visible) {
        this->VisibleLastFrame = visible;
        this->VisibleLastFrameHasBeenUpdated = true;
        if (this->Parent)
            this->Parent->UpdateParentChainVisibilityFlag();
    }
}