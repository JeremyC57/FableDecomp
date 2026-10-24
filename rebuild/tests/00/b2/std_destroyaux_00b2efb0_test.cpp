// Behaviour test for 00b2efb0 (std::__destroy_aux over CGraphicFrame): each frame drops its frame-data reference; the last reference uninitialises the data, releases its counted texture and deletes it; every frame pointer is nulled.
#include <cstdio>
#include <new>
static int g_uninitialised = 0;
static int g_texturesDestroyed = 0;
static int g_deletes = 0;
static void test_delete(void* p) { ++g_deletes; ::operator delete(p); }
struct SCountBlock {
    long  m_references;
    void (*m_pfnDestroy)(void* object);
    void* m_pObject;
};
static void DestroyTexture(void*) { ++g_texturesDestroyed; }
struct CCountedPointer_CTexture {
    void*        m_pObject;
    SCountBlock* m_pCount;
    ~CCountedPointer_CTexture()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                test_delete(m_pCount);
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
struct CGraphicFrameData {
    unsigned long            ReferenceCount;
    void*                    Data;
    CCountedPointer_CTexture PTexture;
    unsigned char            _pad10[0xa];
    unsigned char            m_flags;
    void Uninitialise() { ++g_uninitialised; }    // stands in for 0x009FCBA0
    ~CGraphicFrameData()
    {
        if (m_flags & 0x10)
            Uninitialise();
    }
};
struct CGraphicFrame {
    CGraphicFrameData* FrameData;
    ~CGraphicFrame()
    {
        CGraphicFrameData* data = FrameData;
        if (data) {
            if (--data->ReferenceCount == 0) {
                data->~CGraphicFrameData();
                test_delete(data);
            }
            FrameData = 0;
        }
    }
};
struct __false_type { };
template <class T> inline void _Destroy(T* p) { p->~T(); }
void __destroy_aux(CGraphicFrame* first, CGraphicFrame* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
static CGraphicFrameData* NewData(unsigned long refs, unsigned char flags)
{
    CGraphicFrameData* d = static_cast<CGraphicFrameData*>(::operator new(sizeof(CGraphicFrameData)));
    d->ReferenceCount = refs;
    d->Data = 0;
    d->PTexture.m_pObject = 0;
    d->PTexture.m_pCount = 0;
    d->m_flags = flags;
    return d;
}
int main()
{
    CGraphicFrameData* sole = NewData(1, 0x10);
    SCountBlock* block = static_cast<SCountBlock*>(::operator new(sizeof(SCountBlock)));
    block->m_references = 1; block->m_pfnDestroy = DestroyTexture; block->m_pObject = 0;
    sole->PTexture.m_pCount = block;
    CGraphicFrameData* shared = NewData(2, 0x10);
    union { void* p; char bytes[sizeof(CGraphicFrame) * 3]; } storage;
    CGraphicFrame* frames = reinterpret_cast<CGraphicFrame*>(storage.bytes);
    frames[0].FrameData = sole;
    frames[1].FrameData = shared;
    frames[2].FrameData = 0;
    __false_type tag;
    __destroy_aux(frames, frames, tag);
    if (g_deletes != 0 || frames[0].FrameData != sole) { std::printf("00b2efb0_TEST FAIL\n"); return 1; }
    __destroy_aux(frames, frames + 3, tag);
    if (g_uninitialised != 1 || g_texturesDestroyed != 1 || g_deletes != 2) { std::printf("00b2efb0_TEST FAIL\n"); return 1; }
    if (shared->ReferenceCount != 1 || frames[0].FrameData != 0 || frames[1].FrameData != 0) { std::printf("00b2efb0_TEST FAIL\n"); return 1; }
    ::operator delete(shared);
    std::printf("00b2efb0_TEST PASS\n");
    return 0;
}