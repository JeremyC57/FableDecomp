#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <list>
#include <vector>
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_EActionType.h"
static int live_actions,create_calls,init_calls,state_calls,find_calls,enter_calls,escape_calls,capture_calls;
namespace NUISystem {
struct CUIInterface {virtual ~CUIInterface(){}};struct CObserver {virtual ~CObserver(){}int state[4];};struct CActionParam {virtual ~CActionParam(){}};
struct CAction {EActionType ActionType;std::list<CActionParam*>UIActionParams;CAction():ActionType(UI_ACTION_TYPE_NONE){++live_actions;}~CAction(){--live_actions;}};
struct CComponent: CUIInterface,CObserver,CActionParam {CComponent*child;virtual void Initialise(){++init_calls;assert(create_calls==init_calls);}virtual void ChangeState(unsigned long s){assert(s==1&&init_calls==state_calls+1);++state_calls;}virtual CComponent*FindChild(const char*name){assert(!strcmp(name,"PC_PHOTOJOURNAL_CAPTURE_EDIT_BOX")&&state_calls==find_calls+1);++find_calls;return child;}};
struct CEditBox:CComponent {CCountedPointer<CAction>enter_action,escape_action;void AddActionOnEnter(CCountedPointer<CAction>&a){assert(a->ActionType==UI_ACTION_TYPE_PHOTO_CAPTION&&a->UIActionParams.size()==1&&a->UIActionParams.front()==static_cast<CActionParam*>(this));++enter_calls;enter_action=a;}void AddActionOnEscape(CCountedPointer<CAction>&a){assert(a->ActionType==UI_ACTION_TYPE_DISCARD_PHOTO&&a->UIActionParams.empty());++escape_calls;escape_action=a;}};
struct CManager {static CManager*GetInstance(){static CManager m;return &m;}CComponent*CreateComponent(const char*name,bool dynamic);};
}
struct CTCInventory {void CaptureBackBufferToTexture();};
static CTCInventory inventory;
struct CThing {bool have_inventory;bool GetTC(CTCInventory**p){if(have_inventory)*p=&inventory;return have_inventory;}};
struct CTCPhotojournalCapture {CThing*ParentThing;bool ScreenOpen,FirstRenderFrame,DiscardScreenShot;long FrameCounter;std::vector<NUISystem::CComponent*>Components;CThing&GetParentThing(){return *ParentThing;}void CreateScreen();};
static CTCPhotojournalCapture*running;static NUISystem::CComponent*screen;static unsigned int initial_components;
NUISystem::CComponent*NUISystem::CManager::CreateComponent(const char*name,bool dynamic){assert(!strcmp(name,"PC_PHOTOJOURNAL_CAPTURE")&&!dynamic&&running->ScreenOpen&&running->FirstRenderFrame&&running->FrameCounter==0);++create_calls;return screen;}
void CTCInventory::CaptureBackBufferToTexture(){assert(running->Components.size()==initial_components+1&&running->Components.back()==screen);++capture_calls;}

void CTCPhotojournalCapture::CreateScreen()
{
 ScreenOpen=true;
 FirstRenderFrame=true;
 FrameCounter=0;
 NUISystem::CManager* pmanager=NUISystem::CManager::GetInstance();
 NUISystem::CComponent* pscreen=pmanager->CreateComponent("PC_PHOTOJOURNAL_CAPTURE",false);
 pscreen->Initialise();
 pscreen->ChangeState(1);
 NUISystem::CEditBox* pedit_box=static_cast<NUISystem::CEditBox*>(pscreen->FindChild("PC_PHOTOJOURNAL_CAPTURE_EDIT_BOX"));
 if(pedit_box) {
  CCountedPointer<NUISystem::CAction> paction_on_enter(new NUISystem::CAction);
  paction_on_enter->ActionType=NUISystem::UI_ACTION_TYPE_PHOTO_CAPTION;
  paction_on_enter->UIActionParams.push_back(pedit_box);
  pedit_box->AddActionOnEnter(paction_on_enter);
  CCountedPointer<NUISystem::CAction> paction_on_escape(new NUISystem::CAction);
  paction_on_escape->ActionType=NUISystem::UI_ACTION_TYPE_DISCARD_PHOTO;
  pedit_box->AddActionOnEscape(paction_on_escape);
 }
 Components.push_back(pscreen);
 CTCInventory* ptc_inventory;
 if(GetParentThing().GetTC(&ptc_inventory))ptc_inventory->CaptureBackBufferToTexture();
}
int main(){
 for(int edit=0;edit<2;++edit)for(int inv=0;inv<2;++inv)for(int prefill=0;prefill<2;++prefill){
  create_calls=init_calls=state_calls=find_calls=enter_calls=escape_calls=capture_calls=0;assert(live_actions==0);
  {
   NUISystem::CEditBox box;NUISystem::CComponent main_screen,old_screen;main_screen.child=edit?&box:0;screen=&main_screen;
   CThing thing;thing.have_inventory=inv!=0;
   CTCPhotojournalCapture component;component.ParentThing=&thing;component.ScreenOpen=component.FirstRenderFrame=false;component.FrameCounter=17;component.DiscardScreenShot=true;if(prefill)component.Components.push_back(&old_screen);running=&component;initial_components=component.Components.size();
   component.CreateScreen();
   assert(component.ScreenOpen&&component.FirstRenderFrame&&component.FrameCounter==0&&component.DiscardScreenShot);
   assert(component.Components.size()==initial_components+1&&component.Components.back()==&main_screen);if(prefill)assert(component.Components.front()==&old_screen);
   assert(create_calls==1&&init_calls==1&&state_calls==1&&find_calls==1&&enter_calls==edit&&escape_calls==edit&&capture_calls==inv&&live_actions==edit*2);
   if(edit)assert(box.enter_action.GetReferenceCount()==1&&box.escape_action.GetReferenceCount()==1);
  }
  assert(live_actions==0);
 }
 puts("0055dc00_TEST PASS");return 0;
}