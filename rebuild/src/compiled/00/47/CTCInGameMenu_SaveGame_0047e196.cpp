// CTCInGameMenu::SaveGame -- manual save and confirmation UI.
// Inline-budget: native00485DBC expands allocator::allocate; plain, /d2inlT80/100/120/180 and scoped inline_depth255 retain a call. Only this genuine primitive uses FABLE_FORCEINLINE; see ALLOCATOR_INLINE_LADDER.json.
// Retail: 0x0047E196, full1211 bytes, thiscall, ret4; original locals/scopes from the actual donor procedure.
// Flags: /O1 /Oy-
// Worker validation: target plus seven called STLport bodies match independently; strict relocations audited.
// Genuine helper profiles: frame-omitted erase/fill_n/list insert/node creation precede the frame-pointer target.
// No stand-in bodies or substitute method/class names. External engine/string/UI contracts remain declarations.
// Leaf: ?SaveGame@CTCInGameMenu@@QAEXJ@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#define FABLE_RETAIL_STLP_COPY_TRIVIAL_OUTOFLINE
#define FABLE_RETAIL_STLP_FILL_N_FASTCALL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CYESNO
#define FABLE_RETAIL_CYESNO_CALLBACKS
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINGAMEMENU
#define FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD
#define FABLE_RETAIL_CTCINGAMEMENU_VIDEODIALOGS
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDATEANDTIME_VALID
#define FABLE_RETAIL_CTCINGAMEMENU_SAVEGAME
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SETSAVESLOT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_SAVEGAMESTATE
#define FABLE_RETAIL_CWORLD_SAVEDISKSPACE
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CQUESTMANAGER
#include "engine/CMainGameComponent.h"
#include "engine/CUserProfileManager.h"
#include "engine/CQuestManager.h"
#include "engine/CWorld.h"
#include "engine/C3DVector.h"
#include "engine/CMessageEventManager.h"
#include "stlport/fable_containers.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CYesNo.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#include "engine/GFGetTime.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInGameMenu.h"


void FABLE_FASTCALL SavedConfirmed(); // native 0x0047DB73
using namespace NUISystem;

