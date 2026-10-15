#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
#include <new>
static int g_coInit = 0, g_subs = 0; static void* g_coArg = (void*)1;
static long CoInitialize(void* r) { ++g_coInit; g_coArg = r; return 0; }
struct CMovieSubtitles { unsigned char _body[0x2c]; CMovieSubtitles() { ++g_subs; } };
struct CVideoSys {
    void* GBuilder; void* MControl; void* MPosition; void* MSeeking; void* MEvent; void* BasicAudio; void* PAsfReader; void* PFileSource;
    void* TexRenderer; long PlayMode; long State; unsigned char RenderVideo; unsigned char _pad2d[3]; CMovieSubtitles Subtitles;
    unsigned char SubtitlesEnabled; unsigned char _pad5d[3]; void* pTextBank; unsigned char CriticalSection[0x18]; void* RenderEvent; void* PRenderTexture; long Height; long Width;
    CVideoSys() : PAsfReader(0), PFileSource(0), Subtitles(), SubtitlesEnabled(1), pTextBank(0), RenderEvent(0), Height(0), Width(0)
    { GBuilder = 0; MControl = 0; MPosition = 0; MSeeking = 0; MEvent = 0; BasicAudio = 0; TexRenderer = 0; PlayMode = 1; State = 0; RenderVideo = 1; CoInitialize(0); }
};
int main()
{
    unsigned char raw[sizeof(CVideoSys)]; for (unsigned k = 0; k < sizeof(raw); ++k) raw[k] = 0xAA;
    CVideoSys* v = new (raw) CVideoSys();
    bool ok = g_coInit == 1 && g_coArg == 0 && g_subs == 1 && v->GBuilder == 0 && v->MControl == 0 && v->MPosition == 0 && v->MSeeking == 0 && v->MEvent == 0
        && v->BasicAudio == 0 && v->PAsfReader == 0 && v->PFileSource == 0 && v->TexRenderer == 0 && v->PlayMode == 1 && v->State == 0 && v->RenderVideo == 1
        && v->SubtitlesEnabled == 1 && v->pTextBank == 0 && v->RenderEvent == 0 && v->Height == 0 && v->Width == 0 && v->PRenderTexture != 0;
    if (!ok) { std::printf("00a3bc70_TEST FAIL\n"); return 1; }
    std::printf("00a3bc70_TEST PASS\n"); return 0;
}