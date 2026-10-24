#include <cstdio>
// Behaviour test for CGraphicFrameData::Release (retail 0x0041C090): each call drops one reference; the last one
// runs Uninitialise (only when initialised), releases the counted texture (its destroy function at the last texture
// reference) and frees the frame data.
static int g_uninitialised = 0, g_texturesDestroyed = 0, g_framesFreed = 0, g_infosFreed = 0;
struct CCPPointerInfo { long RefCount; void (*DeleteFunc)(void* data); void* Data; };
static void DestroyTexture(void*) { ++g_texturesDestroyed; }
struct CCountedPointer {
    void* Data; CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; ++g_infosFreed; } }
        Data = 0; Info = 0;
    }
};
struct CGraphicFrameData {
    unsigned long ReferenceCount; void* Data; CCountedPointer PTexture; unsigned char IsInitialised;
    void Uninitialise() { ++g_uninitialised; }
    ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); }
    void Release() { if (--ReferenceCount == 0) { delete this; ++g_framesFreed; } }
};
static CGraphicFrameData* Make(unsigned long refs, bool initialised, CCPPointerInfo* info)
{
    CGraphicFrameData* f = new CGraphicFrameData;
    f->ReferenceCount = refs; f->Data = 0; f->PTexture.Data = 0; f->PTexture.Info = info; f->IsInitialised = initialised;
    return f;
}
int main()
{
    CCPPointerInfo* info = new CCPPointerInfo; info->RefCount = 2; info->DeleteFunc = DestroyTexture; info->Data = 0;
    CGraphicFrameData* a = Make(2, true, info);
    a->Release();
    int ok = g_framesFreed == 0 && g_uninitialised == 0 && a->ReferenceCount == 1;
    a->Release();                                   // last frame reference: texture info 2 -> 1, kept
    ok = ok && g_framesFreed == 1 && g_uninitialised == 1 && g_texturesDestroyed == 0 && info->RefCount == 1;
    CGraphicFrameData* b = Make(1, false, info);
    b->Release();                                   // last texture reference too
    ok = ok && g_framesFreed == 2 && g_uninitialised == 1 && g_texturesDestroyed == 1 && g_infosFreed == 1;
    CGraphicFrameData* c = Make(1, true, 0);
    c->Release();                                   // no texture
    ok = ok && g_framesFreed == 3 && g_uninitialised == 2 && g_texturesDestroyed == 1;
    if (!ok) { std::printf("0041c090_TEST FAIL\n"); return 1; }
    std::printf("0041c090_TEST PASS\n");
    return 0;
}