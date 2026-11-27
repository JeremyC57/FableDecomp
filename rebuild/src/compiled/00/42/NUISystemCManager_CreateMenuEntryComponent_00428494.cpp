// 00428494
// Leaf: ?CreateMenuEntryComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCWideString@@MMABVCCharString@@MV5@MV5@@Z
// Flags: /Gr
// Inline-budget: The size-built component constructor and assignment expand in the primary at 004285ce and 004284d9.
#define FABLE_RETAIL_MENU_BASICSTRING_SPEED_OWNER_PASS29
#define FABLE_RETAIL_STLP_CUISTATE_MAP_OWNERS_PASS28
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY
#define FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_LESS
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_LESS_CCHARSTRING_SIZE_BODY
#define FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_VISIBLE_PASS26
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_NOINLINE
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE
#include "fable_string.h"
#include "engine/NUISystem_CUIState.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CManager.h"

#ifdef _MSC_VER
#pragma optimize("y",on)
#pragma optimize("s",on)
#endif
// The size-owned component copy and assignment expand at 004285ce and 004284d9.
template<> FABLE_FORCEINLINE CCountedPointer<NUISystem::CComponent>::CCountedPointer(const CCountedPointer& p): Data(p.Data),Info(p.Info) {if(Info) ++Info->RefCount;}
template<> FABLE_FORCEINLINE CCountedPointer<NUISystem::CComponent>& CCountedPointer<NUISystem::CComponent>::operator=(const CCountedPointer& p) { ShareData(p.Data,p.Info);return *this; }
#ifdef _MSC_VER
#pragma optimize("y",off)
#pragma optimize("s",on)
#endif
namespace NUISystem {
CComponent* CManager::CreateMenuEntryComponent(const CWideString& text, float position,
    float alpha, const CCharString& name, float position_x, CCharString text_def,
    float text_position, CCharString button_def)
{
    if (BastardChildren.find(button_def) == BastardChildren.end()) {
        CCountedPointer<CComponent> cpbastard_child(CreateComponent(button_def,false));
        BastardChildren[button_def] = cpbastard_child;
    } else if (BastardChildren[button_def].IsNull()) {
        BastardChildren[button_def].Reset(CreateComponent(button_def,false));
    }
    CCountedPointer<CComponent> pcomponent;
    if (text_def == "")
        pcomponent.Reset(CreateComponent("UI_WEAPON_TEXT_TEMPLATE",false));
    else
        pcomponent.Reset(CreateComponent(text_def,false));
    CCountedPointer<CText> ptext;
    pcomponent->GetStates()[UI_STATE_INACTIVE].Position.X += text_position;
    pcomponent->GetStates()[UI_STATE_SELECTED].Position.X += text_position;
    pcomponent->GetStates()[UI_STATE_UNSELECTED].Position.X += text_position;
    ptext.SetStaticCast(CCountedPointer<CComponent>(pcomponent));
    ptext->SetValue(text,true,true);
    ptext->SetName("MENU_ENTRY_TEXT");
    CCountedPointer<CComponent> pbutton(CreateComponent("PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE",false));
    pbutton->AddChild(BastardChildren[button_def]);
    pbutton->AddChild(pcomponent);
    CComponent* pmenu_entry = CreateComponent(name,false);
    pmenu_entry->AddChild(pbutton);
    pmenu_entry->GetStates()[UI_STATE_ACTIVE].Position.Y = position;
    pmenu_entry->GetStates()[UI_STATE_UNSELECTED].Position.Y = position;
    pmenu_entry->GetStates()[UI_STATE_LIST_ACTIVE].Position.Y = position;
    if (pmenu_entry->IsPCStyle()) {
        pmenu_entry->GetStates()[UI_STATE_INACTIVE].Position.Y = position;
        pmenu_entry->GetStates()[UI_STATE_SELECTED].Position.Y = position;
    }
    pmenu_entry->GetStates()[UI_STATE_ACTIVE].Position.X = position_x;
    pmenu_entry->GetStates()[UI_STATE_UNSELECTED].Position.X = position_x;
    pmenu_entry->GetStates()[UI_STATE_LIST_ACTIVE].Position.X = position_x;

    if (alpha < 0.0f) alpha=0.0f;
    if (alpha > 1.0f) alpha=1.0f;
    pmenu_entry->GetStates()[UI_STATE_ACTIVE].Colour.A = static_cast<unsigned char>(alpha * 255.0f);
    pmenu_entry->GetStates()[UI_STATE_UNSELECTED].Colour.A = static_cast<unsigned char>(alpha * 255.0f);
    pmenu_entry->GetStates()[UI_STATE_LIST_ACTIVE].Colour.A = static_cast<unsigned char>(alpha * 255.0f);
    return pmenu_entry;
}
}

#ifdef _MSC_VER
#pragma optimize("y",on)
#pragma optimize("s",on)
#endif