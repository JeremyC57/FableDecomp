// Behaviour test for CGraphicInfoFrame::GetVolumeTexture @ 0x009FF710 (copy of the model with stub loading and LRU list).
#include <cstdio>
static int g_waits, g_uploads, g_touches, g_begins;
struct CBankFileAsyncData { bool done; bool IsFinished() { return done; } void WaitUntilFinished() { ++g_waits; done = true; } };
struct CResourceList { unsigned long Stamp; };
struct CResource {
    CResourceList* List; unsigned long Stamp;
    void Touch() { if (List && Stamp != List->Stamp) { ++g_touches; Stamp = List->Stamp; } }
};
struct SCountBlock { long refs; };
struct CVolumeTexture { int id; };
struct CTexturePointer {
    CVolumeTexture* m_pObject; SCountBlock* m_pCount;
    CTexturePointer() : m_pObject(0), m_pCount(0) {}
    CTexturePointer(const CTexturePointer& o) : m_pObject(o.m_pObject), m_pCount(o.m_pCount) { if (m_pCount) ++m_pCount->refs; }
    ~CTexturePointer() { if (m_pCount) --m_pCount->refs; }
};
struct CVolumeResource : CResource { CTexturePointer Texture; };
struct CVolumeHolder { CVolumeResource* Volume; };
struct CFrameData : CResource {
    CVolumeHolder VolumeHolder; CBankFileAsyncData* AsyncRead; signed char SmallestResidentMip, LowestRequestedMip; unsigned long LastFrameStamp;
    void CompleteLoad() { AsyncRead = 0; } bool UploadFrame(bool) { ++g_uploads; return true; }
};
static unsigned long g_frameStamp;
struct CGraphicInfoFrame {
    CFrameData* Data;
    void BeginLoading(unsigned long, unsigned long) { ++g_begins; }
    bool GetVolumeTexture(CVolumeTexture** out, unsigned long mipLevel)
    {
        BeginLoading(mipLevel, 0);
        if (Data && Data->AsyncRead) { if (!Data->AsyncRead->IsFinished()) Data->AsyncRead->WaitUntilFinished(); Data->CompleteLoad(); Data->UploadFrame(true); }
        if (Data && Data->VolumeHolder.Volume) {
            unsigned long stamp = g_frameStamp;
            if (Data->LastFrameStamp != stamp || mipLevel <= (unsigned long)Data->LowestRequestedMip) {
                Data->LastFrameStamp = stamp;
                CVolumeHolder& holder = Data->VolumeHolder;
                if (holder.Volume) holder.Volume->Touch();
                if (mipLevel <= (unsigned long)Data->SmallestResidentMip) Data->Touch();
                if ((unsigned long)Data->LowestRequestedMip > mipLevel) Data->LowestRequestedMip = (signed char)mipLevel;
            }
            CVolumeTexture* result = CTexturePointer(Data->VolumeHolder.Volume->Texture).m_pObject;
            if (result) { *out = result; return true; }
        }
        return false;
    }
};
int main()
{
    int bad = 0; CResourceList list; list.Stamp = 7; CVolumeTexture tex; tex.id = 1; SCountBlock block; block.refs = 1;
    CVolumeResource vol; vol.List = &list; vol.Stamp = 0; vol.Texture.m_pObject = &tex; vol.Texture.m_pCount = &block;
    CBankFileAsyncData read; read.done = false;
    CFrameData data; data.List = &list; data.Stamp = 0; data.VolumeHolder.Volume = &vol; data.AsyncRead = &read;
    data.SmallestResidentMip = 2; data.LowestRequestedMip = 5; data.LastFrameStamp = 0;
    CGraphicInfoFrame frame; frame.Data = &data; g_frameStamp = 3; CVolumeTexture* out = 0;
    if (!frame.GetVolumeTexture(&out, 1) || out != &tex || g_waits != 1 || g_uploads != 1 || g_touches != 2 || data.LowestRequestedMip != 1 || block.refs != 1) bad++;
    out = 0; g_touches = 0;
    if (!frame.GetVolumeTexture(&out, 4) || out != &tex || g_touches != 0) bad++;                 // same stamp, above the requested mip: no refresh
    vol.Texture.m_pObject = 0; out = 0; if (frame.GetVolumeTexture(&out, 4) || out) bad++;
    data.VolumeHolder.Volume = 0; if (frame.GetVolumeTexture(&out, 0)) bad++;
    frame.Data = 0; if (frame.GetVolumeTexture(&out, 0) || g_begins != 5) bad++;
    if (bad) { std::printf("009ff710_TEST FAIL\n"); return 1; }
    std::printf("009ff710_TEST PASS\n");
    return 0;
}
