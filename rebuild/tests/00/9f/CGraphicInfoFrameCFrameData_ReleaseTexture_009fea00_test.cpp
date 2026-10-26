// Behaviour test for 009fea00 CGraphicInfoFrame::CFrameData::ReleaseTexture (copy of the source model).
#include <cstdio>

class CBankFileAsyncData { public: int dummy; };
static int g_unloads = 0;
static unsigned long g_lastFrame = 0;
static const void* g_lastInfo = 0;
class CGraphicInfo { public:
    int dummy;
    void UnloadFrame(unsigned long frameNum) { ++g_unloads; g_lastFrame = frameNum; g_lastInfo = this; }
};
class CGraphicInfoFrame { public:
    void*          vtable;
    unsigned short FrameNum;
    signed char    SmallMipmapLevel;
    void*          Data;
    CGraphicInfo*  GraphicInfo;
};
struct CCountedPointer_BankFileAsyncData {
    CBankFileAsyncData* m_pObject;
    void*               m_pCount;
};
struct CFrameData {
    unsigned char                     _pad[0x30];
    CCountedPointer_BankFileAsyncData FileData;
    signed char                       CurrentMipmapLevel;
    signed char                       LoadingMipmapLevel;
    signed char                       TouchedMipmapLevel;
    unsigned long                     TouchedFrame;
    CGraphicInfoFrame*                GraphicInfoFrame;
    void ReleaseTexture();
};

void CFrameData::ReleaseTexture()
{
    if (!FileData.m_pObject)
        GraphicInfoFrame->GraphicInfo->UnloadFrame(GraphicInfoFrame->FrameNum);
}

int main()
{
    CGraphicInfo info;
    CGraphicInfoFrame frame; frame.vtable = 0; frame.FrameNum = 0xBEEF; frame.Data = 0; frame.GraphicInfo = &info;
    CFrameData f; f.GraphicInfoFrame = &frame; f.FileData.m_pCount = 0;
    CBankFileAsyncData read;
    int ok = 1;
    f.FileData.m_pObject = &read;
    f.ReleaseTexture();
    if (g_unloads != 0) ok = 0;                                 // read pending: nothing
    f.FileData.m_pObject = 0;
    f.ReleaseTexture();
    if (g_unloads != 1 || g_lastFrame != 0xBEEFul || g_lastInfo != &info) ok = 0;   // zero-extended frame number
    if (!ok) { std::printf("009fea00_TEST FAIL\n"); return 1; }
    std::printf("009fea00_TEST PASS\n");
    return 0;
}