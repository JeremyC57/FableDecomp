#ifndef FABLE_ENGINE_CUSERPROFILEMANAGER_H
#define FABLE_ENGINE_CUSERPROFILEMANAGER_H
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_SCOREBOARD
#include "fable_string.h"
#endif
// CUserProfileManager (new, chunk 765): only the two methods row 0x006C9FD0 calls are modelled. Ego_r.pdb and FableWin.pdb
// both spell the singleton accessor ?Get@CUserProfileManager@@SIAAV1@XZ (static __fastcall, returns a reference; retail
// 0x0040D2A0 lazily news 0x140 bytes into 0x013B7D4C); ?ProcessPendingTutorials@CUserProfileManager@@QAEXXZ = retail 0x0040BB30.
// Landed rows (e.g. NUISystemCSlider_GetValueFromProfile_0054bbb0.cpp) declare the same Get locally.
class CUserProfileManager {
public:
    static CUserProfileManager& __fastcall Get();                         // 0x0040D2A0
    void ProcessPendingTutorials();                                      // 0x0040BB30
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_SCOREBOARD
    // Nested opt-in (chunk 777, row 0x0055F710 CTCHeroOnlineScoreboard::ResetAndOpenHTTPAndPrepareRequest): the scoreboard settings and
    // their FableWin.pdb accessors (?GetScoreboardUserName@CUserProfileManager@@QAE?AVCWideString@@XZ, ..ProxyIp.., ?GetScoreboardProxyPort
    // @CUserProfileManager@@QAEJXZ, ?GetScoreboardServerPort@..@QAEJXZ; inline in retail, which copies the CWideString member with the
    // copy constructor 0x0099B720 and reads the ports in place). Retail offsets = Ego_r.pdb - 4 (STLport vectors 12, the deque-backed
    // queue 0x28): ScoreBoardUserName +0x110 (0x0055F8A8), ScoreBoardProxyIp +0x118 (0x0055F759), ScoreBoardProxyPort +0x11C
    // (0x0055F7B9), ScoreBoardServerPort +0x120 (0x0055F830). The members before them stay opaque.
    CWideString GetScoreboardUserName() { return ScoreBoardUserName; }
    CWideString GetScoreboardPassword() { return ScoreBoardPassword; }
    CWideString GetScoreboardProxyIp() { return ScoreBoardProxyIp; }
    long GetScoreboardProxyPort() { return ScoreBoardProxyPort; }
    long GetScoreboardServerPort() { return ScoreBoardServerPort; }
    unsigned char _pad_0[0x110];                                         // ProfileName .. StereoReversedNewProfile
    CWideString ScoreBoardUserName;                                      // +0x110
    CWideString ScoreBoardPassword;                                      // +0x114
    CWideString ScoreBoardProxyIp;                                       // +0x118
    long ScoreBoardProxyPort;                                            // +0x11c
    long ScoreBoardServerPort;                                           // +0x120
#endif
};
#endif
