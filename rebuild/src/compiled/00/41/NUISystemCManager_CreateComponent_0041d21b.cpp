// NUISystem::CManager::CreateComponent(long) -- component allocation factory.
// Leaf: ?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@J@Z
// Flags: /Os /Oy-
// Retail: Fable.exe 0x0041D21B, RELOCATION_MATCH (chunk 849 rebase of the held Codex pass 12 source).
// Callees: CDefinitionManager::GetDef<NUISystem::CUIDef> (same-TU noinline body under auto_inline(off)), operator new, the 44 UI
//   component constructors (their retail sizes come from the *_MEMBERS opt-ins and the new NUISystem headers), CDefString::GetString.
// Shape: Codex body unchanged; the visible noinline GetDef body stops VC7.1 reloading and null-testing the definition pointer.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCOMPONENTCONTAINER
#define FABLE_RETAIL_CYESNO
#define FABLE_RETAIL_CYESNO_CTOR
#define FABLE_RETAIL_CCOMPONENTCONTAINER_CTORS
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTEXT
#define FABLE_RETAIL_CTEXT_LAYOUT
#define FABLE_RETAIL_CMOVIE
#define FABLE_RETAIL_CMOVIE_MEMBERS
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CFRONTENDSCREEN_MEMBERS
#define FABLE_RETAIL_CFRONTENDBUTTON_MEMBERS
#define FABLE_RETAIL_CLISTARROW_MEMBERS
#define FABLE_RETAIL_CSWAPPINGSTATECOMPONENT_MEMBERS
#define FABLE_RETAIL_CTEXTCONTAINER_MEMBERS
#define FABLE_RETAIL_CZOOMINGCOMPONENT_MEMBERS
#define FABLE_RETAIL_CSPELLCONTAINER_MEMBERS
#define FABLE_RETAIL_CDYNAMICLIST_MEMBERS
#include "fable_string.h"
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CMouseCursor.h"
#include "engine/NUISystem_CClickable.h"
#include "engine/NUISystem_CScrollBar.h"
#include "engine/NUISystem_CScrollBarOutside.h"
#include "engine/NUISystem_CDraggableInto.h"
#include "engine/NUISystem_CNavButton.h"
#include "engine/NUISystem_CKeyRedefiner.h"
#include "engine/NUISystem_CRedefinerList.h"
#include "engine/NUISystem_CScrollableList.h"
#include "engine/NUISystem_CMorphingSprite.h"
#include "engine/NUISystem_CTable.h"
#include "engine/NUISystem_CMesh.h"
#include "engine/CText.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CFrontEndScreen.h"
#include "engine/NUISystem_CFrontEndButton.h"
#include "engine/NUISystem_CFrontEndList.h"
#include "engine/NUISystem_CScrollingViewport.h"
#include "engine/NUISystem_CListArrow.h"
#include "engine/NUISystem_CSlider.h"
#include "engine/NUISystem_CTextSlider.h"
#include "engine/CMovie.h"
#include "engine/NUISystem_CSwappingStateComponent.h"
#include "engine/NUISystem_CScrollingComponent.h"
#include "engine/NUISystem_CTextContainer.h"
#include "engine/NUISystem_CZoomingComponent.h"
#include "engine/NUISystem_CComponentContainer.h"
#include "engine/NUISystem_CSpellContainer.h"
#include "engine/NUISystem_CSpellContainerList.h"
#include "engine/NUISystem_CYesNo.h"
#include "engine/NUISystem_CParticleEffect.h"
#include "engine/NUISystem_COk.h"
#include "engine/NUISystem_CControllerDisconnect.h"
#include "engine/NUISystem_CIconText.h"
#include "engine/NUISystem_CDynamicList.h"
#include "engine/NUISystem_CEditBox.h"
#pragma auto_inline(off)
template<> __declspec(noinline) bool CDefinitionManager::GetDef<NUISystem::CUIDef>(long index,CDefPointer<const NUISystem::CUIDef>* out)const {
    if(index>0){CDefPointer<const CDefClassBase> value=GetPDefFromGlobalIndex(index);if(!value.IsNull()){out->SetDynamicCast(value);return true;}}
    return false;
}
#pragma auto_inline(on)

