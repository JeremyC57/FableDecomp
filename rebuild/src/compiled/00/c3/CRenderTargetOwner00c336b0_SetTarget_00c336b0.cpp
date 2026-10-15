// CRenderTargetOwner_00c336b0::SetTarget(void* target) -- forward the target to the sink (+0x1d4, virtual slot 7) and, while active (+0x180), store it into the view (+0x58) and refresh the view
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C336B0, length 52 bytes (+ int3 padding). No direct callers (virtual).
// Leaf: SetTarget
// The catalog label "CVolume::operator=" is propagated: the body reads +0x58/+0x180/+0x1d4 and returns nothing (eax is not `this`).
// Shape: the argument in edi for both uses; the view pointer is ONE named local (`view->m_target = target; view->Refresh();`
// loads it once and fetches the vtable before the store -- two member reads reload it after the store, which may alias).
struct CTargetSink {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void SetTarget(void* target);   // slot 7 (+0x1c)
};
struct CTargetView {
    virtual void v00();
    virtual void Refresh();                 // slot 1 (+0x04)
    unsigned char _pad04[0x18];
    void* m_target;                         // +0x1c
};
struct CRenderTargetOwner_00c336b0 {
    unsigned char _pad00[0x58];
    CTargetView*  m_pView;                  // +0x58
    unsigned char _pad5c[0x180 - 0x5c];
    void*         m_pActive;                // +0x180
    unsigned char _pad184[0x1d4 - 0x184];
    CTargetSink*  m_pSink;                  // +0x1d4
    void SetTarget(void* target);
};
void CRenderTargetOwner_00c336b0::SetTarget(void* target)
{
    if (m_pSink)
        m_pSink->SetTarget(target);
    if (m_pActive) {
        CTargetView* view = m_pView;
        view->m_target = target;
        view->Refresh();
    }
}