namespace std {
// Original STLport pointer allocator operations are expanded in the retail overflow body.
template<> FABLE_FORCEINLINE CComponent** allocator<CComponent*>::allocate(size_t n, const void*)
{ return n ? static_cast<CComponent**>(malloc(n * sizeof(CComponent*))) : 0; }
template<> inline void allocator<CComponent*>::deallocate(CComponent** p, size_t)
{ if (p) free(p); }

#ifdef _MSC_VER
#pragma optimize("y", on)
#pragma optimize("s", on)
#pragma auto_inline(off)
#endif
// NF217 (2026-11-21, q12371): `inline` + a guarded noinline, as stlport/fable_containers.h's
//   FABLE_RETAIL_STLP_FILL_N_NUISYSTEM_CCOMPONENT_SIZE_BODY spells the same instance: the copy is a pick-any COMDAT like the one
//   0x006116E0 / 0x007E4B60 emit (one definition; it was a strong definition, LNK2005 beside them), still out of line and still
//   compiled in this optimize("y" / "s") region (a non-template inline explicit specialization takes the mode at its definition).
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
CComponent** FABLE_FASTCALL fill_n(CComponent** first, unsigned int n, CComponent* const& x)
{
    for (; n > 0; --n, ++first)
        *first = x;
    return first;
}

template<>
_List_node<CActionParam*>* list<CActionParam*>::_M_create_node(CActionParam* const& x)
{
    _List_node<CActionParam*>* p = this->_M_node.allocate(1);
    new(static_cast<void*>(&p->_M_data)) CActionParam*(x);
    return p;
}

template<>
list<CActionParam*>::iterator list<CActionParam*>::insert(iterator position, CActionParam* const& x)
{
    _List_node<CActionParam*>* tmp = _M_create_node(x);
    _List_node_base* n = position._M_node;
    _List_node_base* p = n->_M_prev;
    tmp->_M_next=n; tmp->_M_prev=p; p->_M_next=tmp; n->_M_prev=tmp;
    return iterator(tmp);
}

template<>
vector<CComponent*>::iterator vector<CComponent*>::erase(iterator position)
{
    if (position + 1 != end())
        __copy_trivial(position + 1, this->_M_finish, position);
    --this->_M_finish;
    _Destroy(this->_M_finish);
    return position;
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#pragma optimize("y", off)
#endif

} // namespace std

void CTCInGameMenu::SaveGame(long index)
{
    CWideString file_name;
    if (FileTimes[index].IsValid())
        file_name = FileNames[index];
    else if (CUserProfileManager::Get().IsDebugProfile())
        file_name = CUserProfileManager::Get().GetManualSaveFileName(FileNames.size());
    else {
        file_name = CUserProfileManager::Get().GetManualSaveFileName(index);
        file_name = L"Manual - " + file_name;
    }
    bool save_failed = false;
    if ((unsigned long)index < FileNames.size() && !CMainGameComponent::Get()->GetWorld().IsDiskSpaceForSaveGame())
        save_failed = true;
    else {
        CWideString path_name = CUserProfileManager::Get().GetPathNameFromFileName(file_name, true);
        if (path_name != L"") {
            if (!CUserProfileManager::Get().IsDebugProfile()) {
                if (index <= 0)
                    return;
                CUserProfileManager::Get().SetSaveSlotFileName(index - 1, file_name);
            }
            CMainGameComponent::Get()->GetWorld().SaveGameStateManual(file_name);
        }
    }
    while (GFGetTime() - SaveInProgressTime < 3.0) {
    }
    for (CComponent** it = Components.begin(); it != Components.end(); ++it) {
        if (*it == PSaveInProgress) {
            delete PSaveInProgress;
            Components.erase(it);
            break;
        }
    }
    if (!save_failed)
        GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_GAME_SAVED_MANUALLY, CMainGameComponent::GetConstantFPS(), C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, 0);
    CComponent* pComponent = CManager::GetInstance()->CreateComponent("PC_DIALOG_GAME_SAVED_GAME", false);
    if (CQuestManager::Get().IsBetweenQuestStartAndFinishScreens()) {
        CComponent* ptext_confirm = pComponent->FindChild("PC_GAME_SAVED_GAME_TEXT");
        if (ptext_confirm) {
            CText* ptext_confirm_real = static_cast<CText*>(ptext_confirm);
            ptext_confirm_real->SetValue("TEXT_GUI_CONFIRM_HERO_SAVED", true, true);
        }
    }
    if (save_failed) {
        CComponent* ptext_confirm = pComponent->FindChild("PC_GAME_SAVED_GAME_TEXT");
        if (ptext_confirm) {
            CText* ptext_confirm_real = static_cast<CText*>(ptext_confirm);
            ptext_confirm_real->SetValue("TEXT_GUI_SAVE_FAILED", true, true);
        }
    }
    pComponent->Initialise();
    Components.push_back(pComponent);
    m_pOKDialog = static_cast<CYesNo*>(pComponent);
    m_pOKDialog->SetYesCallBack(SavedConfirmed);
    m_pOKDialog->SetNoCallBack(SavedConfirmed);
    m_pOKDialog->ChangeState(1);
    CComponent* pclickable = m_pOKDialog->FindChild("PC_OK_GAME_OKBUTTON");
    if (pclickable) {
        CCountedPointer<CAction> paction(new CAction);
        paction->ActionType = UI_ACTION_TYPE_SEND_EVENT;
        paction->UIActionParams.push_back(new CActionParamSLONG(4));
        pclickable->AddActionOnLeftUnclicked(paction);
        CManager::GetInstance()->AddConcurrentExclusiveObserver(pclickable);
        pclickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    CManager::GetInstance()->RemoveObserver(m_pOKDialog);
    CManager::GetInstance()->AddConcurrentExclusiveObserver(m_pOKDialog);
    CManager::GetInstance()->RemoveObserver(PMousePointer);
    CManager::GetInstance()->AddConcurrentExclusiveObserver(PMousePointer);
    PMousePointer->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    m_pOKDialog->ObserveAllEvents();
    C2DVector pos((CUIScreenUnitConverter::GetBackBufferWidth() * 0.5f) - CManager::GetInstance()->GetUIScale().X * 204.0f,
                                       CUIScreenUnitConverter::GetBackBufferHeight() * 0.5f - CManager::GetInstance()->GetUIScale().Y * 52.0f);
    m_pOKDialog->ChangePosition(pos, 0.0f, false);
}