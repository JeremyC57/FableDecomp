// Self-contained C++03: selected destructor body and original member order.
// String/UI services are controlled; standard containers exercise all implicit member destruction.
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <stack>
#include <deque>
#ifndef _MSC_VER
#define __fastcall
#endif
static int events[256],event_count,deleted[32],keyboard_calls,clear_calls;static bool recording;
static void require(bool x){if(!x){puts("005954ad_TEST FAIL");exit(1);}}
static void event(int n){if(recording){require(event_count<256);events[event_count++]=n;}}
class CWideString {public:int id;CWideString():id(0){}explicit CWideString(int n):id(n){}~CWideString(){if(id)event(id);}bool operator<(const CWideString& x)const{return id<x.id;}};
class CCharString {public:int id;CCharString():id(0){}explicit CCharString(int n):id(n){}~CCharString(){if(id)event(id);}};
class CDateAndTime {protected:long Year;unsigned long Month,Day,Hour,Minute,Second,Millisecond;};
class CNewFrontendGameComponent{};class CVirtualKeyboard{};
template<class T>class CCountedPointer;
enum FRONTEND_SCREEN{FRONTEND_NULL=0};
namespace NUISystem {
class CAction{};class CActionParam{};
enum EEvent {UI_TEST_EVENT=0};
class CObserver {public:virtual ~CObserver(){}virtual void ProcessEvent(EEvent)=0;protected:std::set<EEvent> EventsToObserve;bool PreventObservation;};
class CComponent {public:int id;explicit CComponent(int n):id(n){}virtual ~CComponent(){require(id>=0&&id<32);++deleted[id];event(100+id);}};
class CManager {public:static void __fastcall Clear();};
class CFrontEndManager:public CObserver {
public:
 CFrontEndManager():PGameComponent(0),m_pVirtualKeyboard(0){}
 virtual ~CFrontEndManager();
 virtual void Action(CCountedPointer<CAction>&){}
 virtual void ProcessEvent(EEvent){}
 void CloseVirtualKeyboard(bool);
 // Public fixture access changes no destructor operation.
 CWideString MediaPlayerErrorString;
 std::list<CActionParam*> UIActionParams;
 CNewFrontendGameComponent* PGameComponent;
 static CFrontEndManager* m_pInstance;
 std::stack<CComponent*> m_stackScreens;
 std::vector<CComponent*> m_vComponents;
 std::map<FRONTEND_SCREEN,CComponent*> m_mapScreens;
 CVirtualKeyboard* m_pVirtualKeyboard;
 bool m_bStartNewGame;
 std::vector<CDateAndTime> m_vFileTimes;
 std::vector<CCharString> FileRegionNames,FileRegionMiniMapNames;
 std::vector<long> FilePlayTimes;
 CComponent* PScreenToDeactivate;CComponent* PScreenToActivate;
 bool GoToKeyboard;
 CComponent* PScreenToDeactivateForStartGame;CComponent* PScreenToDeactivateForLoadGame;
 CWideString GameToLoad,ProfileToDelete;
 unsigned long BlocksOnUNeeded,BlocksOnTNeeded,NumberSaveGames;
 bool m_bQuitFromGame;
 std::map<CWideString,bool> Signatures;
 double InstallationPauseStartTime;
 bool InstallationPaused,FinalInstallationNeverPause;
};
CFrontEndManager* CFrontEndManager::m_pInstance=0;
void CFrontEndManager::CloseVirtualKeyboard(bool reactivate){require(!reactivate&&m_pInstance==this);++keyboard_calls;event(1);delete m_pVirtualKeyboard;m_pVirtualKeyboard=0;}
void __fastcall CManager::Clear(){require(CFrontEndManager::m_pInstance!=0);++clear_calls;event(2);}
}
NUISystem::CFrontEndManager::~CFrontEndManager()
{
 CloseVirtualKeyboard(false);
 std::map<FRONTEND_SCREEN,CComponent*>::const_iterator iter;
 for(iter=m_mapScreens.begin();iter!=m_mapScreens.end();++iter)
  if(iter->second)delete iter->second;
 CManager::Clear();
 m_pInstance=0;
}

int main(){using namespace NUISystem;int cases=0;
 for(int count=0;count!=7;++count)for(int holes=0;holes!=4;++holes)for(int members=0;members!=3;++members)for(int keyboard=0;keyboard!=2;++keyboard){
  recording=false;CFrontEndManager* manager=new CFrontEndManager;manager->m_pInstance=manager;
  for(int k=count-1;k>=0;--k){CComponent* value=((holes&(1<<(k%2)))!=0)?0:new CComponent(k);manager->m_mapScreens[(FRONTEND_SCREEN)k]=value;manager->m_vComponents.push_back(value);manager->m_stackScreens.push(value);}
  if(keyboard)manager->m_pVirtualKeyboard=new CVirtualKeyboard;
  manager->MediaPlayerErrorString.id=10;manager->GameToLoad.id=11;manager->ProfileToDelete.id=12;
  manager->m_vFileTimes.resize(members);manager->FilePlayTimes.resize(members,27);
  for(int k=0;k<members;++k){manager->FileRegionNames.push_back(CCharString(20+k));manager->FileRegionMiniMapNames.push_back(CCharString(30+k));manager->Signatures[CWideString(40+k)]=(k%2)!=0;}
  CActionParam actions[2];manager->UIActionParams.push_back(&actions[0]);manager->UIActionParams.push_back(&actions[1]);
  for(int k=0;k<32;++k)deleted[k]=0;event_count=keyboard_calls=clear_calls=0;recording=true;delete manager;recording=false;
  require(CFrontEndManager::m_pInstance==0&&keyboard_calls==1&&clear_calls==1);int at=0;require(events[at++]==1);
  for(int k=0;k<count;++k){bool live=(holes&(1<<(k%2)))==0;require(deleted[k]==(live?1:0));if(live)require(events[at++]==100+k);}require(events[at++]==2);
  // Container traversal orders may differ across standard libraries; member order may not.
  for(int k=0;k<members;++k){require(events[at]>=40&&events[at]<40+members);++at;}
  require(events[at++]==12);require(events[at++]==11);
  for(int k=0;k<members;++k){require(events[at]>=30&&events[at]<30+members);++at;}
  for(int k=0;k<members;++k){require(events[at]>=20&&events[at]<20+members);++at;}
  require(events[at++]==10&&at==event_count);++cases;
 }
 require(cases==168);puts("005954ad_TEST PASS");return 0;
}