NUISystem::CComponent* NUISystem::CManager::CreateComponent(long definition)
{
    CDefPointer<const CUIDef> pdef;
    CComponent* presult=0;
    if(m_pDefinitionManager)m_pDefinitionManager->GetDef(definition,&pdef);
    else CGameDefinitionManager::Get().GetDef(definition,&pdef);
    switch(pdef->Type) {
    case UI_TYPE_SPRITE:presult=new CSprite(definition);break;
    case UI_TYPE_MOUSE_POINTER:presult=new CMouseCursor(definition);break;
    case UI_TYPE_HOVERABLE:presult=new CHoverable(definition);break;
    case UI_TYPE_CLICKABLE:presult=new CClickable(definition);break;
    case UI_TYPE_DRAGGABLE:presult=new CDraggable(definition);break;
    case UI_TYPE_SCROLLBAR:presult=new CScrollBar(definition);break;
    case UI_TYPE_SCROLLBAR_OUTSIDE:presult=new CScrollBarOutside(definition);break;
    case UI_TYPE_DRAGGABLE_INTO:presult=new CDraggableInto(definition);break;
    case UI_TYPE_NAVIGATION_BUTTON:presult=new CNavButton(definition);break;
    case UI_TYPE_KEY_REDEFINER:presult=new CKeyRedefiner(definition);break;
    case UI_TYPE_REDEFINER_LIST:presult=new CRedefinerList(definition);break;
    case UI_TYPE_SCROLLABLE_LIST:presult=new CScrollableList(definition);break;
    case UI_TYPE_MORPHING_SPRITE:presult=new CMorphingSprite(definition);break;
    case UI_TYPE_COMPOSITE:presult=new CComponent(definition);break;
    case UI_TYPE_TABLE:presult=new CTable(definition);break;
    case UI_TYPE_MESH:presult=new CMesh(definition);break;
    case UI_TYPE_TEXT:presult=new CText(definition);break;
    case UI_TYPE_MENU_ENTRY:presult=new CMenuEntry(definition);break;
    case UI_TYPE_LIST:presult=new CList(definition);break;
    case UI_TYPE_CHANGING_STATE_COMPOSITE:presult=new CChangingStateComponent(definition);break;
    case UI_TYPE_VIEWPORT:presult=new CViewport(definition);break;
    case UI_TYPE_FRONTEND_SCREEN:presult=new CFrontEndScreen(definition);break;
    case UI_TYPE_FRONTEND_BUTTON:presult=new CFrontEndButton(definition);break;
    case UI_TYPE_FRONTEND_LIST:presult=new CFrontEndList(definition);break;
    case UI_TYPE_SCROLLING_VIEWPORT:presult=new CScrollingViewport(definition);break;
    case UI_TYPE_LIST_ARROW:presult=new CListArrow(definition);break;
    case UI_TYPE_SLIDER:presult=new CSlider(definition);break;
    case UI_TYPE_TEXT_SLIDER:presult=new CTextSlider(definition);break;
    case UI_TYPE_MOVIE:presult=new CMovie(CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine(),definition);break;
    case UI_TYPE_SWAPPING_STATE_COMPOSITE:presult=new CSwappingStateComponent(definition);break;
    case UI_TYPE_SCROLLING_COMPOSITE:presult=new CScrollingComponent(definition);break;
    case UI_TYPE_TEXT_CONTAINER:presult=new CTextContainer(definition);break;
    case UI_TYPE_ZOOMING_COMPOSITE:presult=new CZoomingComponent(definition);break;
    case UI_TYPE_COMPONENT_CONTAINER:presult=new CComponentContainer(definition);break;
    case UI_TYPE_SPELL_CONTAINER:presult=new CSpellContainer(definition);break;
    case UI_TYPE_SPELL_CONTAINER_LIST:presult=new CSpellContainerList(definition);break;
    case UI_TYPE_YESNO:presult=new CYesNo(definition);break;
    case UI_TYPE_PARTICLE_EFFECT:presult=new CParticleEffect(definition);break;
    case UI_TYPE_OK:presult=new COk(definition);break;
    case UI_TYPE_CONTROLLERDISCONNECT:presult=new CControllerDisconnect(definition);break;
    case UI_TYPE_ICON_TEXT:presult=new CIconText(definition);break;
    case UI_TYPE_DYNAMIC_LIST:presult=new CDynamicList(definition);break;
    case UI_TYPE_EDIT_BOX:presult=new CEditBox(definition);break;
    }
    if(m_pDefinitionManager){
        CCharString name=m_pDefinitionManager->GetDefNameFromGlobalIndex(definition).GetString();
        presult->SetName(name);
        return presult;
    }else{
        CCharString name=CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(definition).GetString();
        presult->SetName(name);
        return presult;
    }
}