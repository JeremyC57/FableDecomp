// NUISystem::CManager::CreateComponent(long) -- component allocation factory.
// Leaf: ?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@J@Z
#define FABLE_RETAIL_CRESOURCE_PRIVATE_MEMBERS
#define FABLE_RETAIL_IVREFCOUNT_PRIVATE
#define FABLE_RETAIL_DEFPTR_ORDINARY_INLINE
// Flags: /Os /Oy-
#define FABLE_RETAIL_UI_FACTORY
#define FABLE_RETAIL_CMENUENTRY
#define FABLE_STLPORT_UI_ACTIONS
#define FABLE_STLPORT_CONSTRUCTORS
#define FABLE_RETAIL_UI_ACTIONS
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEFINITION_MANAGER
#include "rebuild_abi.h"
#include "boost/scoped_ptr.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CFrontEndList.h"
#include "engine/CFrontEndScreen.h"
#include "engine/CMorphingSprite.h"
#include "engine/CMovie.h"
#include "engine/CScrollingComponent.h"
#include "engine/CTable.h"
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/NUISystem_CClickable.h"
#include "engine/NUISystem_CComponentContainer.h"
#include "engine/NUISystem_CControllerDisconnect.h"
#include "engine/NUISystem_CDraggable.h"
#include "engine/NUISystem_CDraggableInto.h"
#include "engine/NUISystem_CDynamicList.h"
#include "engine/NUISystem_CEditBox.h"
#include "engine/NUISystem_CFrontEndButton.h"
#include "engine/NUISystem_CIconText.h"
#include "engine/NUISystem_CKeyRedefiner.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CListArrow.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/NUISystem_CMesh.h"
#include "engine/NUISystem_CMouseCursor.h"
#include "engine/NUISystem_CNavButton.h"
#include "engine/NUISystem_COk.h"
#include "engine/NUISystem_CParticleEffect.h"
#include "engine/NUISystem_CRedefinerList.h"
#include "engine/NUISystem_CScrollBar.h"
#include "engine/NUISystem_CScrollBarOutside.h"
#include "engine/NUISystem_CScrollableList.h"
#include "engine/NUISystem_CScrollingViewport.h"
#include "engine/NUISystem_CSlider.h"
#include "engine/NUISystem_CSpellContainer.h"
#include "engine/NUISystem_CSpellContainerList.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CSwappingStateComponent.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CTextContainer.h"
#include "engine/NUISystem_CTextSlider.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CYesNo.h"
#include "engine/NUISystem_CZoomingComponent.h"

#pragma auto_inline(off)
template<> __declspec(noinline) bool CDefinitionManager::GetDef<NUISystem::CUIDef>(long index,CDefPointer<const NUISystem::CUIDef>* out)const {
    if(index>0){CDefPointer<const CDefClassBase> value=GetPDefFromGlobalIndex(index);if(!value.IsNull()){out->SetDynamicCast(value);return true;}}
    return false;
}
__declspec(noinline) CDefString CDefinitionManager::GetDefNameFromGlobalIndex(long index)const {return InstantiatedDefs[index]->InstantiationName;}
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
