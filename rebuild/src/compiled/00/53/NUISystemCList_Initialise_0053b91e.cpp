// NUISystem::CList::Initialise
// Leaf: ?Initialise@CList@NUISystem@@UAEXXZ
// Flags: none
#pragma optimize("s", on)
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CLISTARROW_VSLOTS
#define FABLE_RETAIL_CLISTARROW_MEMBERS
#define FABLE_RETAIL_CSCROLLBAROUTSIDE_SCROLLBAR_PASS26
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_NOINLINE
#define FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_VISIBLE_PASS26
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CListArrow.h"
#include "engine/NUISystem_CScrollBarOutside.h"
#include "engine/CDefPointer.h"
#include "engine/GFDebugPDynamicCast.h"

#pragma optimize("y", off)
void NUISystem::CList::Initialise()
{
    CChangingStateComponent::Initialise();
    LastInputTime = Time;
    for (unsigned long count = 0; count < ScrollingChildren.size(); count++)
        ScrollingChildrenStates.push_back(ScrollingChildren[count]->GetStates());
    if (!ScrollingChildren.empty()) {
        if (ScrollingChildren.size() & 1)
            SetSelectedChild(ScrollingChildren.size() / 2);
        else
            SetSelectedChild(ScrollingChildren.size() / 2 - 1);
        if (LoadAndReturnDef()->PCStyle)
            SetSelectedChild(0);
    }
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
        PListUp = Children[Children.size() - 1].Get();
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
        PListDown = Children[Children.size() - 1].Get();
    }
    if (pdefinition->MeshIndex) {
        Children.push_back(CCountedPointer<CComponent>(CManager::GetInstance()->CreateComponent(pdefinition->MeshIndex)));
        NonScrollingChildren.push_back(Children[Children.size() - 1].Get());
        Children[Children.size() - 1]->Initialise();
        Children[Children.size() - 1]->SetParent(this);
        PScrollBar = Children[Children.size() - 1].Get();
        Children.push_back(CCountedPointer<CComponent>(CManager::GetInstance()->CreateComponent("PC_LIST_SCROLL_BAR_POSITION_DUMMY", false)));
        NonScrollingChildren.push_back(Children[Children.size() - 1].Get());
        Children[Children.size() - 1]->Initialise();
        Children[Children.size() - 1]->SetParent(this);
        Children[Children.size() - 1]->AddPositionChild(PScrollBar);
    }
    if (pdefinition->AnimationIndex) {
        Children.push_back(CCountedPointer<CComponent>(CManager::GetInstance()->CreateComponent(pdefinition->AnimationIndex)));
        NonScrollingChildren.push_back(Children[Children.size() - 1].Get());
        Children[Children.size() - 1]->Initialise();
        Children[Children.size() - 1]->SetParent(this);
        PScrollBarOutside = Children[Children.size() - 1].Get();
        Children.push_back(CCountedPointer<CComponent>(CManager::GetInstance()->CreateComponent("PC_LIST_SCROLL_BAR_POSITION_DUMMY", false)));
        NonScrollingChildren.push_back(Children[Children.size() - 1].Get());
        Children[Children.size() - 1]->Initialise();
        Children[Children.size() - 1]->SetParent(this);
        Children[Children.size() - 1]->AddPositionChild(PScrollBarOutside);
        if (PScrollBar && PScrollBarOutside) {
            CScrollBarOutside* pscroll_bar_outside = GFDebugPDynamicCast(PScrollBarOutside, static_cast<CScrollBarOutside*>(0));
            pscroll_bar_outside->SetScrollBar(PScrollBar);
        }
    }
}

#pragma optimize("y", on)