#ifndef FABLE_ENGINE_NHEROINFORMATIONSCREENS_CMANAGER_H
#define FABLE_ENGINE_NHEROINFORMATIONSCREENS_CMANAGER_H
// NHeroInformationScreens::CManager (chunk 728 round 2, row 0x006D0830 CTCWife::GlobalUpdate): the hero-information screen manager
// CPlayerGui::GetPHeroInfoScreensManager 0x00437CD0 returns. Ego_r.pdb layout (sizeof 0x60 there: World +0, three Dinkumware
// vectors Screens / InventoryScreens / ScrollBarIndices, CurrentScreen, LastScreen, HighlightedScreen, bool OpenFlag, the
// CFadeInFadeOutBase ScreenEffectFade) at retail STLport sizes: the vectors are 12 bytes, so OpenFlag is +0x34 (GlobalUpdate reads
// [manager+34h]). FableWin.pdb / Ego_r.pdb ?IsOpen@CManager@NHeroInformationScreens@@QBE_NXZ is inline in retail.
#include "rebuild_abi.h"
class CWorld;
namespace NHeroInformationScreens {
class CManager {
public:
    bool IsOpen() const { return OpenFlag; }
private:
    CWorld* World;                                                       // +0x00
    unsigned char Screens[0xc];                                          // +0x04 vector<const NHeroInformationScreens::CBase*>
    unsigned char InventoryScreens[0xc];                                 // +0x10 vector<const NHeroInformationScreens::CBase*>
    unsigned char ScrollBarIndices[0xc];                                 // +0x1c vector<long>
    long CurrentScreen;                                                  // +0x28
    long LastScreen;                                                     // +0x2c
    long HighlightedScreen;                                              // +0x30
    bool OpenFlag;                                                       // +0x34
};
}
#endif
