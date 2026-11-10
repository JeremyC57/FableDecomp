// Body-level fixture: selected CardsMainGame body verbatim; container and external game services are test boundaries.
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int cases_run=0,strings_live=0;
static void check(bool b) { if(!b) { fprintf(stderr,"case %d failed\n",cases_run);abort(); } }
class CCharString {
public: const char* text;
 CCharString(const char* t,long=-1):text(t){++strings_live;}
 CCharString(const CCharString& x):text(x.text){++strings_live;}
 ~CCharString(){--strings_live;}
};
class CWideString {public: unsigned long value; explicit CWideString(unsigned long x):value(x){} };
enum ENullDef { NULL_DEF };
class CTavernGameDef { public: mutable int refs; unsigned long WinPhrase,LosePhrase,Continue; };
class CTavernGameCardBaseDef { public: mutable int refs; float CardSeparation,TableSeparation; };
template<class T> class CDefPointer {
public: T* Object;
 explicit CDefPointer(ENullDef):Object(0){}
 ~CDefPointer(){if(Object)--Object->refs;}
 const T* operator->()const{return Object;}
};
class C3DVector { public:float X,Y,Z;C3DVector(float x,float y,float z):X(x),Y(y),Z(z){} };
class CMainGameComponent {public:static long fps;static long GetConstantFPS(){return fps;} };long CMainGameComponent::fps;
static CTavernGameDef game_def;
static CTavernGameCardBaseDef card_def;
static int game_lookups,card_lookups,deal_sounds,lose_sounds,win_sounds,moves,state_calls,ok_calls;
static bool mutate_scale;
static float move_z[8];
class CCard {
public: bool Turning,FaceDown;
 void Move(const C3DVector& to,bool snap) {
  check(!snap && to.X==0 && to.Y==0 && moves<8);move_z[moves++]=to.Z;
 }
};
class CTCTavernGameCardSorting {
public:
 enum ECardGameType {ECGT_TIME_LIMIT=0,ECGT_ROUND_LIMIT=1};
 enum ETavernGameStates {ETGS_TURN_CARDS_BACK=24,ETGS_CARDS_MOVING_BACK=26};
 bool RoundFinished,GameFinished,GameWon;
 ECardGameType GameType;
 float Counter,TargetTime;
 std::vector<CCard*> Cards;
 void CardsMainGame();
 void GetDef(CDefPointer<const CTavernGameDef>* out) {++game_lookups;++game_def.refs;out->Object=&game_def;}
 void GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* out) {++card_lookups;++card_def.refs;out->Object=&card_def;}
 CWideString GetText(unsigned long value) {return CWideString(value);}
 void SetStateAfterOK(CWideString title,CWideString text,ETavernGameStates state) {
  ++ok_calls;check(state==ETGS_TURN_CARDS_BACK && title.value==game_def.LosePhrase && text.value==game_def.Continue);
 }
 unsigned long PlaySoundW(const CCharString& sound) {
  if(strcmp(sound.text,"TAVERN_GAME_CARD_DEAL")==0) {++deal_sounds;if(mutate_scale)card_def.CardSeparation=float(deal_sounds+1);}
  else if(strcmp(sound.text,"TAVERN_GAME_LOSE_GAME")==0)++lose_sounds;
  else if(strcmp(sound.text,"TAVERN_GAME_WIN_GAME")==0)++win_sounds;
  else check(false);
  return 0;
 }
 void SetState(ETavernGameStates state) {++state_calls;check(state==ETGS_CARDS_MOVING_BACK);}
};
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

static void run(int n,int face_mask,int turn_mask,int type,float counter,long fps,bool stopped,bool mutation) {
 ++cases_run;game_lookups=card_lookups=deal_sounds=lose_sounds=win_sounds=moves=state_calls=ok_calls=0;mutate_scale=mutation;
 game_def.refs=card_def.refs=0;game_def.WinPhrase=101;game_def.LosePhrase=202;game_def.Continue=303;
 card_def.CardSeparation=2;card_def.TableSeparation=0.5f;CMainGameComponent::fps=fps;
 CCard storage[4];CTCTavernGameCardSorting game;
 game.RoundFinished=stopped;game.GameFinished=false;game.GameWon=true;game.GameType=(CTCTavernGameCardSorting::ECardGameType)type;game.Counter=counter;game.TargetTime=5;
 for(int i=0;i<n;++i){storage[i].FaceDown=(face_mask&(1<<i))!=0;storage[i].Turning=(turn_mask&(1<<i))!=0;game.Cards.push_back(&storage[i]);}
 const bool face_up=face_mask!=((1<<n)-1),turning=turn_mask!=0;
 float expected_counter=counter;if(!stopped && type==1)expected_counter+=1.0f/float(fps);if(expected_counter<0)expected_counter=0;
 const bool lose=face_up && type==1 && expected_counter>5;
 const bool dealt=!face_up && !turning;
 game.CardsMainGame();
 check(game.Counter==expected_counter);
 check(game.RoundFinished==(lose || !face_up || stopped));check(game.GameFinished==lose);check(game.GameWon==!lose);
 check(game_lookups==(lose?1:0) && card_lookups==(dealt?1:0));check(game_def.refs==0 && card_def.refs==0);
 check(ok_calls==(lose?1:0) && lose_sounds==(lose?1:0) && win_sounds==0);
 check(state_calls==(dealt?1:0) && deal_sounds==(dealt?n:0) && moves==(dealt?n:0));
 for(int i=0;i<moves;++i) {float scale=mutation?float(i+2):2;check(move_z[i]==float(n-i)*scale+0.5f);}
 check(strings_live==0);
}
int main() {
 const float counters[6]={-1,0,1,5,10,20};const long rates[3]={1,2,4};
 for(int n=0;n<=3;++n)for(int face=0;face<(1<<n);++face)for(int turn=0;turn<(1<<n);++turn)
 for(int type=0;type<2;++type)for(int c=0;c<6;++c)for(int rate=0;rate<3;++rate)for(int stopped=0;stopped<2;++stopped)
 run(n,face,turn,type,counters[c],rates[rate],stopped!=0,false);
 check(cases_run==6120);run(3,7,0,1,0,2,false,true);check(cases_run==6121);
 puts("007d5070_TEST PASS");return 0;
}