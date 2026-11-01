// Behaviour test for CGraphicInfoFrame::CFrameData::~CFrameData (0x009FE6C0): ResetTexture (Release the managed texture,
// UnInitialise the frame, reset the mipmap levels), then FileData released, both managed textures destroyed, the frame
// (already empty) and the CResource base.
#include <cstdio>
static char g_log[64];
static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
static int g_fileDeletes = 0;
static void DeleteFile(void*) { ++g_fileDeletes; Log('D'); }
class CCPPointerInfo { public: unsigned int RefCount; void (*DeleteFunc)(void*); void* Data; };
struct CCountedFile {
    void* Data; CCPPointerInfo* Info;
    ~CCountedFile() { if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } } Data = 0; Info = 0; }
};
class CGraphicFrameData { public: unsigned int ReferenceCount; };
class CGraphicFrame { public:
    CGraphicFrameData* FrameData;
    void UnInitialise() { Log('U'); if (FrameData) { if (--FrameData->ReferenceCount == 0) delete FrameData; FrameData = 0; } }
    ~CGraphicFrame() { if (FrameData) Log('X'); }
};
class CManagedTexture { public: char tag; bool Release() { Log('R'); return true; } ~CManagedTexture() { Log(tag); } };
class CResource { public: ~CResource() { Log('Z'); } };
struct CFrameData : CResource {
    CGraphicFrame Frame;
    CManagedTexture ManagedTexture, LoadingManagedTexture;
    CCountedFile FileData;
    signed char CurrentMipmapLevel, LoadingMipmapLevel, TouchedMipmapLevel;
    void ResetTexture() { ManagedTexture.Release(); Frame.UnInitialise(); CurrentMipmapLevel = -1; TouchedMipmapLevel = 127; }
    ~CFrameData() { ResetTexture(); }
};
int main()
{
    CFrameData* f = new CFrameData;
    f->ManagedTexture.tag = 'm'; f->LoadingManagedTexture.tag = 'l';
    f->Frame.FrameData = new CGraphicFrameData; f->Frame.FrameData->ReferenceCount = 1;
    f->FileData.Data = 0; f->FileData.Info = new CCPPointerInfo; f->FileData.Info->RefCount = 1; f->FileData.Info->DeleteFunc = DeleteFile; f->FileData.Info->Data = 0;
    f->ResetTexture();
    if (f->CurrentMipmapLevel != -1 || f->TouchedMipmapLevel != 127 || f->Frame.FrameData) { std::printf("009fe6c0_TEST FAIL\n"); return 1; }
    g_n = 0; g_log[0] = 0;
    delete f;
    const char* want = "RUDlmZ";
    for (int i = 0; want[i] || g_log[i]; ++i) if (want[i] != g_log[i]) { std::printf("009fe6c0_TEST FAIL\n"); return 1; }
    if (g_fileDeletes != 1) { std::printf("009fe6c0_TEST FAIL\n"); return 1; }
    std::printf("009fe6c0_TEST PASS\n");
    return 0;
}