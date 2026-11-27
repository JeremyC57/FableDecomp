// 0054d660 NUISystem::CFrontEndList::Initialise
// Leaf: ?Initialise@CFrontEndList@NUISystem@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_FRONT_END_LIST_INITIALISE_PASS29
#define FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE_FASTCALL
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_NUISYSTEM_CCOMPONENT_SIZE_BODY
#define FABLE_RETAIL_STLP_FILL_N_FASTCALL
#define FABLE_RETAIL_STLP_COPY_TRIVIAL_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NUISYSTEM_CCOMPONENT_SIZE_BODY
#define FABLE_RETAIL_STLP_COMPONENT_OVERFLOW_ALLOCATE_INLINE_PASS26
#define FABLE_RETAIL_STLP_COMPONENT_FILL_N_VISIBLE_PASS26
#define FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#define FABLE_RETAIL_CCOUNTEDPOINTER
// NUISystem::CFrontEndList::Initialise
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CLISTARROW_VSLOTS
#define FABLE_RETAIL_CLISTARROW_MEMBERS
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CFrontEndList.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CListArrow.h"
#include "engine/CDefPointer.h"

void NUISystem::CFrontEndList::Initialise()
{
    CChangingStateComponent::Initialise();
    for (unsigned long count = 0; count < ScrollingChildren.size(); count++)
        ScrollingChildrenStates.push_back(ScrollingChildren[count]->GetStates());
    LastInputTime = Time;
    SelectedChild = 0;
    CDefPointer<const CUIDef> pdefinition = LoadAndReturnDef();
    CCountedPointer<CListArrow> pchild_up;
    if (pdefinition->UpArrow) {
        Children.push_back(CCountedPointer<CComponent>(CManager::GetInstance()->CreateComponent(pdefinition->UpArrow)));
        NonScrollingChildren.push_back(Children[Children.size() - 1].Get());
        pchild_up.SetStaticCast(Children[Children.size() - 1]);
        pchild_up->SetMinToDraw(pdefinition->UpLimit);
        pchild_up->SetMaxToDraw(ScrollingChildren.size());
        pchild_up->Initialise();
        Children[Children.size() - 1]->SetParent(this);
    }
    CCountedPointer<CListArrow> pchild_down;
    if (pdefinition->DownArrow) {
        Children.push_back(CCountedPointer<CComponent>(CManager::GetInstance()->CreateComponent(pdefinition->DownArrow)));
        NonScrollingChildren.push_back(Children[Children.size() - 1].Get());
        pchild_down.SetStaticCast(Children[Children.size() - 1]);
        pchild_down->SetMinToDraw(0);
        pchild_down->SetMaxToDraw(ScrollingChildren.size() - pdefinition->DownLimit - 1);
        pchild_down->Initialise();
        Children[Children.size() - 1]->SetParent(this);
    }
}