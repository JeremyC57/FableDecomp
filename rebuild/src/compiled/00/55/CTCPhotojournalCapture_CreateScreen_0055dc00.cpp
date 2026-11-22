// CTCPhotojournalCapture::CreateScreen -- create and wire the caption capture screen.
// Leaf: ?CreateScreen@CTCPhotojournalCapture@@AAEXXZ
// Flags: (none)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CACTION_ORIGINAL_DTOR_DECL_PASS26
#define FABLE_RETAIL_CACTION_COUNTED_DELETE_DECL_PASS26
#define FABLE_RETAIL_STLP_ACTION_PARAM_LIST_SIZE_OWNERS_PASS26
#define FABLE_RETAIL_STLP_ACTION_PARAM_LIST_ALLOCATE_INLINE_PASS26
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCINVENTORY_ORIGINAL_COMPLETE_CLASS_PASS22
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CAPTUREBACKBUFFER
#define FABLE_RETAIL_STLP_COMPONENT_FILL_N_VISIBLE_PASS26
#define FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_COMPONENT_OVERFLOW_ALLOCATE_INLINE_PASS26
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_NUISYSTEM_CCOMPONENT_SIZE_BODY
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NUISYSTEM_CCOMPONENT_SIZE_BODY
#define FABLE_RETAIL_STLP_FILL_N_FASTCALL
#define FABLE_RETAIL_STLP_COPY_TRIVIAL_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_action_params_list.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
#include "engine/CTCPhotojournalCapture.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CEditBox.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CAction_CountedPointerOwner.h"
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