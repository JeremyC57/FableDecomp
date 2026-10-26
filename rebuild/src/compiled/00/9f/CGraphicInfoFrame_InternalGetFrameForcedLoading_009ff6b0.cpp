// CGraphicInfoFrame::InternalGetFrameForcedLoading(long flags) -- when the frame data (+8) has an async read (+0x30): wait for it unless finished (0x009D54E0 / 0x009D5B60), finish the load (0x009FC440), upload the frame (0x009FF220 with 1; false when that fails) and, for flag bit 3, mark the data unloadable (0x009FBF50); true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FF6B0, length 96 bytes.
// Callees: CBankFileAsyncData::IsFinished @ 0x009D54E0 and WaitUntilFinished @ 0x009D5B60, the load completion @ 0x009FC440 (labelled "CTCActivationTrigger::OnKill",
// propagated), CFrameData::UploadFrame @ 0x009FF220 (1), CResource::SetAsUnloadable @ 0x009FBF50. Shape: the frame data re-read from +8 for every call, the flag byte tested
// in the argument slot, `mov al,1` / `xor al,al`, `ret 4`.
class CBankFileAsyncData { public:
public:
    bool WaitUntilFinished();   // 0x009D5B60
public:
    bool IsFinished() const;   // 0x009D54E0
};
class CResource
{
public:
    void SetAsUnloadable();   // 0x009FBF50
};
class CManagedTextureOwner : public CResource
{
public:
};

struct CFrameData_009ff6b0 : public CManagedTextureOwner {
    unsigned char       _pad[0x30];
    CBankFileAsyncData* m_pAsyncRead;              // +0x30
    void CompleteLoad();                           // 0x009FC440
    bool UploadFrame(long flags);                  // 0x009FF220
};

class CGraphicInfoFrame { public:
    unsigned char        _pad[8];
    CFrameData_009ff6b0* m_pData;                  // +0x08
    bool InternalGetFrameForcedLoading(long flags);
};

bool CGraphicInfoFrame::InternalGetFrameForcedLoading(long flags)
{
    if (!m_pData->m_pAsyncRead)
        return false;
    if (!m_pData->m_pAsyncRead->IsFinished())
        m_pData->m_pAsyncRead->WaitUntilFinished();
    m_pData->CompleteLoad();
    if (m_pData->UploadFrame(1)) {
        if (flags & 8)
            m_pData->SetAsUnloadable();
        return true;
    }
    return false;
}