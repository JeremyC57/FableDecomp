#include <cstdio>
class CTexture;
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
    unsigned char             Alpha : 1;                 // +0x1a
    unsigned char             BooleanAlpha : 1;
    unsigned char             UsesSurface : 1;
    unsigned char             SectionOfTPage : 1;
    unsigned char             IsInitialised : 1;
    unsigned char             ScreenGraphic : 1;
    void Uninitialise();                                 // 0x009FCBA0
    ~CGraphicFrameData();
};
CGraphicFrameData::~CGraphicFrameData()
{
    if (IsInitialised)
        Uninitialise();
}

#include <new>
#include <cstring>
static int g_uninit = 0, g_deleted = 0;
void CGraphicFrameData::Uninitialise() { ++g_uninit; IsInitialised = 0; }
static void __fastcall DeleteTex(void* p) { ++g_deleted; }
int main()
{
    static unsigned char buf[sizeof(CGraphicFrameData)];
    std::memset(buf, 0, sizeof buf);
    CGraphicFrameData* f = reinterpret_cast<CGraphicFrameData*>(buf);
    CCPPointerInfo* info = new CCPPointerInfo; info->RefCount = 2; info->DeleteFunc = DeleteTex; info->Data = (void*)0x10;
    f->PTexture.Data = (CTexture*)0x10; f->PTexture.Info = info; f->IsInitialised = 1;
    f->~CGraphicFrameData();
    bool ok = g_uninit == 1 && g_deleted == 0 && info->RefCount == 1 && f->PTexture.Info == 0 && f->PTexture.Data == 0;
    f->PTexture.Data = (CTexture*)0x10; f->PTexture.Info = info; f->IsInitialised = 0;
    f->~CGraphicFrameData();
    ok = ok && g_uninit == 1 && g_deleted == 1 && f->PTexture.Info == 0;
    if (ok) std::printf("0041bdb0_TEST PASS\n");
    return 0;
}