#ifndef FABLE_NPLAYERGUI_CDRAWGUILDSEAL_H
#define FABLE_NPLAYERGUI_CDRAWGUILDSEAL_H
#ifdef FABLE_RETAIL_NPLAYERGUI_CDRAWGUILDSEAL_PASS23
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/CFadeInFadeOutBase.h"
#include "engine/C2DVector.h"
#include "fable_string.h"
#include "stlport/fable_deque.h"
namespace NPlayerGui {
// Both original TPI class records; native constructor00651904 confirms field offsets.
class CDrawGuildSeal:public CDrawBase {
public:
 CDrawGuildSeal(CPlayerGui&,std::vector<CDrawBase*>&);
 ~CDrawGuildSeal();
 void Draw(const CEnginePrimitivePrepareInfo&);
 void SetEnabled(bool);
 bool IsEnabled()const;
#ifdef FABLE_RETAIL_DISPLAYNEXTMESSAGE_PASS30
 void ClearMessages();
 void DisplayNextMessage();
 void SetLevelUpGraphic(long graphic) { LevelUpGraphic = graphic; }
#endif
private:
 bool Enabled,ReadyToLevelUp;
 unsigned char FlashAlpha;
 long FlashCycles;
 CFadeInFadeOutBase FadeInOut;
 float ReminderTime;
 std::vector<C2DVector> LevelUpPositions;
 long LevelUpGraphic,CurrentMessageEndFrame,NextMessageDelayEndFrame;
 std::queue<std::pair<CCharString,long> > Messages;
};
}
FABLE_STATIC_ASSERT_AT(29, sizeof(NPlayerGui::CDrawGuildSeal)==144);
#endif
#endif
