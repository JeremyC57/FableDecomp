#include <cstdio>
struct CCPPointerInfo {
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info)
        {
            --Info->RefCount;
            if (Info->RefCount == 0)
            {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CTexture;
class CGraphicFrameData {
public:
    unsigned long             ReferenceCount;            // +0x00
    void*                     Data;                      // +0x04
    CCountedPointer<CTexture> PTexture;                  // +0x08
    unsigned short            TLX;                       // +0x10
    unsigned short            TLY;                       // +0x12
    unsigned short            Width;                     // +0x14
    unsigned short            Height;                    // +0x16
    unsigned short            Pitch;                     // +0x18
    unsigned char             Alpha : 1;                 // +0x1a bit 0
    unsigned char             BooleanAlpha : 1;          // bit 1
    unsigned char             UsesSurface : 1;           // bit 2
    unsigned char             SectionOfTPage : 1;        // bit 3
    unsigned char             IsInitialised : 1;         // bit 4
    unsigned char             ScreenGraphic : 1;         // bit 5
    void Uninitialise();                                 // 0x009FCBA0
    ~CGraphicFrameData()
    {
        if (IsInitialised)
            Uninitialise();
    }
};
void DeleteGraphicFrameData(CGraphicFrameData* frame)    // emission helper, not a retail function
{
    delete frame;
}

static int g_uninit, g_deleted_items;
void CGraphicFrameData::Uninitialise() { ++g_uninit; IsInitialised = 0; }
static void __fastcall DeleteItem(void*) { ++g_deleted_items; }
int main()
{
    CGraphicFrameData* f = new CGraphicFrameData;
    f->Alpha = 0; f->BooleanAlpha = 0; f->UsesSurface = 0; f->SectionOfTPage = 0; f->IsInitialised = 1; f->ScreenGraphic = 0;
    f->PTexture.Data = 0;
    f->PTexture.Info = new CCPPointerInfo; f->PTexture.Info->RefCount = 1; f->PTexture.Info->DeleteFunc = DeleteItem; f->PTexture.Info->Data = 0;
    DeleteGraphicFrameData(f);
    bool ok = g_uninit == 1 && g_deleted_items == 1;
    CGraphicFrameData* g = new CGraphicFrameData;
    g->IsInitialised = 0; g->PTexture.Data = 0;
    CCPPointerInfo* shared = new CCPPointerInfo; shared->RefCount = 2; shared->DeleteFunc = DeleteItem; shared->Data = 0;
    g->PTexture.Info = shared;
    DeleteGraphicFrameData(g);
    ok = ok && g_uninit == 1 && g_deleted_items == 1 && shared->RefCount == 1 && sizeof(CGraphicFrameData) == 0x1c;
    std::printf(ok ? "0041bdf0_TEST PASS\n" : "0041bdf0_TEST FAIL\n");
    return ok ? 0 : 1;
}