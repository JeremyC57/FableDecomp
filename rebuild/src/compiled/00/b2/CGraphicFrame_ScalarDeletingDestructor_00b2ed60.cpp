// CGraphicFrame::ScalarDeletingDestructor(unsigned int flags) -- the frame destructor (drop the shared frame data reference, deleting the data on the last one, pointer cleared) followed by `operator delete(this)` when bit 0 of the flags is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2ED60, length 114 bytes.
// Label correction: the catalog "CGraphicFrame::UnInitialise" is propagated -- the `test byte ptr [esp+0Ch],1 ; push edi ;
// call operator delete` tail and `mov eax,edi ; ret 4` make it the deleting destructor.
// Callees: CGraphicFrameData::Uninitialise @ 0x009FCBA0, the count block's destroy (+4, object at +8 in ecx), operator
//          delete @ 0x00BFE9BC (block, data, frame).
// Shape: CGraphicFrame is not polymorphic; VC7.1 emits `??_G` for a plain class deleted with `delete` (GOTCHAS 24m), so
// the source is the `__forceinline` destructor (the landed UnInitialise body) plus a `delete` helper.
// Layout: engine/CGraphicFrame.h has FrameData as an opaque CGraphicFrameData*; the data's inline destructor chain needs
// the local model of the landed siblings (0x0041C150, 0x00B2EEC0).
// Leaf: ??_GCGraphicFrame@@
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
class CGraphicFrameData { public:                         // the shared frame data: a reference count, flags, a counted texture
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
class CGraphicFrame { public:
    CGraphicFrameData* FrameData;                  // +0x00
    __forceinline ~CGraphicFrame()
    {
        CGraphicFrameData* data = FrameData;
        if (data) {
            if (--data->ReferenceCount == 0) {
                data->~CGraphicFrameData();
                operator delete(data);
            }
            FrameData = 0;
        }
    }
};

void DeleteGraphicFrame(CGraphicFrame* frame)
{
    delete frame;
}