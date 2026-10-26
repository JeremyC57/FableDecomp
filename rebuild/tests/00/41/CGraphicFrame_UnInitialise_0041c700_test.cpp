#include <cstdio>
class CCPPointerInfo { public:
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
class CGraphicFrame {
public:
    CGraphicFrameData* FrameData;                        // +0x00
    void UnInitialise();
};
void CGraphicFrame::UnInitialise()
{
    CGraphicFrameData* data = FrameData;
    if (data)
    {
        if (--data->ReferenceCount == 0)
            { data->~CGraphicFrameData(); ::operator delete(data); }
        FrameData = 0;
    }
}

static int g_uninit, g_texDel, g_frees;
void CGraphicFrameData::Uninitialise() { ++g_uninit; }
static void __fastcall TexDelete(void*) { ++g_texDel; }
int main()
{
    CCPPointerInfo* info = new CCPPointerInfo; info->RefCount = 1; info->DeleteFunc = TexDelete; info->Data = 0;
    CGraphicFrameData* d = (CGraphicFrameData*)::operator new(sizeof(CGraphicFrameData));
    d->ReferenceCount = 2; d->PTexture.Data = 0; d->PTexture.Info = info; *((unsigned char*)d + 0x1a) = 0; d->IsInitialised = 1;
    CGraphicFrame a, b; a.FrameData = d; b.FrameData = d;
    a.UnInitialise();
    bool ok = a.FrameData == 0 && d->ReferenceCount == 1 && g_uninit == 0 && g_texDel == 0;
    b.UnInitialise();
    ok = ok && b.FrameData == 0 && g_uninit == 1 && g_texDel == 1;
    CGraphicFrame c; c.FrameData = 0; c.UnInitialise();
    ok = ok && c.FrameData == 0 && g_uninit == 1;
    std::printf(ok ? "0041c700_TEST PASS\n" : "0041c700_TEST FAIL\n");
    return ok ? 0 : 1;
}