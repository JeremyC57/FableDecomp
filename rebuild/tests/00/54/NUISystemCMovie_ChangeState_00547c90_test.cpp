#include <cstdio>
// Behaviour test for NUISystem::CMovie::ChangeState (0x00547C90): only a change INTO state 3 opens the movie; a failed
// Init leaves it stopped but still the playing movie; success sizes the component and recreates the texture.
static int g_closes = 0, g_releases = 0, g_creates = 0, g_ownerNotes = 0, g_handleClears = 0;
static long g_initResult = 0;
struct CVideoSys { long w, h; long m_24; void Close() { ++g_closes; } long Init() { return g_initResult; } };
static CVideoSys* g_pVideoSys = 0;
static void* g_pMovieTexture = 0;
static int g_texture;
struct CMovie {
    unsigned long TargetState;
    bool Playing, AttempToPlay, IsPlaying;
    float Width, Height;
    bool hasOwner;
    static CMovie* PlayingMovie;
    void BaseChangeState(unsigned long s) { TargetState = s; }
    void ChangeState(unsigned long newState)
    {
        unsigned long oldState = TargetState;
        BaseChangeState(newState);
        if (newState == 3 && oldState != newState) {
            if (g_pVideoSys == 0) {
                g_pVideoSys = new CVideoSys;
                g_pVideoSys->w = 640; g_pVideoSys->h = 480;
                g_pVideoSys->m_24 = 0;
            }
            if (PlayingMovie && PlayingMovie->IsPlaying && g_pVideoSys)
                g_pVideoSys->Close();
            Playing = false;
            if (g_pVideoSys->Init() >= 0) {
                Width = (float)g_pVideoSys->w;
                Height = (float)g_pVideoSys->h;
                ++g_handleClears;
                if (hasOwner)
                    ++g_ownerNotes;
                if (g_pMovieTexture) {
                    ++g_releases;
                    g_pMovieTexture = 0;
                }
                ++g_creates;
                g_pMovieTexture = &g_texture;
                AttempToPlay = true;
                Playing = true;
            }
            PlayingMovie = this;
        }
    }
};
CMovie* CMovie::PlayingMovie = 0;
int main()
{
    CMovie a = { 0, false, false, true, 0, 0, true };
    CMovie b = { 3, false, false, true, 0, 0, false };
    int bad = 0;
    b.ChangeState(3);                       // already targeting 3: nothing happens
    bad |= g_pVideoSys != 0 || CMovie::PlayingMovie != 0;
    a.ChangeState(3);
    bad |= !a.Playing || !a.AttempToPlay || a.Width != 640.0f || a.Height != 480.0f || CMovie::PlayingMovie != &a;
    bad |= g_creates != 1 || g_releases != 0 || g_closes != 0 || g_ownerNotes != 1;
    b.TargetState = 1;
    g_initResult = -1;
    b.ChangeState(3);                       // a is playing -> closed; Init fails
    bad |= g_closes != 1 || b.Playing || CMovie::PlayingMovie != &b || g_creates != 1;
    g_initResult = 0;
    b.TargetState = 2;
    b.ChangeState(3);
    bad |= g_closes != 2 || g_releases != 1 || g_creates != 2 || !b.Playing;
    if (bad) { std::printf("00547c90_TEST FAIL\n"); return 1; }
    std::printf("00547c90_TEST PASS\n");
    return 0;
}