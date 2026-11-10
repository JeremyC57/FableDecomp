// CTCTavernGameCardSorting::CardsMainGame -- time and collect the cards after each round.
// Leaf: ?CardsMainGame@CTCTavernGameCardSorting@@UAEXXZ
// Retail: 007d5070,539 bytes,thiscall.
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_game.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/card_sorting_types.h"
void CTCTavernGameCardSorting::CardsMainGame() {
 if(!RoundFinished) {
  switch(GameType) {
   case ECGT_ROUND_LIMIT: Counter+=1.0f/(float)CMainGameComponent::GetConstantFPS();break;
  }
 }
 if(Counter<0.0f)Counter=0.0f;
 for(std::vector<CCard*>::iterator card_it=Cards.begin();card_it!=Cards.end();++card_it) {
  if(!(*card_it)->FaceDown) {
   bool end_game=false;
   switch(GameType) {
    case ECGT_ROUND_LIMIT: if(Counter>TargetTime) { GameWon=false; end_game=true; }break;
   }
   if(end_game) {
    RoundFinished=true;GameFinished=true;
    if(GameWon)PlaySoundW("TAVERN_GAME_WIN_GAME");
    else PlaySoundW("TAVERN_GAME_LOSE_GAME");
    CDefPointer<const CTavernGameDef> pdef(NULL_DEF);GetDef(&pdef);
    SetStateAfterOK(GetText(GameWon?pdef->WinPhrase:pdef->LosePhrase),GetText(pdef->Continue),ETGS_TURN_CARDS_BACK);
   }
   return;
  }
 }
 RoundFinished=true;
 for(std::vector<CCard*>::iterator card_it=Cards.begin();card_it!=Cards.end();++card_it)if((*card_it)->Turning==true)return;
 int i=0;
 CDefPointer<const CTavernGameCardBaseDef> pdef(NULL_DEF);GetCardDef(&pdef);
 for(std::vector<CCard*>::iterator card_it=Cards.begin();card_it!=Cards.end();++card_it,++i) {
  PlaySoundW("TAVERN_GAME_CARD_DEAL");
  (*card_it)->Move(C3DVector(0.0f,0.0f,((float)(Cards.size()-i)*pdef->CardSeparation)+pdef->TableSeparation),false);
 }
 SetState(ETGS_CARDS_MOVING_BACK);
}