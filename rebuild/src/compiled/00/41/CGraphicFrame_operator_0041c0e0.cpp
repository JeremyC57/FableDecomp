// CGraphicFrame::operator=(const CGraphicFrame& other) -- drops this frame's reference to its shared frame data (deleting the data on the last reference), then shares the other frame's data (reference count bumped)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041C0E0, length 104 bytes (padded to 112).
// Label correction: the catalog "CGraphicFrame::UnInitialise" is propagated -- the body takes a frame by reference
// (`ret 4`), adopts its data pointer with `inc [eax]` and returns `this`: the copy assignment.
// Callees: CGraphicFrameData::Uninitialise @ 0x009FCBA0, the count block's destroy (+4, object at +8 in ecx), operator
//          delete @ 0x00BFE9BC (the block, then the data).
// Shape: the landed UnInitialise (0x0041C150) release without the pointer clear (the new pointer is stored straight
// away), then `m_pData = other.m_pData; if (m_pData) ++m_pData->ReferenceCount;`.
// Layout: engine/CGraphicFrame.h has FrameData as an opaque CGraphicFrameData*; the data's inline destructor chain needs
// the local model of the landed siblings (0x0041C150, 0x00B2EEC0).
// Leaf: ??4CGraphicFrame
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct SCountBlock {
    long  m_references;                            // +0x00
    void (__fastcall* m_pfnDestroy)(void* object); // +0x04
    void* m_pObject;                               // +0x08
};
struct CCountedPointer_CTexture {                  // 8 bytes: the object and its count block
    void*        m_pObject;                        // +0x00
    SCountBlock* m_pCount;                         // +0x04
    __forceinline ~CCountedPointer_CTexture()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
struct CGraphicFrameData {                         // the shared frame data: a reference count, flags, a counted texture
    unsigned long            ReferenceCount;       // +0x00
    void*                    Data;                 // +0x04
    CCountedPointer_CTexture PTexture;             // +0x08
    unsigned char            _pad10[0xa];
    unsigned char            m_flags;              // +0x1a (bit 4 = IsInitialised)
    void Uninitialise();                           // 0x009FCBA0
    __forceinline ~CGraphicFrameData()
    {
        if (m_flags & 0x10)
            Uninitialise();
    }
};
struct CGraphicFrame {
    CGraphicFrameData* FrameData;                  // +0x00
    CGraphicFrame& operator=(const CGraphicFrame& other);
};

CGraphicFrame& CGraphicFrame::operator=(const CGraphicFrame& other)
{
    CGraphicFrameData* data = FrameData;
    if (data && --data->ReferenceCount == 0) {
        data->~CGraphicFrameData();                // the inline destructor chain (no out-of-line deleting destructor)
        operator delete(data);
    }
    FrameData = other.FrameData;
    if (FrameData)
        ++FrameData->ReferenceCount;
    return *this;
}