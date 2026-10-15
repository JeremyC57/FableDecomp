// CFrameDataHandle::Clear() -- true when empty; otherwise, when the frame data is shared by exactly two holders (count == 2), close it (0x00A69A80) and report true; then release the handle
// Grade: DIFFER(ours v retail) 70v73 -- retail re-reads the member for the second null test (`mov eax,[esi+4] ; test eax,eax ; je ; cmp [eax+4],2`) while keeping the first load in ecx for the Close call; ours tests the cached ecx (`test ecx,ecx ; je ; cmp [ecx+4],2`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A69AD0, length 73 bytes (+ int3 padding). Callers: CFrameData::StopLoading/BeginLoading
// (0x009FDC20, 0x009FDE80), ~CFrameData (0x00B2B910) and others (11 sites). The catalog label "CViewport::ClearHandle" is propagated.
// Leaf: Clear
// Callees: the frame data's Close @ 0x00A69A80 (clears its load state and frees its buffers), the counted object's virtual slot 1 (destroy).
// Shape: early `mov al,1 ; ret` for the empty handle, the result flag in bl, the CIVCountedPointer release with its double clear.
// Tried: `m_p && m_p->m_count == 2` (the DDK folds the second null test away), an inline bool `IsPairOwned()` (same), a `GetRefCount()`
// ternary accessor (same), an object local for the call plus the accessor (this file: re-tests, but through the cached register).
// Next: a volatile-cast read for the second test only, or a lane gap (RTM re-loads a member the QFE compiler CSEs).
struct CFrameData {
    virtual void v00();
    virtual void Destroy();                     // slot 1 (+0x04)
    long m_count;                               // +0x04
    void Close();                               // 0x00A69A80
};
struct CFrameDataHandle {
    void*           m_owner;                    // +0x00
    CFrameData* m_p;                        // +0x04
    void Reset()
    {
        CFrameData* cur = m_p;
        if (cur) {
            if (--cur->m_count == 0)
                cur->Destroy();
            m_p = 0;
        }
        m_p = 0;
    }
    long GetRefCount() const { return m_p ? m_p->m_count : 0; }
    bool Clear();
};
bool CFrameDataHandle::Clear()
{
    CFrameData* object = m_p;
    if (!object)
        return true;
    bool closed = false;
    if (GetRefCount() == 2) {
        object->Close();
        closed = true;
    }
    Reset();
    return closed;
}
