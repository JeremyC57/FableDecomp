// CGraphicFrame::UnInitialise_00a00b90() -- drop this frame's reference to its shared frame data (+0): when the count reaches zero, delete the data (its inline destructor: Uninitialise when the initialised bit is set, the counted texture released) and clear the pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A00B90, length 94 bytes.
// Callees: CGraphicFrameData::Uninitialise @ 0x009FCBA0, the count block's destroy (slot +4 with the object at +8 in ecx), operator delete @ 0x00BFE9BC twice (the block,
// then the data). Shape: `dec dword ptr [esi] ; jne` on the shared count, the inlined destructor chain, the pointer cleared on both arms. One of three byte-identical copies (0x0041C150, 0x00A00B90, 0x00B2EEC0).
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct SCountBlock_00a00b90 {
    long  m_references;                            // +0x00
    void (__fastcall* m_pfnDestroy)(void* object); // +0x04
    void* m_pObject;                               // +0x08
};
struct CCountedPointer_00a00b90 {                     // 8 bytes: the object and its count block
    void*                 m_pObject;               // +0x00
    SCountBlock_00a00b90*    m_pCount;                // +0x04
    __forceinline ~CCountedPointer_00a00b90()
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
struct CGraphicFrameData_00a00b90 {                   // the shared frame data: a reference count, flags, a counted texture
    long                    m_references;          // +0x00
    long                    m_4;
    CCountedPointer_00a00b90   m_texture;             // +0x08
    unsigned char           _pad10[0xa];
    unsigned char           m_flags;               // +0x1a (bit 4 = initialised)
    void Uninitialise();                           // 0x009FCBA0
    __forceinline ~CGraphicFrameData_00a00b90()
    {
        if (m_flags & 0x10)
            Uninitialise();
    }
};

struct CGraphicFrame {
    CGraphicFrameData_00a00b90* m_pData;                // +0x00
    void UnInitialise_00a00b90();
};

void CGraphicFrame::UnInitialise_00a00b90()
{
    CGraphicFrameData_00a00b90* data = m_pData;
    if (data) {
        if (--data->m_references == 0) {
            data->~CGraphicFrameData_00a00b90();          // the inline destructor chain (a `delete` would go through the out-of-line deleting destructor)
            operator delete(data);
        }
        m_pData = 0;
    }
}