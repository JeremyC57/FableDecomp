// Isolated fixture: selected AddElement body, actual reference-sharing semantics with live external owners.
#include <cstdio>
#include <cassert>
#include <list>
class CCPPointerInfo {public:unsigned long RefCount;};
template<class T>class CCountedPointer {public:T* Data;CCPPointerInfo* Info;
 CCountedPointer(T* data,CCPPointerInfo* info):Data(data),Info(info){++Info->RefCount;}
 CCountedPointer(const CCountedPointer& other):Data(other.Data),Info(other.Info){if(Info)++Info->RefCount;}
 ~CCountedPointer(){if(Info){assert(Info->RefCount>1);--Info->RefCount;}Data=0;Info=0;}
 T* operator->()const{return Data;}T* Get()const{return Data;}
};
class CQuestManager {public:
 class CActiveQuest {public:long ID;long GetID()const{return ID;}};
 CActiveQuest* PQuestCurrentlyRunning;static CQuestManager* current;
 static CQuestManager& Get(){return *current;}CActiveQuest* GetPActiveQuest()const{return PQuestCurrentlyRunning;}
};
CQuestManager* CQuestManager::current=0;
namespace NPlayerGui {class CDrawQuestInfo {public:
 class CQuestInfoElement {public:long ID,ScriptID;float Scale;};
 std::list<CCountedPointer<CQuestInfoElement> > Elements;
 void AddElement(CCountedPointer<CQuestInfoElement> pelement);
};}
namespace NPlayerGui {
void CDrawQuestInfo::AddElement(CCountedPointer<CQuestInfoElement> pelement)
{
    CQuestManager::CActiveQuest* pactive_quest=CQuestManager::Get().GetPActiveQuest();
    if(pactive_quest)pelement->ScriptID=pactive_quest->GetID();
    long highest_id=0;
    for(std::list<CCountedPointer<CQuestInfoElement> >::iterator it=Elements.begin();it!=Elements.end();++it){
        CQuestInfoElement* ploop_element=it->Get();
        if(ploop_element->ID>highest_id)highest_id=ploop_element->ID;
    }
    pelement->ID=highest_id+1;
    pelement->Scale*=0.7f;
    Elements.push_back(pelement);
}
}

int main(){
 typedef NPlayerGui::CDrawQuestInfo Draw;
 CQuestManager manager;CQuestManager::current=&manager;CQuestManager::CActiveQuest quest;quest.ID=73;
 const long ids[]={-4,0,3,11};const float scales[]={0.0f,10.0f,20.0f,40.0f,-10.0f};const float scaled[]={0.0f,7.0f,14.0f,28.0f,-7.0f};
 for(int active=0;active<2;++active)for(int count=0;count<4;++count)for(int pattern=0;pattern<4;++pattern)for(int scale=0;scale<5;++scale){
  Draw::CQuestInfoElement objects[4];CCPPointerInfo infos[4];for(int i=0;i<4;++i)infos[i].RefCount=1;
  manager.PQuestCurrentlyRunning=active?&quest:0;
  {Draw display;long highest=0;
   for(int i=0;i<count;++i){objects[i].ID=ids[(i+pattern)%4];objects[i].ScriptID=17;objects[i].Scale=1.0f;CCountedPointer<Draw::CQuestInfoElement> p(&objects[i],&infos[i]);display.Elements.push_back(p);if(objects[i].ID>highest)highest=objects[i].ID;}
   objects[3].ID=999;objects[3].ScriptID=-28;objects[3].Scale=scales[scale];
   {CCountedPointer<Draw::CQuestInfoElement> p(&objects[3],&infos[3]);unsigned long before=infos[3].RefCount;display.AddElement(p);
    assert(infos[3].RefCount==before+1);assert(p.Get()==&objects[3]);assert(display.Elements.size()==(unsigned int)(count+1));assert(display.Elements.back().Get()==&objects[3]);
    assert(objects[3].ID==highest+1&&objects[3].ScriptID==(active?73:-28)&&objects[3].Scale==scaled[scale]);
    for(int i=0;i<count;++i)assert(objects[i].ID==ids[(i+pattern)%4]&&objects[i].ScriptID==17&&objects[i].Scale==1.0f);
   }
   assert(infos[3].RefCount==2);
  }
  for(int i=0;i<4;++i)assert(infos[i].RefCount==1);
 }
 std::puts("0064a3ed_TEST PASS");return 0;
}