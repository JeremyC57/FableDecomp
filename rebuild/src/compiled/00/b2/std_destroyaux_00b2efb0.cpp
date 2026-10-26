// std::__destroy_aux(CGraphicFrame* first, CGraphicFrame* last, const __false_type&) -- run the inlined ~CGraphicFrame over [first, last): drop each shared frame-data reference, deleting the data on the last one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2EFB0, length 104 bytes (padded to 112).
// Leaf: __destroy_aux
// Label: the catalog "_Destroy_range<std::allocator<CGraphicFrame>>" names the element correctly (4-byte stride, the
// landed ~CGraphicFrame chain of 0x00B2ED60 inline); as an STLport out-of-line body it is `__destroy_aux(first, last,
// const __false_type&)` (callers 0x00B2D98B CEngineSurfaceCompositionManager::UpdateDiskCacheTexture and five more).
// Convention: LTCG register form -- first in ecx, last in edx, the tag pointer on the stack (`ret 4`).
// Callees: CGraphicFrameData::Uninitialise @ 0x009FCBA0, the count block's destroy (+4, object at +8 in ecx), operator
//          delete @ 0x00BFE9BC (texture count block, frame data).
// Layout: engine/CGraphicFrame.h has FrameData as an opaque CGraphicFrameData*; the data's inline destructor chain uses
// the local model of the landed siblings (0x00B2ED60, 0x0041C150): ReferenceCount +0, PTexture {+8, +0xc}, flags +0x1a.
// Shape: the landed __forceinline destructors unchanged; one zero register (ebp) serves every null test and store.
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
namespace std {
struct __false_type { };
template <class T> inline void _Destroy(T* p) { p->~T(); }
void __fastcall __destroy_aux(CGraphicFrame* first, CGraphicFrame* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
}