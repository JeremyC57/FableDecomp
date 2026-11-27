#include <cstdio>
#include <cstdlib>
#include <list>
#include <vector>
#include <map>
#include "engine/EGameAction.h"
typedef unsigned CCharString;
enum EControllerType { CONTROLLER_NONE,CONTROLLER_XBOX_PAD,CONTROLLER_KEYBOARD,CONTROLLER_MOUSE };
enum EInputKey { KB_ESC=1 };
enum EMouseButtonControl { MOUSE_BUTTON_NULL_CONTROL=0,MOUSE_MOVEMENT=4,MOUSE_WHEEL_MOVEMENT=5 };
struct CActionInputControl {
    EGameAction GameAction;
    int ControllerType,KeyboardKey,MouseButton;
    bool IsDirectional() const { return GameAction==GAME_ACTION_MOVEMENT || GameAction==GAME_ACTION_BETTING; }
};
struct CRGBColour { unsigned r,g,b,a; CRGBColour(unsigned R,unsigned G,unsigned B,unsigned A):r(R),g(G),b(B),a(A){} };
template<class T> struct CDefPointer { T* data; CDefPointer(T* p):data(p){} T* operator->()const{return data;} };
template<class T> struct CCountedPointer { T* data; CCountedPointer(T* p):data(p){} T* operator->()const{return data;} };
static unsigned allowMask,createCount,events;
static int stateValue;
static void check(bool condition) { if (!condition) std::exit(1); }
namespace NUISystem {
struct CUIDef { long Redefiner;std::map<unsigned long,CCharString> ActionMap;std::map<unsigned long,unsigned long> ActionMapAliases;std::vector<unsigned long> ActionOrder; };
struct CComponent {
    int index,phase;std::vector<int> added;
    void AddChildDynamically(CCountedPointer<CComponent>& p){check(p->phase==2);p->phase=3;added.push_back(p->index);++events;}
    void ChangeState(unsigned long n,float t){check(phase==3 && n==(unsigned)stateValue && t==0.2f);phase=4;++events;}
    void ChangeColour(const CRGBColour& c,float t,bool level){check(phase==4 && !level && t==0.0f && !(c.r|c.g|c.b|c.a));phase=5;++events;}
};
struct CKeyRedefiner: CComponent {
    enum EGameActionSubType {ACTION_SUBTYPE_NONE=0,ACTION_SUBTYPE_UP=10,ACTION_SUBTYPE_DOWN,ACTION_SUBTYPE_LEFT,ACTION_SUBTYPE_RIGHT};
    EGameAction action;int kind;CCharString label;std::vector<EGameAction> aliases;
    void Initialise(){check(phase==0);phase=1;++events;}
    void SetAction(const CActionInputControl& c,const CCharString& text){check(phase==1);phase=2;action=c.GameAction;kind=c.MouseButton;label=text;++events;}
    EGameAction GetAction() const{return action;}
    EGameActionSubType GetSubType() const{return (EGameActionSubType)kind;}
    void AddAlias(EGameAction value){check(phase==2);aliases.push_back(value);++events;}
};
static CKeyRedefiner items[40];static unsigned used;
struct CManager {
    static CManager* GetInstance(){static CManager mgr;return &mgr;}
    CComponent* CreateComponent(long definition){check(definition==719 && createCount<30);const unsigned bit=createCount++;if(!(allowMask&(1u<<bit)))return 0;CKeyRedefiner& item=items[used];item.index=used++;item.phase=0;item.aliases.clear();return &item;}
};
struct CRedefinerList: CComponent {
    CUIDef def;unsigned long RequestedState;
    CDefPointer<const CUIDef> LoadAndReturnDef()const{return &def;}
    bool IsRedefinableControl(const CActionInputControl&)const;
    CKeyRedefiner* FindRedefinerForAlias(EGameAction,std::list<CKeyRedefiner*>&,CKeyRedefiner::EGameActionSubType)const;
    void AddAlias(const std::map<unsigned long,unsigned long>::const_iterator&,std::list<CKeyRedefiner*>&,CKeyRedefiner::EGameActionSubType);
    void GenerateListItems(const std::vector<CActionInputControl>&);
};
CKeyRedefiner* CRedefinerList::FindRedefinerForAlias(EGameAction action, std::list<CKeyRedefiner*>& rList, CKeyRedefiner::EGameActionSubType subType) const
{
    for(std::list<CKeyRedefiner*>::iterator it=rList.begin(); it!=rList.end(); ++it)
        if((*it)->GetAction()==action && (*it)->GetSubType()==subType)
            return *it;
    return 0;
}
inline bool CRedefinerList::IsRedefinableControl(const CActionInputControl& inputControl) const
{
    switch (inputControl.ControllerType) {
    case CONTROLLER_KEYBOARD: switch(inputControl.KeyboardKey) { case KB_ESC: return false; } break;
    case CONTROLLER_XBOX_PAD: return false;
    case CONTROLLER_MOUSE: switch(inputControl.MouseButton) { case MOUSE_BUTTON_NULL_CONTROL: case MOUSE_MOVEMENT: case MOUSE_WHEEL_MOVEMENT: return false; } break;
    case CONTROLLER_NONE: return false;
    }
    return true;
}
inline void CRedefinerList::AddAlias(const std::map<unsigned long,unsigned long>::const_iterator& alias,std::list<CKeyRedefiner*>& rList,CKeyRedefiner::EGameActionSubType subType)
{
    CKeyRedefiner* pKeyRedefiner=FindRedefinerForAlias((EGameAction)alias->second,rList,subType);
    if(pKeyRedefiner) pKeyRedefiner->AddAlias((EGameAction)alias->first);
}
void CRedefinerList::GenerateListItems(const std::vector<CActionInputControl>& controls)
{
    std::list<CKeyRedefiner*> redefinerList;
    CDefPointer<const CUIDef> pDefinition=LoadAndReturnDef();
    bool needToSkipFirstContextAction=true;
    for(std::vector<CActionInputControl>::const_iterator it=controls.begin();it!=controls.end();++it) {
        if(needToSkipFirstContextAction && it->GameAction==GAME_ACTION_CONTEXT_SENSITIVE_ITEM) {
            needToSkipFirstContextAction=false;
            continue;
        }
        if(!IsRedefinableControl(*it)) continue;
        std::map<unsigned long,CCharString>::const_iterator actionMap=pDefinition->ActionMap.find(it->GameAction);
        if(actionMap!=pDefinition->ActionMap.end()) {
            CComponent* pKeyRedefinerComponent=CManager::GetInstance()->CreateComponent(pDefinition->Redefiner);
            if(pKeyRedefinerComponent) {
                CKeyRedefiner* pKeyRedefiner=static_cast<CKeyRedefiner*>(pKeyRedefinerComponent);
                pKeyRedefiner->Initialise();
                pKeyRedefiner->SetAction(*it,actionMap->second);
                redefinerList.push_back(pKeyRedefiner);
            }
        }
    }
    for(std::map<unsigned long,unsigned long>::const_iterator alias=pDefinition->ActionMapAliases.begin();alias!=pDefinition->ActionMapAliases.end();++alias) {
        CActionInputControl temp;
        temp.GameAction=static_cast<EGameAction>(alias->first);
        if(temp.IsDirectional()) {
            AddAlias(alias,redefinerList,CKeyRedefiner::ACTION_SUBTYPE_UP);
            AddAlias(alias,redefinerList,CKeyRedefiner::ACTION_SUBTYPE_DOWN);
            AddAlias(alias,redefinerList,CKeyRedefiner::ACTION_SUBTYPE_LEFT);
            AddAlias(alias,redefinerList,CKeyRedefiner::ACTION_SUBTYPE_RIGHT);
        } else {
            CKeyRedefiner::EGameActionSubType subType=CKeyRedefiner::ACTION_SUBTYPE_NONE;
            switch(alias->first) {
            case GAME_ACTION_INVENTORY_UP: case GAME_ACTION_ORACLE_MINIGAME_UP: subType=CKeyRedefiner::ACTION_SUBTYPE_UP;break;
            case GAME_ACTION_INVENTORY_DOWN: case GAME_ACTION_ORACLE_MINIGAME_DOWN: subType=CKeyRedefiner::ACTION_SUBTYPE_DOWN;break;
            case GAME_ACTION_INVENTORY_LEFT: case GAME_ACTION_ORACLE_MINIGAME_LEFT: subType=CKeyRedefiner::ACTION_SUBTYPE_LEFT;break;
            case GAME_ACTION_INVENTORY_RIGHT: case GAME_ACTION_ORACLE_MINIGAME_RIGHT: subType=CKeyRedefiner::ACTION_SUBTYPE_RIGHT;break;
            }
            AddAlias(alias,redefinerList,subType);
        }
    }
    for(std::vector<unsigned long>::const_iterator it=pDefinition->ActionOrder.begin();it!=pDefinition->ActionOrder.end();++it) {
        for(std::list<CKeyRedefiner*>::iterator redefiner=redefinerList.begin();redefiner!=redefinerList.end();) {
            CKeyRedefiner* pRedefiner=*redefiner;
            ++redefiner;
            if(*it==pRedefiner->GetAction()) {
                CCountedPointer<CComponent> pChild(pRedefiner);
                CComponent::AddChildDynamically(pChild);
                pChild->ChangeState(RequestedState,0.2f);
                pChild->ChangeColour(CRGBColour(0,0,0,0),0.0f,false);
                redefinerList.remove(pRedefiner);
            }
        }
    }
    while(!redefinerList.empty()) {
        CKeyRedefiner* pRedefiner=redefinerList.front();
        CCountedPointer<CComponent> pChild(pRedefiner);
        CComponent::AddChildDynamically(pChild);
        pChild->ChangeState(RequestedState,0.2f);
        pChild->ChangeColour(CRGBColour(0,0,0,0),0.0f,false);
        redefinerList.remove(pRedefiner);
    }
}
}
using namespace NUISystem;
struct Expected {int action,kind;unsigned label;std::vector<int> aliases;bool selected;};
static unsigned nextValue(unsigned& s){s=1664525u*s+1013904223u;return s;}
int main()
{
    const int keys[]={56,60,114,37,38,39,40,66,67,68,69,5};
    const int aliasTargets[]={5,37,60,56};
    for(unsigned step=0;step<2048;++step) {
        CRedefinerList list;std::vector<CActionInputControl> controls;std::vector<Expected> expected;std::vector<int> sequence;
        unsigned seed=step*127u+3u;stateValue=(int)(nextValue(seed)&255);list.RequestedState=stateValue;list.def.Redefiner=719;createCount=used=events=0;
        allowMask=step%5?nextValue(seed):~0u;
        for(unsigned k=0;k<12;++k)if(step%3 || k==0)list.def.ActionMap[keys[k]]=7001+k;
        for(unsigned k=0;k<12;++k) if(step%3!=1 || k%2) list.def.ActionMapAliases[keys[k]]=aliasTargets[step%4];
        for(unsigned k=0;k<8;++k) if(step%4)list.def.ActionOrder.push_back(keys[(step+k*3)%12]);
        unsigned count=step%17?19:0;
        for(unsigned k=0;k<count;++k) {
            CActionInputControl c;
            c.GameAction=static_cast<EGameAction>(k<2?56:keys[(nextValue(seed)>>12)%12]);
            c.ControllerType=(nextValue(seed)>>16)%5;c.KeyboardKey=(nextValue(seed)>>12)%4;c.MouseButton=(nextValue(seed)>>13)%15;
            if(step%4==0&&k>1){c.ControllerType=2;c.KeyboardKey=2;c.GameAction=static_cast<EGameAction>(aliasTargets[step%4]);c.MouseButton=k%5?9+k%5:0;}
            controls.push_back(c);
        }
        bool skipped=false;unsigned attempts=0;
        for(unsigned k=0;k<controls.size();++k){
            const CActionInputControl& c=controls[k];if(c.GameAction==56&&!skipped){skipped=true;continue;}
            if(c.ControllerType<=1||(c.ControllerType==2&&c.KeyboardKey==1)||(c.ControllerType==3&&(c.MouseButton==0||c.MouseButton==4||c.MouseButton==5)))continue;
            if(!list.def.ActionMap.count(c.GameAction))continue;
            if(!(allowMask&(1u<<attempts++)))continue;
            Expected e;e.action=c.GameAction;e.kind=c.MouseButton;e.selected=false;e.label=list.def.ActionMap[c.GameAction];expected.push_back(e);
        }
        unsigned aliasCount=0;for(std::map<unsigned long,unsigned long>::iterator entry=list.def.ActionMapAliases.begin();entry!=list.def.ActionMapAliases.end();++entry) {
            int key=entry->first;int lo=0,hi=0;
            if(key==60||key==114){lo=10;hi=13;}else if(key>=37&&key<=40){lo=hi=10+key-37;}else if(key>=66&&key<=69){lo=hi=10+key-66;}
            for(int n=lo;n<=hi;++n)for(unsigned i=0;i<expected.size();++i)
                if(expected[i].action==(int)entry->second && expected[i].kind==n){expected[i].aliases.push_back(key);++aliasCount;break;}
        }
        for(unsigned k=0;k<list.def.ActionOrder.size();++k)for(unsigned i=0;i<expected.size();++i)if(!expected[i].selected && (unsigned)expected[i].action==list.def.ActionOrder[k]){expected[i].selected=true;sequence.push_back(i);}
        for(unsigned i=0;i<expected.size();++i)if(!expected[i].selected)sequence.push_back(i);
        list.GenerateListItems(controls);
        check(createCount==attempts && used==expected.size() && list.added==sequence && events==5*used+aliasCount);
        for(unsigned i=0;i<used;++i){
            check(items[i].phase==5 && items[i].action==expected[i].action && items[i].label==expected[i].label && items[i].aliases.size()==expected[i].aliases.size());
            for(unsigned k=0;k<items[i].aliases.size();++k)check(items[i].aliases[k]==expected[i].aliases[k]);
        }
    }
    std::puts("00556a40_TEST PASS");
}