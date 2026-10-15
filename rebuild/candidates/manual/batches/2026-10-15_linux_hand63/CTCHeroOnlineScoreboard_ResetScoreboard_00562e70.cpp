// CTCHeroOnlineScoreboard::ResetScoreboard() -- drops the HTTP connection (PHttp.reset(): the old LHHttp2 destroyed and freed), clears the downloaded scoreboard data, then zeroes ScoreboardState and the scoreboard/connecting component pointers
// Leaf: ResetScoreboard
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00562E70, length 56 bytes (`ret`).
// Callees: LHNetworkLib::LHHttp2::~LHHttp2() @ 0x008410D0 (manifest label
//          "CNetworkConnection_Cleanup"), operator delete @ 0x00BFE9BC,
//          CTCHeroOnlineScoreboard::ClearScoreboard() @ 0x00562E10.
// Identity: the manifest label "CParticleEffect::~CParticleEffect" is propagated; this is a plain
// member (no vtable store, `ret`), called on the scoreboard component fetched from a thing's
// component map (0x00584B39, 0x00587900), right after ClearScoreboard 0x00562E10 in the
// CTCHeroOnlineScoreboard block. The name is descriptive.
// Layout: PDB CTCHeroOnlineScoreboard with the retail 12-byte maps/vectors and 4-byte lists (the
// fields used by ClearScoreboard and 0x00563410 pin it): PHttp (scoped_ptr<LHHttp2>) +0x0c,
// ScoreboardState +0x50, TaskState +0x54, PScoreboardScreen +0x58, PClanMenuEntry +0x5c,
// PConnectingMessage +0x60, PConnectingMessage1 +0x64 (PDB +0x70.. -- 0x20 lower).
// Shape: the inline scoped_ptr reset (`old = px; px = 0; delete old;` with the zero in ebx), the
// non-virtual LHHttp2 destructor out of line plus operator delete, the ClearScoreboard call, then the
// four zero stores in source order through the callee-saved zero register.

struct CCharString {
    void* m_pData;
};

namespace LHNetworkLib {

struct LHHttp2 {
    ~LHHttp2();                                                           // 0x008410D0
};

} // namespace LHNetworkLib

struct CHttpScopedPtr {                                                   // scoped_ptr<LHNetworkLib::LHHttp2>
    LHNetworkLib::LHHttp2* px;
    void reset(LHNetworkLib::LHHttp2* p = 0)
    {
        LHNetworkLib::LHHttp2* old = px;
        px = p;
        delete old;
    }
};

struct CComponent;

struct CTCHeroOnlineScoreboard {
    virtual void v00();
    void*          m_pThing;                                              // +0x04
    long           m_8;                                                   // +0x08
    CHttpScopedPtr PHttp;                                                 // +0x0c
    unsigned char  _scoreboardData[0x40];                                 // +0x10 PictureCache .. DownloadedClanScoreboard
    long           ScoreboardState;                                       // +0x50
    long           TaskState;                                             // +0x54
    CComponent*    PScoreboardScreen;                                     // +0x58
    CComponent*    PClanMenuEntry;                                        // +0x5c
    CComponent*    PConnectingMessage;                                    // +0x60
    CComponent*    PConnectingMessage1;                                   // +0x64
    void ClearScoreboard();                                               // 0x00562E10
    void ResetScoreboard();
};

void CTCHeroOnlineScoreboard::ResetScoreboard()
{
    PHttp.reset();
    ClearScoreboard();
    ScoreboardState = 0;
    PConnectingMessage = 0;
    PConnectingMessage1 = 0;
    PScoreboardScreen = 0;
}
