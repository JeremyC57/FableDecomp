#include <cstdio>
#include <vector>
#include <cstdlib>
class CCharString{public:const char* Text;CCharString(const char* s,long=-1):Text(s){}};
class C2DVector{public:float X,Y;C2DVector(float x,float y):X(x),Y(y){}};
static bool gBetween,gChild;static unsigned int gSetCalls,gInitialises,gPositions;
namespace NUISystem {
class CComponent{public:virtual ~CComponent(){};virtual CComponent* FindChild(CCharString);void Initialise(){++gInitialises;}void ChangePosition(const C2DVector& p,float d,bool b){if(d!=0||b)std::exit(1);LastX=p.X;LastY=p.Y;++gPositions;}float LastX,LastY;};
class CText:public CComponent{public:void SetValue(const CCharString&,bool a,bool b){if(!a||!b)std::exit(1);++gSetCalls;}};
static CText text;
CComponent* CComponent::FindChild(CCharString){return gChild?&text:0;}
class CManager{public:static CManager* GetInstance(){static CManager m;return &m;}CComponent* CreateComponent(const CCharString&,bool b){if(b)std::exit(1);return &Component;}C2DVector GetUIScale()const{return C2DVector(2.0f,0.5f);}CComponent Component;};
class CUIScreenUnitConverter{public:static float GetBackBufferWidth(){return 1024.0f;}static float GetBackBufferHeight(){return 768.0f;}};
}
class CQuestManager{public:static CQuestManager& Get(){static CQuestManager q;return q;}bool IsBetweenQuestStartAndFinishScreens()const{return gBetween;}};
static double GFGetTime(){return 17.25;}
class CTCInGameMenu{public:long SaveGameIndex,FramesToWaitToSave;std::vector<NUISystem::CComponent*> Components;NUISystem::CComponent* PSaveInProgress;double SaveInProgressTime;void SetToSave(long);};
using namespace NUISystem;
void CTCInGameMenu::SetToSave(long save_index)
{
 SaveGameIndex=save_index;
 FramesToWaitToSave=2;
 Components.push_back(CManager::GetInstance()->CreateComponent("PC_SAVE_IN_PROGRESS_DIALOG",false));
 if(CQuestManager::Get().IsBetweenQuestStartAndFinishScreens()) {
  CComponent* ptext=Components[Components.size()-1]->FindChild(CCharString("PC_SAVE_IN_PROGRESS_TEXT",-1));
  if(ptext) {
   CText* ptext_real=static_cast<CText*>(ptext);
   ptext_real->SetValue("TEXT_GUI_SAVE_HERO_IN_PROGRESS_PC",true,true);
  }
 }
 Components[Components.size()-1]->Initialise();
 PSaveInProgress=Components[Components.size()-1];
 C2DVector pos(CUIScreenUnitConverter::GetBackBufferWidth()*0.5f-CManager::GetInstance()->GetUIScale().X*204.0f,
               CUIScreenUnitConverter::GetBackBufferHeight()*0.5f-CManager::GetInstance()->GetUIScale().Y*52.0f);
 PSaveInProgress->ChangePosition(pos,0.0f,false);
 SaveInProgressTime=GFGetTime();
}

static void require(bool ok){if(!ok)std::exit(1);}
int main(){for(unsigned int b=0;b<2;++b)for(unsigned int c=0;c<2;++c){
 gBetween=b!=0;gChild=c!=0;gSetCalls=gInitialises=gPositions=0;
 CTCInGameMenu m;m.SetToSave(9);
 require(m.SaveGameIndex==9&&m.FramesToWaitToSave==2&&m.Components.size()==1);
 require(gSetCalls==(b&&c?1u:0u)&&gInitialises==1&&gPositions==1);
 require(m.PSaveInProgress==m.Components.back()&&m.SaveInProgressTime==17.25);
 require(m.PSaveInProgress->LastX==104.0f&&m.PSaveInProgress->LastY==358.0f);
 }std::printf("0047ed76_TEST PASS\n");return 0;}
