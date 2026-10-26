// CGraphicInfoFrame::CFrameData::IsResourceLoaded() const -- loaded unless an asynchronous bank read (FileData +0x30) is still running
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FD7F0, length 20 bytes.
// Leaf: IsResourceLoaded
// Callees: CBankFileAsyncData::IsFinished @ 0x009D54E0 (thiscall, bool in al).
// Name: vtable 0x0129C89C slot 4 (rtti-vtables.tsv). The CResource virtuals in FableWin.pdb are IsResourceLoaded,
// OnResourceLoaded, ... and slot 5 is the landed CFrameData::OnResourceLoaded (0x009FF860), so slot 4 is the
// CFrameData override ?IsResourceLoaded@CFrameData@CGraphicInfoFrame@@UBE_NXZ (const, bool). Layout: Ego_r.pdb
// CFrameData (0x44): FileData (CCountedPointer<CBankFileAsyncData>, object word) at +0x30, as the landed
// StopLoading (0x009FDC20) and GetUnloadableMemory (0x009FD830).
// Shape: `if (p && !p->IsFinished()) return false; return true;` -- the `||` expression form gives the 32-bit
// `xor eax,eax` / `mov eax,1` pair instead; here the null test jumps to the shared `mov al,1`, a finished read jumps there
// too, and the unfinished case returns the call's own zero al (`ret` straight after the `jne`).
class CBankFileAsyncData { public:
    bool IsFinished();                     // 0x009D54E0
};

struct CCountedPointer_BankFileAsyncData {
    CBankFileAsyncData* m_pObject;         // +0
    void*               m_pCount;          // +4
};

struct CFrameData {
    unsigned char                     _pad[0x30];
    CCountedPointer_BankFileAsyncData FileData;    // +0x30
    bool IsResourceLoaded() const;
};

bool CFrameData::IsResourceLoaded() const
{
    if (FileData.m_pObject && !FileData.m_pObject->IsFinished())
        return false;
    return true;
}