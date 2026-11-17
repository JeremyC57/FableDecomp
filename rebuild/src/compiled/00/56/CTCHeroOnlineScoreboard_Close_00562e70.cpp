// CTCHeroOnlineScoreboard::ResetScoreboard() -- drops the HTTP connection (PHttp.reset(): the old LHHttp2 destroyed and freed), clears the downloaded scoreboard data, then zeroes ScoreboardState and the scoreboard/connecting component pointers
// Leaf: ?Close@CTCHeroOnlineScoreboard@@QAEXXZ
// Naming wave 31 (family hos): ClearScoreboard 0x00562E10 is private in both PDBs (?ClearScoreboard@CTCHeroOnlineScoreboard@@AAEXXZ, the chunk 702 own
//   row; was the public QAE spelling); the bare Leaf "ResetScoreboard" is the emitted symbol ?ResetScoreboard@CTCHeroOnlineScoreboard@@QAEXXZ (descriptive name; PROBABLE Close, chunk 702: one source)
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
// NF70 (round 7, land1117fu q10739): Ego_r.pdb 0x005DA530 / FableWin.pdb 0x022E9040 ?Close@CTCHeroOnlineScoreboard@@QAEXXZ (Ego_r donor order
//   ClearScoreboard 0x005DA4E0 -> Close 0x005DA530 = retail 0x00562E10 -> 0x00562E70); was the descriptive ResetScoreboard over a local
//   CTCHeroOnlineScoreboard / CHttpScopedPtr model. Now on engine/CTCHeroOnlineScoreboard.h (the 0x00563480 define block; FABLE_RETAIL_
//   CTCHEROONLINESCOREBOARD_CLOSE, and the overlay opt-in _CLEARSCOREBOARD for the private ClearScoreboard 0x00562E10).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_NETWORK
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_CLOSE
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_CLEARSCOREBOARD
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SCOREBOARD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_LHHTTP2
#include "fable_compiler.h"
#include "engine/CTCHeroOnlineScoreboard.h"

void CTCHeroOnlineScoreboard::Close()
{
    PHttp.reset();
    ClearScoreboard();
    ScoreboardState = 0;
    PConnectingMessage = 0;
    PConnectingMessage1 = 0;
    PScoreboardScreen = 0;
}
