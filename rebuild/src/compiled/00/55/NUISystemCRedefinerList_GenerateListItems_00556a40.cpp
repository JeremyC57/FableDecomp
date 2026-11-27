// 00556a40 NUISystem::CRedefinerList::GenerateListItems
// Leaf: ?GenerateListItems@CRedefinerList@NUISystem@@IAEXABV?$vector@VCActionInputControl@@V?$allocator@VCActionInputControl@@@std@@@std@@@Z
// Flags: none
#define FABLE_RETAIL_REDEFINER_LIST_GENERATE_PASS29
#define FABLE_RETAIL_CACTIONINPUTCONTROL_DEFAULT_CTOR
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_LIST_CONST_FRONT
#define FABLE_RETAIL_STLP_LIST_REMOVE_DECL
// Inline-budget: the const tree lookup at the first map search is a call. Its search loop expands within the callee.
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_NOINLINE
#define FABLE_RETAIL_STLP_RB_INCREMENT_SIZE_OWNER_PASS27
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#define FABLE_RETAIL_CMANAGER
#include "engine/CActionInputControl.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CKeyRedefiner.h"
#include "engine/NUISystem_CRedefinerList.h"
#include "engine/NUISystem_CManager.h"
#ifdef _MSC_VER
// Fidelity: the linked UI counted-pointer constructors and release functions are size-optimized.
#pragma optimize("s",on)
#endif
template<> inline CCountedPointer<NUISystem::CComponent>::CCountedPointer(NUISystem::CComponent* data): Data(data)
{
    if (data) Info=new CCPPointerInfo(1,DeleteData,Data);
    else Info=0;
}
template<> inline void CCountedPointer<NUISystem::CComponent>::ReleaseData()
{
    if(Info) {
        --Info->RefCount;
        if(Info->RefCount==0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    Data=0;
    Info=0;
}
template<> inline CCountedPointer<NUISystem::CComponent>::~CCountedPointer() { ReleaseData(); }
#ifdef _MSC_VER
#pragma optimize("",on)
#endif

namespace std {
template<> inline void list<NUISystem::CKeyRedefiner*>::remove(NUISystem::CKeyRedefiner* const& value)
{
    iterator first=begin(),last=end();
    while(first!=last) {
        iterator next=first;
        ++next;
        if(value==*first) erase(first);
        first=next;
    }
}
}
namespace NUISystem {
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