// CUserProfileManager::ResetAssignedInputs() -- reload the passive and aggressive input assignments from the PC control-scheme defs
//   (left-click-is-king, two-mode or one-mode scheme) through whichever definition manager is live, then apply the values
// Leaf: ?ResetAssignedInputs@CUserProfileManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004085F0, 557 bytes, thiscall, plain `ret` (two epilogue copies).
// Identity: Ego_r.pdb ResetAssignedInputs 0x00408530 directly followed by ResetAssignedInputsWASD (= retail 0x004085F0 -> 0x00408820);
//   FableWin.pdb same leaf; landed callers NUISystem::CRedefinerList::ResetKeysToDefaults 0x005565C0 / GetAssignedInputForAction 0x00408C90.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 (implicit name temporaries),
//   CGameDefinitionManager::Get 0x0044C6B0 / CStartupDefinitionManager::Get 0x0043368D (__fastcall),
//   CDefinitionManager::GetDef<CControlsDef>(const CCharString&, CDefPointer<const CControlsDef>*) const 0x00410820,
//   vector<CActionInputControl>::operator= 0x00411B90, CUserProfileManager::ApplyValues 0x00407370 (protected), the def's Delete
//   (vtable slot 1) on the last release. Data: 0x013B85F4 g_bControlSchemeLeftClickIsKing (bool; the landed name and type of
//   CThingFilterIsValidTargetForPlayerCreature 0x00808E40; DESCRIPTIVE, no PDB data name), 0x013B85F3 g_bAggressiveModeAvailable,
//   CMainGameComponent::PMainGameComponent 0x013B86A0 (inline Get). The presence test `mov eax,[0x013B8760] ; test eax,eax ; je` (0x00408636 / 0x004086D8 / 0x004087AE) is the inlined FableWin
//   ?IsAllocated@CStartupDefinitionManager@@SI_NXZ (0x018377B4 / 0x01888A60; overlay opt-in FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER_
//   ISALLOCATED) over the counted pointer 0x013B8760 that owns the singleton (CStartupDefinitionManager::PClass: FableWin.pdb
//   dynamic initializer / atexit destructor names, agentNF31 overlay opt-in PCLASS; Get 0x0043368D is `mov eax,[0x013B8760] ; ret`, its atexit dtor 0x01228AF1 -> CCountedPointer<...>::ReleaseData 0x00433CC0).
// GetDef reads unqualified in both branches: FABLE_RETAIL_CGAMEDEFINITIONMANAGER hides the header's non-retail
//   CGameDefinitionManager::GetDef(long, CDefPointer<T>&) (absent from Ego_r / FableWin), so the call resolves to the base
//   CDefinitionManager::GetDef<CControlsDef> exactly as in the startup branch.
// Access: Ego_r / FableWin ?ResetAssignedInputs@CUserProfileManager@@QAEXXZ = public, non-virtual, thiscall, void(void).
// Shape: each scheme branch declares its OWN CDefPointer local (VC7.1 hoists the three identical null-inits above the first branch
//   and gives each branch its own release + ApplyValues tail, which is why the two-mode tail is not cross-jumped).
// GetDef body: the by-name template GetDef<T> is out of line in retail (aligned COMDAT 0x00410820 = landed row
//   rebuild/src/compiled/00/41/CDefinitionManager_GetDef00410820_00410820.cpp), and this TU sees its body: the shared header's
//   FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY. Retail fact: edi (def) and ebp (&def->Controls) stay live across both vector
//   operator= calls (0x004087E1..0x004087FA and 0x00408743) with no reload of [esp+0xc], so VC7.1 must know GetDef does not keep
//   &def; with a declaration-only GetDef it reloads after every call.
// Layout: engine/CUserProfileManager.h PASSIVEINPUTS + RESETASSIGNEDINPUTS (AssignedInputsAggressive +0x60); CControlsDef restated
//   on the retail def chain (engine/_quarantine only): Controls +0x3c (Ego_r +0x40 - 4).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER   // hides the non-retail CGameDefinitionManager::GetDef(long, CDefPointer<T>&) so GetDef reads unqualified
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY   // same-TU body of GetDef<T>(const CCharString&, ...) (0x00410820)
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER_ISALLOCATED   // inline static IsAllocated() (FableWin 0x01888A60)
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER_PCLASS   // agentNF31: 0x013B8760 = CStartupDefinitionManager::PClass (FableWin.pdb dynamic initializer / atexit names + Anniversary publics; overlay opt-in)
#define FABLE_RETAIL_CUSERPROFILEMANAGER_PASSIVEINPUTS
#define FABLE_RETAIL_CUSERPROFILEMANAGER_RESETASSIGNEDINPUTS
#define FABLE_RETAIL_STLP_VECTOR_C   // required: the vector<CActionInputControl>::operator= call 0x00411B90 (569v557 without it; VECTOR_ASSIGN matches too)
#include "fable_string.h"
#include "fable_game.h"
#include "engine/CUserProfileManager.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CStartupDefinitionManager.h"
#include "engine/CCountedPointer.h"

