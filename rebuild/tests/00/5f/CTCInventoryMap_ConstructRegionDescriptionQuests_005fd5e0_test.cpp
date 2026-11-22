#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <list>
static void require(bool value) { if(!value) abort(); }
typedef std::string CCharString;
typedef std::string CWideString;

template <class T> struct CCountedPointer {
    T* ptr;
    CCountedPointer():ptr(0){}
    ~CCountedPointer(){if(ptr)--ptr->refs;}
    void Reset(T* next){if(next)++next->refs;if(ptr)--ptr->refs;ptr=next;}
    T* operator->() const { require(ptr!=0);return ptr; }
private:
    CCountedPointer(const CCountedPointer&);
    CCountedPointer& operator=(const CCountedPointer&);
};
namespace NUISystem {
class CComponent {
public:
    bool childAvailable;
    int refs,lookups;
    std::string title,lookup;
    CComponent* text;
    std::vector<CComponent*> children;
    CComponent():childAvailable(false),refs(0),lookups(0),text(0){}
    virtual ~CComponent(){}
    CComponent* FindChild(const CCharString& name) {++lookups;lookup=name;return childAvailable?text:0;}
    void AddChild(CCountedPointer<CComponent>& p) {require(p.ptr && p.ptr->refs==1);children.push_back(p.ptr);}
};
class CText:public CComponent {
public:
    int updates;
    CText():updates(0){}
    void SetValue(const CWideString& n,bool update,bool reformat) {require(update&&reformat);++updates;title=n;}
};
class CManager {
public:
    static CManager* active;
    CComponent widgets[6];
    CText labels[6];
    unsigned available,count;
    static CManager* GetInstance(){return active;}
    CManager():available(0),count(0){}
    CComponent* CreateComponent(const CCharString& id,bool hidden) {
        require(count<6&&!hidden);unsigned i=count++;widgets[i].title=id;
        widgets[i].text=&labels[i];widgets[i].childAvailable=(available&(1U<<i))!=0;
        return &widgets[i];
    }
};
CManager* CManager::active=0;
}
class CRegion {public:CCharString Name;const CCharString& GetName() const{return Name;}};
struct CQuestCardDef { bool IsVignette; };
class CTCQuestCard {
public:
    CQuestCardDef def;
    bool core,optional;
    CCharString region,title;
    CCharString GetRegionName()const{return region;}
    bool IsCore()const{return core;}
    bool IsOptional()const{return optional;}
    const CQuestCardDef* GetPDef()const{return &def;}
    CWideString GetQuestName()const{return title;}
};
class CThing { public:CTCQuestCard card;bool GetTC(CTCQuestCard** out){*out=&card;return true;} };
template <class T> struct CIntelligentPointer { T* ptr;T* operator->()const{return ptr;} };
class CQuestManager {
public:
    static CQuestManager* current;
    std::list<CIntelligentPointer<CThing> > cards;
    const std::list<CIntelligentPointer<CThing> >& GetActiveQuestCards() const{return cards;}
    static CQuestManager& Get(){require(current!=0);return *current;}
};
CQuestManager* CQuestManager::current=0;
class CTCInventoryMap {
public:void ConstructRegionDescriptionQuests(NUISystem::CComponent*,CRegion&,bool);
};
void CTCInventoryMap::ConstructRegionDescriptionQuests(NUISystem::CComponent* pcontainer,CRegion& region,bool pc_style)
{
    NUISystem::CManager* pmanager=NUISystem::CManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> pcomposite;
    std::list<CIntelligentPointer<CThing> > quests(CQuestManager::Get().GetActiveQuestCards());
    for(std::list<CIntelligentPointer<CThing> >::const_iterator it=quests.begin();it!=quests.end();++it) {
        CTCQuestCard* ptc_card;
        (*it)->GetTC(&ptc_card);
        if(ptc_card->GetRegionName()==region.GetName()) {
            if(ptc_card->IsCore()) {
                if(pc_style) pcomposite.Reset(pmanager->CreateComponent("PC_CORE_ICON_AND_NAME",false));
                else pcomposite.Reset(pmanager->CreateComponent("UI_CORE_ICON_AND_NAME",false));
            } else if(ptc_card->GetPDef()->IsVignette) {
                if(pc_style) pcomposite.Reset(pmanager->CreateComponent("PC_VIGNETTE_ICON_AND_NAME",false));
                else pcomposite.Reset(pmanager->CreateComponent("UI_VIGNETTE_ICON_AND_NAME",false));
            } else if(ptc_card->IsOptional()) {
                if(pc_style) pcomposite.Reset(pmanager->CreateComponent("PC_OPTIONAL_ICON_AND_NAME",false));
                else pcomposite.Reset(pmanager->CreateComponent("UI_OPTIONAL_ICON_AND_NAME",false));
            } else {
                if(pc_style) pcomposite.Reset(pmanager->CreateComponent("PC_CORE_ICON_AND_NAME",false));
                else pcomposite.Reset(pmanager->CreateComponent("UI_CORE_ICON_AND_NAME",false));
            }
            NUISystem::CComponent* ptext;
            if(pc_style) ptext=pcomposite->FindChild("PC_QUEST_NAME");
            else ptext=pcomposite->FindChild("UI_QUEST_NAME");
            if(ptext) {
                NUISystem::CText* ptext_real=static_cast<NUISystem::CText*>(ptext);
                ptext_real->SetValue(ptc_card->GetQuestName(),true,true);
            }
            pcontainer->AddChild(pcomposite);
        }
    }
}
static void check(int count,int flags,bool pc,unsigned regions,unsigned textMask) {
    CQuestManager quests;CQuestManager::current=&quests;
    NUISystem::CManager ui;NUISystem::CManager::active=&ui;ui.available=textMask;
    CRegion location;location.Name="Oakvale";
    CThing things[5];const char* names[]={"A","B","C","D","E"};
    NUISystem::CComponent root;CTCInventoryMap map;
    for(int i=0;i<count;++i) {
        CTCQuestCard& c=things[i].card;int kind=(flags+i)%8;
        c.region=(regions&(1U<<i))?"Oakvale":"Bowerstone";
        c.core=(kind&1)!=0;c.optional=(kind&2)!=0;c.def.IsVignette=(kind&4)!=0;
        c.title=names[i];CIntelligentPointer<CThing> p;p.ptr=&things[i];quests.cards.push_back(p);
    }
    map.ConstructRegionDescriptionQuests(&root,location,pc);
    unsigned index=0;
    for(int i=0;i<count;++i)if(regions&(1U<<i)){
        const int kind=(flags+i)%8;
        std::string prefix=pc?"PC_":"UI_";
        std::string type=(kind&1)?"CORE":(kind&4)?"VIGNETTE":(kind&2)?"OPTIONAL":"CORE";
        require(index<root.children.size());NUISystem::CComponent* widget=root.children[index];
        require(widget==&ui.widgets[index] && !widget->refs && widget->title==prefix+type+"_ICON_AND_NAME");
        require(widget->lookups==1 && widget->lookup==prefix+"QUEST_NAME");
        const bool hasText=(textMask&(1U<<index))!=0;
        require(ui.labels[index].updates==(hasText?1:0) && ui.labels[index].title==(hasText?names[i]:""));++index;
    }
    require(ui.count==index && root.children.size()==index && quests.cards.size()==(unsigned)count);
}
int main(){
    for(int count=0;count<=5;++count)for(int flags=0;flags<8;++flags)for(int pc=0;pc<2;++pc)
        for(unsigned regions=0;regions<(1U<<count);++regions)for(unsigned text=0;text<(1U<<count);++text)
            check(count,flags,pc!=0,regions,text);
    puts("005fd5e0_TEST PASS");return 0;
}