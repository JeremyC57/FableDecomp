// CMapBrushApplier_00820c90::ApplyAll() -- for every node of the list whose sentinel is at +0xc, apply the scripted map brush (0x00820B80) of the node's mode block (+8, CModeBlock::GetAnimFilter @ 0x0082E250 with 1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00820C90, length 48 bytes.
// Callees: CModeBlock::GetAnimFilter @ 0x0082E250 (on the node's +8, 1), CMap::ApplyScriptedMapBrush @ 0x00820B80 (on `this`, the filter). Shape: the sentinel-based
// list walk (`mov esi,[eax] ; cmp esi,eax` then `cmp esi,[edi+0Ch]`), `nop`-aligned loop top. The chunk label "CGraphicBankManager::FrameEnd" is propagated; named by address.
struct CModeBlock_00820c90 {
    void* GetAnimFilter(long flag);                // 0x0082E250
};
struct SBankNode_00820c90 {                        // a list node: next, prev, the payload
    SBankNode_00820c90* m_pNext;
    SBankNode_00820c90* m_pPrev;
    CModeBlock_00820c90 m_block;                   // +0x08
};

struct CMapBrushApplier_00820c90 {
    unsigned char      _pad[0xc];
    SBankNode_00820c90* m_pHead;                   // +0x0c (the sentinel)
    void ApplyScriptedMapBrush(void* filter);      // 0x00820B80
    void ApplyAll();
};

void CMapBrushApplier_00820c90::ApplyAll()
{
    for (SBankNode_00820c90* node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext)
        ApplyScriptedMapBrush(node->m_block.GetAnimFilter(1));
}