class CControlsDef : public CParentDefClassBase {                    // retail layout (Ego_r - 4)
public:
    std::vector<CActionInputControl> Controls;                       // +0x3c
};

extern bool g_bControlSchemeLeftClickIsKing;                         // 0x013B85F4 (descriptive; landed name, CThingFilterIsValidTargetForPlayerCreature 0x00808E40)
extern unsigned char g_bAggressiveModeAvailable;                     // 0x013B85F3

void CUserProfileManager::ResetAssignedInputs()
{
    if (g_bControlSchemeLeftClickIsKing) {
        CDefPointer<const CControlsDef> def;
        if (CMainGameComponent::Get()) {
            CGameDefinitionManager::Get().GetDef("FABLE_PC_CONTROL_SCHEME_GDD_LEFT_CLICK_IS_KING", &def);
            AssignedInputsPassive = def->Controls;
            AssignedInputsAggressive = def->Controls;
        } else if (CStartupDefinitionManager::IsAllocated()) {
            CStartupDefinitionManager::Get().GetDef("FABLE_PC_CONTROL_SCHEME_GDD_LEFT_CLICK_IS_KING", &def);
            AssignedInputsPassive = def->Controls;
            AssignedInputsAggressive = def->Controls;
        }
    } else if (g_bAggressiveModeAvailable) {
        CDefPointer<const CControlsDef> def;
        if (CMainGameComponent::Get()) {
            CGameDefinitionManager::Get().GetDef("FABLE_PC_CONTROL_SCHEME_GDD", &def);
            AssignedInputsPassive = def->Controls;
            CGameDefinitionManager::Get().GetDef("FABLE_PC_CONTROL_SCHEME_GDD_AGGRESSIVE", &def);
            AssignedInputsAggressive = def->Controls;
        } else if (CStartupDefinitionManager::IsAllocated()) {
            CStartupDefinitionManager::Get().GetDef("FABLE_PC_CONTROL_SCHEME_GDD", &def);
            AssignedInputsPassive = def->Controls;
            CStartupDefinitionManager::Get().GetDef("FABLE_PC_CONTROL_SCHEME_GDD_AGGRESSIVE", &def);
            AssignedInputsAggressive = def->Controls;
        }
    } else {
        CDefPointer<const CControlsDef> def;
        if (CMainGameComponent::Get()) {
            CGameDefinitionManager::Get().GetDef("FABLE_PC_CONTROL_SCHEME_GDD_ONE_MODE", &def);
            AssignedInputsPassive = def->Controls;
            AssignedInputsAggressive = def->Controls;
        } else if (CStartupDefinitionManager::IsAllocated()) {
            CStartupDefinitionManager::Get().GetDef("FABLE_PC_CONTROL_SCHEME_GDD_ONE_MODE", &def);
            AssignedInputsPassive = def->Controls;
            AssignedInputsAggressive = def->Controls;
        }
    }
    ApplyValues();
}