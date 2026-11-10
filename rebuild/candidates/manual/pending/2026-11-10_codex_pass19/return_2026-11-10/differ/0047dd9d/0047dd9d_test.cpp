#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#define FABLE_FASTCALL
static void check(bool b){if(!b)std::abort();}
static int trace[64],used;
static void log(int x){trace[used++]=x;}
namespace NUISystem {
enum {UI_STATE_ACTIVE=1,UI_EVENT_MOUSE_MOVEMENT=25};
struct C2DVector{float X,Y;C2DVector(float x=0,float y=0):X(x),Y(y){}};
struct CObserver{int id;CObserver(int i):id(i){} void ObserveEvent(int e){check(e==25);log(id*10+3);}void ObserveAllEvents(){log(id*10+4);}};
struct CComponent: CObserver {
 CComponent* yes;CComponent* no;C2DVector pos;int state;void(*yescb)();void(*nocb)();
 CComponent(int i):CObserver(i),yes(0),no(0),state(0),yescb(0),nocb(0){}
 void SetYesCallBack(void(*f)()){yescb=f;log(101);}
 void SetNoCallBack(void(*f)()){nocb=f;log(102);}
 void ChangeState(int s){state=s;log(103);}
 void ChangePosition(const C2DVector& p,float t,bool b){check(t==0&&!b);pos=p;log(104);}
 CComponent* FindChild(const char* s){if(!std::strcmp(s,"PC_YESNO_GAME_YESBUTTON")){log(105);return yes;}check(!std::strcmp(s,"PC_YESNO_GAME_NOBUTTON"));log(106);return no;}
};
struct CYesNo:CComponent{CYesNo():CComponent(1){}};
struct CManager {
 static CManager* instance;C2DVector scale;
 static CManager* GetInstance(){return instance;}
 C2DVector GetUIScale()const{return scale;}
 void RemoveObserver(CObserver* p){log(p->id*10+1);}
 void AddConcurrentExclusiveObserver(CObserver* p){log(p->id*10+2);}
};CManager* CManager::instance=0;
struct CUIScreenUnitConverter{static float width,height;static float GetBackBufferWidth(){return width;}static float GetBackBufferHeight(){return height;}};
float CUIScreenUnitConverter::width;float CUIScreenUnitConverter::height;
}
using namespace NUISystem;
static void PCLoad(){}
static void PCCancelQuitToFrontEnd(){}
CComponent* g_pPCMousePointer=0;
CYesNo* g_pPCYesNoDialog=0;
struct CTCInGameMenu{long SaveGameToLoad;CYesNo* PYesNo;CComponent* PMousePointer;void PCSetToLoad(long);};
void CTCInGameMenu::PCSetToLoad(long index)
{
 SaveGameToLoad=index;
 PYesNo->SetYesCallBack(PCLoad);
 PYesNo->ChangeState(UI_STATE_ACTIVE);
 C2DVector pos(CUIScreenUnitConverter::GetBackBufferWidth()*0.5f-CManager::GetInstance()->GetUIScale().X*104.0f,
               CUIScreenUnitConverter::GetBackBufferHeight()*0.5f-CManager::GetInstance()->GetUIScale().Y*52.0f);
 PYesNo->ChangePosition(pos,0.0f,false);
 PYesNo->SetNoCallBack(PCCancelQuitToFrontEnd);
 g_pPCYesNoDialog=PYesNo;
 g_pPCMousePointer=PMousePointer;
 CManager::GetInstance()->RemoveObserver(g_pPCYesNoDialog);
 CManager::GetInstance()->AddConcurrentExclusiveObserver(g_pPCYesNoDialog);
 CManager::GetInstance()->RemoveObserver(g_pPCMousePointer);
 CManager::GetInstance()->AddConcurrentExclusiveObserver(g_pPCMousePointer);
 g_pPCMousePointer->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
 g_pPCYesNoDialog->ObserveAllEvents();
 CComponent* pclickable=0;
 pclickable=PYesNo->FindChild("PC_YESNO_GAME_YESBUTTON");
 if(pclickable) {
  CManager::GetInstance()->AddConcurrentExclusiveObserver(pclickable);
  pclickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
 }
 pclickable=PYesNo->FindChild("PC_YESNO_GAME_NOBUTTON");
 if(pclickable) {
  CManager::GetInstance()->AddConcurrentExclusiveObserver(pclickable);
  pclickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
 }
}

int main(){
 CManager manager;CManager::instance=&manager;
 CYesNo dialog;CComponent mouse(2),yes(3),no(4);CTCInGameMenu menu;menu.PYesNo=&dialog;menu.PMousePointer=&mouse;
 for(int c=0;c<16;++c){
  manager.scale=C2DVector(float(c%3+1)*0.5f,float(c%5+1)*0.25f);
  CUIScreenUnitConverter::width=640.0f+c*17;CUIScreenUnitConverter::height=480.0f+c*13;
  dialog.yes=(c&1)?&yes:0;dialog.no=(c&2)?&no:0;used=0;
  menu.PCSetToLoad(c*7-13);
  check(menu.SaveGameToLoad==c*7-13&&dialog.yescb==PCLoad&&dialog.nocb==PCCancelQuitToFrontEnd&&dialog.state==UI_STATE_ACTIVE);
  check(g_pPCYesNoDialog==&dialog&&g_pPCMousePointer==&mouse);
  check(std::fabs(dialog.pos.X-(CUIScreenUnitConverter::width*0.5f-manager.scale.X*104.0f))<0.00001);
  check(std::fabs(dialog.pos.Y-(CUIScreenUnitConverter::height*0.5f-manager.scale.Y*52.0f))<0.00001);
  int expected[20]={101,103,104,102,11,12,21,22,23,14,105};int count=11;
  if(c&1){expected[count++]=32;expected[count++]=33;}
  expected[count++]=106;if(c&2){expected[count++]=42;expected[count++]=43;}
  check(used==count);for(int j=0;j<count;++j)check(trace[j]==expected[j]);
 }
 std::puts("0047dd9d_TEST PASS");return 0;
}
