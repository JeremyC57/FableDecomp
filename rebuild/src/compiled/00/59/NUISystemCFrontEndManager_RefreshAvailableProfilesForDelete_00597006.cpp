// NUISystem::CFrontEndManager::RefreshAvailableProfilesForDelete() -- rebuild the delete-profile screen's list: one button per saved profile, each carrying a "delete this profile" action; returns the profile count
// Leaf: ?RefreshAvailableProfilesForDelete@CFrontEndManager@NUISystem@@IAEJXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe @ 0x00597006, 546 bytes, `ret`; frame-pointer size TU (`#pragma optimize("y", off)` + `("s", on)`).
// Body: GetProfileNames(names) (0x005955AB, landed); the delete screen (m_mapScreens[21], map operator[] 0x0059B5D7)
// drops its old "UI_FRONTEND_LIST_FOR_PROFILES_FOR_DELETE" child (FindChild, slot 3, CCharString by value; the child gets a
// type-1 CAction by value through slot 62: CAction 0x0042BED4 / copy 0x0042CD84 / list dtor 0x0042ABCA); a new list of that
// definition comes from CManager::CreateComponent (0x0041DB1D on CManager::GetInstance 0x0041E5F2) into a counted
// pointer (0x00429C15), converted to CCountedPointer<CScrollableList> (ShareData 0x0059AA86) and added to the screen (AddChild, slot 59).
// Per profile name: CreateFrontEndButtonComponent(name, y, "UI_FRONTEND_BUTTON_FOR_PROFILES_LIST", "", false) (0x00596A68,
// landed), a new CAction (0x10 bytes, ctor 0x0042BE50, counted 0x0042AA29) of type 0xD7 holding a new CActionParamString(name)
// (0x0054DAE0) pushed on its UIActionParams (0x0042B646), given to the button (slots 66 and 71) and the button added to
// the list (slot 60); y advances by 30. Finally the screen's slot 43, the counted pointers' dtors (0x0059A994, 0x00429160,
// 0x004291DE) and ~vector<CWideString> (0x00412130).
// Layout: engine/CFrontEndManager.h m_mapScreens +0x54 (local model of the retail class).
// Shape: retail walks the names with a pointer plus a down counter and keeps the list's Data in ebx; VC7.1 does that only when the
//   callees taking a local's address are same-TU bodies that never store it. NF73 (2026-11-17): GetProfileNames 0x005955AB is visible
//   (engine/CFrontEndManager.h FABLE_RETAIL_CFRONTENDMANAGER_GETPROFILENAMES_BODY) with the STLport push_back / _M_insert_overflow /
//   erase(first, last) it calls visible but out of line (PUSHBACK_NOINLINE, OVERFLOW_OUTOFLINE, ERASE_RANGE_NOINLINE), and the counted
//   pointers' ReleaseData is visible but out of line (engine/CCountedPointer.h FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_NOINLINE): with
//   ReleaseData declared only, 542v546 (0 kept in ebx instead of the list's Data). Every local model of NF51's draft replaced by the shared
//   headers; protected per Ego_r.pdb / FableWin.pdb (IAEJXZ; the landed row was public QAEKXZ, its caller 0x0059A238 goes to NAME_FIXES.tsv).
// Name fix (agentNF 2026-11-10): 0x004291DE is CCountedPointer<NUISystem::CComponent>::ReleaseData (the destructor is the 5-byte
//   `jmp` COMDAT 0x00429C53; Ego_r.pdb ??1 is 5 bytes): FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (engine/CCountedPointer.h).
#pragma optimize("s", on)
#pragma optimize("y", off)
// NF75 (round 7, land1117fu q10944): the list pointer is CCountedPointer<NUISystem::CScrollableList> (engine/NUISystem_CScrollableList.h), not
//   CCountedPointer<CList>: Ego_d.pdb's ui_frontend_manager.obj (this TU) contributes CCountedPointer<NUISystem::CScrollableList>::ReleaseData /
//   ShareData / SetStaticCast<CComponent>, FableWin.pdb has both, and the byte-identical CList pair is 0x00429288 / 0x00429486 (retail has no
//   identical-code folding). So 0x0059A994 / 0x0059AA86 now bind the CScrollableList spellings: one name per address.
#define FABLE_RETAIL_CCHARSTRING_ENDSWITH
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_SCANNEXTFILE
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_NOINLINE
#define FABLE_RETAIL_CFRONTENDMANAGER_GETPROFILENAMES_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CFRONTENDMANAGER_DESTRUCTOR
#define FABLE_RETAIL_CFRONTENDMANAGER_SAVEDGAMES
#define FABLE_RETAIL_CFRONTENDMANAGER_BUILDCOMPONENTS
#define FABLE_RETAIL_CFRONTENDMANAGER_REFRESHAVAILABLEPROFILES
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_NOINLINE
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"
#include "engine/CFrontEndManager.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_EActionType.h"
#include "engine/NUISystem_CActionParamString.h"
#include "engine/NUISystem_CScrollableList.h"

namespace NUISystem {
long CFrontEndManager::RefreshAvailableProfilesForDelete()
{
    std::vector<CWideString> names;
    GetProfileNames(names);
    CManager* manager = CManager::GetInstance();
    CComponent* screen = m_mapScreens[FRONTEND_SCREEN_PROFILES_FOR_DELETE_PC];
    CComponent* oldList = screen->FindChild("UI_FRONTEND_LIST_FOR_PROFILES_FOR_DELETE");
    if (oldList) {
        CDeletion deletion;
        deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
        oldList->SetDeletion(deletion);
    }
    CCountedPointer<CComponent> listComponent(manager->CreateComponent("UI_FRONTEND_LIST_FOR_PROFILES_FOR_DELETE", false));
    CCountedPointer<CScrollableList> list;
    list.SetStaticCast(listComponent);
    screen->AddChild(listComponent);
    float y = 0.0f;
    const unsigned long count = names.size();
    for (unsigned long i = 0; i < count; ++i) {
        CCountedPointer<CComponent> button(CreateFrontEndButtonComponent(names[i], y, "UI_FRONTEND_BUTTON_FOR_PROFILES_LIST", "", false));
        CCountedPointer<CAction> action(new CAction);
        CActionParam* param = new CActionParamString(names[i]);
        action->ActionType = UI_ACTION_TYPE_GO_TO_DELETE_PROFILE_SCREEN;
        action->UIActionParams.push_back(param);
        button->AddAction(action);
        button->AddActionOnLeftClicked(action);
        list->AddChildDynamically(button);
        y += 30.0f;
    }
    screen->Initialise();
    return count;
}
}
