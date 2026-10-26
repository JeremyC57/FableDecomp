// CFrameData::StopLoading() -- if an asynchronous file read is attached (FileData +0x30): fail (false) while it is unfinished, else drop the counted read and clear the loading texture's handle; then re-publish the frame's resource size and return true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FDC20, length 136 bytes (+ int3 padding).
// Callees: CBankFileAsyncData::IsFinished @ 0x009D54E0, the count block's destroy function (`mov ecx,[eax+8] ; call [eax+4]`),
// operator delete @ 0x00BFE9BC, CManagedTexture::ClearHandle @ 0x00A69AD0 (label "CViewport::ClearHandle" propagated) on
// LoadingManagedTexture (+0x28), the inlined CFrameData::Sizeof (landed 0x009FDBE0: CGraphicFrameData::Sizeof @ 0x009FCBF0 and
// CManagedTexture::Sizeof @ 0x00A69E60), CResource::SetResourceSize(unsigned long) @ 0x009FBF20 (on `this`).
// Layout: Ego_r.pdb CFrameData (0x44): Frame data pointer +0x1c, LoadingManagedTexture +0x28 (its data word +0x2c), FileData
// (CCountedPointer<CBankFileAsyncData>: object +0x30, count block +0x34), as the landed Sizeof / OnResourceLoaded.
// Shape: the unfinished case is an `else return false;` arm placed after the body (its own `xor al,al`; an early
// `if (!IsFinished()) return false;` returns the call's zero al inline); `FileData = 0` is the inlined CCountedPointer reset (block decremented through the member and re-read, destroyed and freed
// on zero, then block and object zero stores); the Sizeof call inlined exactly as its landed body (`lea edi,[eax+44h]`).
void __fastcall Destroy_CBankFileAsyncData(void* object);   // the per-type deleter stored in the block

struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
};

class CBankFileAsyncData { public:
    bool IsFinished();                     // 0x009D54E0
};

struct CCountedPointer_BankFileAsyncData {
    CBankFileAsyncData* m_pObject;         // +0
    SCountBlock*        m_pCount;          // +4
    void Reset()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pCount = 0;
        m_pObject = 0;
    }
};

class CGraphicFrameData { public:
    long Sizeof();                         // 0x009FCBF0
};
class CManagedTexture { public:
    long  m_0;
    void* m_pData;                         // +0x04
    bool HasData() const { return m_pData != 0; }
    long Sizeof();                         // 0x00A69E60
    void ClearHandle();                    // 0x00A69AD0
};

struct CFrameData {
    unsigned char                     _pad[0x1c];
    CGraphicFrameData*                m_pGraphic;              // +0x1c
    unsigned char                     _pad20[8];
    CManagedTexture                   LoadingManagedTexture;   // +0x28
    CCountedPointer_BankFileAsyncData FileData;                // +0x30
    long Sizeof()                                              // 0x009FDBE0, inlined
    {
        long size = (m_pGraphic ? m_pGraphic->Sizeof() + 4 : 4) + 0x44;
        if (LoadingManagedTexture.HasData())
            size += LoadingManagedTexture.Sizeof();
        return size;
    }
    void SetResourceSize(unsigned long size);                 // 0x009FBF20 (CResource)
    bool StopLoading();
};

bool CFrameData::StopLoading()
{
    if (FileData.m_pObject) {
        if (FileData.m_pObject->IsFinished()) {
            FileData.Reset();
            LoadingManagedTexture.ClearHandle();
        } else {
            return false;
        }
    }
    SetResourceSize(Sizeof());
    return true;
}