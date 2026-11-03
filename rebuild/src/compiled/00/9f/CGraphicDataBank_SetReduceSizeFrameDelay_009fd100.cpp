// CGraphicDataBank::SetReduceSizeFrameDelay(unsigned long delay) -- set the frame delay before the bank shrinks its loaded data (stored to both retail copies of the field)
// Leaf: ?SetReduceSizeFrameDelay@CGraphicDataBank@@QAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FD100, 19 bytes, `ret 4`.
// Identity (naming wave 34): Ego_r.pdb 0x00B566A0 / FableWin.pdb 0x03117A80 ?SetReduceSizeFrameDelay@CGraphicDataBank@@QAEXK@Z; the three
// landed callers CEngineResourceManager::Render 0x00B3B7C0 / FrameEnd 0x00B3B870 / ResizeHiresTextureMemory 0x00B3B4F0 pass
// it a frame count. Was landed as CLandscapeBackgroundSubNode::OnPostParentRelocation (a propagated label).
// Layout: the two retail fields +0x19C and +0x26C (Ego_r ReduceSizeFrameDelay +0x2E8; which retail word is which is not
// proven, so the members are named by offset). Retail bytes:  mov eax,[esp+4] ; mov [ecx+26Ch],eax ; mov [ecx+19Ch],eax ; ret 4
class CGraphicDataBank {
public:
    unsigned char _pad000[0x19c];
    unsigned long m_19c;                                // +0x19C
    unsigned char _pad1a0[0x26c - 0x1a0];
    unsigned long m_26c;                                // +0x26C
    void SetReduceSizeFrameDelay(unsigned long delay);
};

void CGraphicDataBank::SetReduceSizeFrameDelay(unsigned long delay)
{
    m_26c = delay;
    m_19c = delay;
}
