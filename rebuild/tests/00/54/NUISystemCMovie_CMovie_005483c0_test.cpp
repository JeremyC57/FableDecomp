// Behaviour test for NUISystem::CMovie::~CMovie @ 0x005483C0: the movie clears its primitive handle and its parent viewport's
// handle; when it is the movie playing it closes the video system (only if playing), forgets itself, frees the movie texture
// from the engine pools and deletes the video system. A movie that is not the playing one leaves the statics alone.
#include <stdio.h>
static int g_handleClear = 0, g_viewportClear = 0, g_close = 0, g_videoDtor = 0, g_freeTex = 0, g_baseDtor = 0;
static void* g_freedTexture = 0;
class CTexture { public: int Id; };
class CVideoSys { public: void Close() { g_close++; } ~CVideoSys() { g_videoDtor++; } };
CVideoSys* g_pVideoSys = 0;
class CIEngine { public: void FreeTextureFromEngineResourcePools(CTexture* t) { g_freeTex++; g_freedTexture = t; } };
class CEnginePrimitiveHandle { public: void Clear() { g_handleClear++; } };
namespace NUISystem {
class CComponent { public:
    virtual ~CComponent() { g_baseDtor++; }
    virtual void ClearHandle() { g_viewportClear++; }
    CComponent* PParentViewport;
};
class CMovie : public CComponent { public:
    bool                   Playing;
    CEnginePrimitiveHandle Handle;
    static CMovie*         PlayingMovie;
    static CIEngine*       PEngine;
    static CTexture*       MovieTexture;
    virtual ~CMovie();
protected:
    void FreeTextures()
    {
        if (MovieTexture) {
            PEngine->FreeTextureFromEngineResourcePools(MovieTexture);
            MovieTexture = 0;
        }
    }
};
CMovie* CMovie::PlayingMovie = 0;
CIEngine* CMovie::PEngine = 0;
CTexture* CMovie::MovieTexture = 0;
CMovie::~CMovie()
{
    Handle.Clear();
    if (PParentViewport)
        PParentViewport->ClearHandle();
    if (PlayingMovie == this) {
        if (Playing)
            g_pVideoSys->Close();
        PlayingMovie = 0;
        FreeTextures();
        delete g_pVideoSys;
        g_pVideoSys = 0;
    }
}
}
using NUISystem::CMovie;
static int fail(int line) { printf("005483c0_TEST FAIL\nline %d\n", line); return 1; }
int main()
{
    CIEngine engine; CTexture tex; CMovie::PEngine = &engine;
    NUISystem::CComponent viewport; viewport.PParentViewport = 0;
    // a movie that is not playing: only the handles are cleared
    CMovie* other = new CMovie; other->PParentViewport = &viewport; other->Playing = true;
    CMovie* playing = new CMovie; playing->PParentViewport = 0; playing->Playing = true;
    CMovie::PlayingMovie = playing; CMovie::MovieTexture = &tex; g_pVideoSys = new CVideoSys;
    delete other;
    if (g_handleClear != 1 || g_viewportClear != 1 || g_close != 0 || g_videoDtor != 0 || CMovie::PlayingMovie != playing)
        return fail(__LINE__);
    // the playing movie: close, free the texture, delete the video system
    delete playing;
    if (g_handleClear != 2 || g_viewportClear != 1 || g_close != 1 || g_videoDtor != 1 || g_pVideoSys != 0 ||
        CMovie::PlayingMovie != 0 || g_freeTex != 1 || g_freedTexture != &tex || CMovie::MovieTexture != 0) return fail(__LINE__);
    // the current movie that is not playing and has no texture: no Close, no texture free
    CMovie* paused = new CMovie; paused->PParentViewport = &viewport; paused->Playing = false;
    CMovie::PlayingMovie = paused; g_pVideoSys = new CVideoSys;
    delete paused;
    if (g_close != 1 || g_videoDtor != 2 || g_freeTex != 1 || g_viewportClear != 2 || CMovie::PlayingMovie != 0 ||
        g_baseDtor != 3) return fail(__LINE__);
    printf("005483c0_TEST PASS\n");
    return 0;
}