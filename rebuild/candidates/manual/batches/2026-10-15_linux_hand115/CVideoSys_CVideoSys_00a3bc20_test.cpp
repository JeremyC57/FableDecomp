#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
#include <stdlib.h>
static int g_rendererDeleted = 0, g_coUninit = 0, g_wideDtor = 0, g_cleared = 0, g_freed = 0; static void* g_freedPtr = 0;
static void CoUninitialize() { ++g_coUninit; }
static void MyFree(void* p) { ++g_freed; g_freedPtr = p; }
struct CTextureRenderer {
    virtual long __stdcall QueryInterface(void*, void**) { return 0; }
    virtual unsigned long __stdcall AddRef() { return 1; }
    virtual unsigned long __stdcall Release() { return 0; }
    virtual ~CTextureRenderer() { ++g_rendererDeleted; }
};
struct CWideString { void* m_pData; ~CWideString() { ++g_wideDtor; } };
struct CSubtitleList { void* m_pHead; long m_count; void Clear() { ++g_cleared; } ~CSubtitleList() { Clear(); if (m_pHead) MyFree(m_pHead); } };
struct CMovieSubtitles { CSubtitleList m_subtitles; unsigned char _pad08[0xc]; CWideString m_text; ~CMovieSubtitles() { } };
struct CVideoSys {
    void* GBuilder; void* MControl; void* MPosition; void* MSeeking; void* MEvent; void* BasicAudio; void* PAsfReader; void* PFileSource;
    CTextureRenderer* TexRenderer; long PlayMode; long State; unsigned char RenderVideo; unsigned char _pad2d[3]; CMovieSubtitles Subtitles;
    ~CVideoSys() { if (TexRenderer) { delete TexRenderer; TexRenderer = 0; } CoUninitialize(); }
};
int main()
{
    int head;
    {
        CVideoSys v; v.TexRenderer = new CTextureRenderer; v.Subtitles.m_subtitles.m_pHead = &head; v.Subtitles.m_subtitles.m_count = 0;
    }
    if (g_rendererDeleted != 1 || g_coUninit != 1 || g_wideDtor != 1 || g_cleared != 1 || g_freed != 1 || g_freedPtr != &head) { std::printf("00a3bc20_TEST FAIL\n"); return 1; }
    {
        CVideoSys v; v.TexRenderer = 0; v.Subtitles.m_subtitles.m_pHead = 0;
    }
    if (g_rendererDeleted != 1 || g_coUninit != 2 || g_wideDtor != 2 || g_cleared != 2 || g_freed != 1) { std::printf("00a3bc20_TEST FAIL\n"); return 1; }
    std::printf("00a3bc20_TEST PASS\n"); return 0;
}
