// CEngineLandscapeMap::IsBackgroundAtFullDetailInArea(long,long,long,long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDC150, oracle length 17 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov ecx,[ecx+4] ; test ecx,ecx ; jne L ; mov al,1 ; ret 10h ; L: jmp CLandscapeBackgroundTreeNode::IsBackgroundAtFullDetailInArea
// PDB: CEngineLandscapeMap +0x04 = CLandscapeBackgroundTreeNode* PatchTree. With no background
// patch tree loaded every area counts as "full detail" (true); otherwise the query is forwarded
// to the tree root (0x00BF6D50, same four long arguments), which VC7.1 emits as a tail jump.

// Call-surface view of CLandscapeBackgroundTreeNode (its engine header carries the layout only).
struct CLandscapeBackgroundTreeNodeCalls {
    bool IsBackgroundAtFullDetailInArea(long minX, long minY, long maxX, long maxY);   // 0x00BF6D50
};

struct CEngineLandscapeMap {
    void*                              __vftable;   // +0x00
    CLandscapeBackgroundTreeNodeCalls* PatchTree;   // +0x04 (PDB: CLandscapeBackgroundTreeNode*)
    bool IsBackgroundAtFullDetailInArea(long minX, long minY, long maxX, long maxY);
};

bool CEngineLandscapeMap::IsBackgroundAtFullDetailInArea(long minX, long minY, long maxX, long maxY)
{
    CLandscapeBackgroundTreeNodeCalls* tree = this->PatchTree;
    if (!tree)
        return true;                    // nothing to stream in: always at full detail
    return tree->IsBackgroundAtFullDetailInArea(minX, minY, maxX, maxY);
}
