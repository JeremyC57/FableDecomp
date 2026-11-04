#ifndef FABLE_ENGINE_CUSERPROFILEMANAGER_H
#define FABLE_ENGINE_CUSERPROFILEMANAGER_H
// CUserProfileManager (new, chunk 765): only the two methods row 0x006C9FD0 calls are modelled. Ego_r.pdb and FableWin.pdb
// both spell the singleton accessor ?Get@CUserProfileManager@@SIAAV1@XZ (static __fastcall, returns a reference; retail
// 0x0040D2A0 lazily news 0x140 bytes into 0x013B7D4C); ?ProcessPendingTutorials@CUserProfileManager@@QAEXXZ = retail 0x0040BB30.
// Landed rows (e.g. NUISystemCSlider_GetValueFromProfile_0054bbb0.cpp) declare the same Get locally.
class CUserProfileManager {
public:
    static CUserProfileManager& __fastcall Get();                         // 0x0040D2A0
    void ProcessPendingTutorials();                                      // 0x0040BB30
};
#endif
