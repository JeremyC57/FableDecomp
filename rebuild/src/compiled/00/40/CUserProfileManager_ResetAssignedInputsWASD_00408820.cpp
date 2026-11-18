// CUserProfileManager::ResetAssignedInputsWASD() -- reset the passive and aggressive input assignments to the PC WASD control scheme
//   (the no-caps-lock variant when that option is set), or to the default schemes when aggressive mode is available, then apply the values
// Leaf: ?ResetAssignedInputsWASD@CUserProfileManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00408820, 185 bytes, thiscall, plain `ret` (two epilogue copies).
// Identity: Ego_r.pdb ?ResetAssignedInputsWASD@CUserProfileManager@@QAEXXZ (directly after ResetAssignedInputs = retail 0x004085F0)
//   and Ego_d.pdb user_profile_manager.obj (same order: ResetAssignedInputs, ResetAssignedInputsWASD); landed caller
//   NUISystem::CRedefinerList::ResetKeysToDefaults 0x005565C0 declares the same leaf.
// Locals (Ego_d.pdb): one block holding `CDefPointer<const CControlsDef> pdef` then `CCharString controlDefName` -- retail destroys
//   controlDefName (0x004088B2) before releasing pdef (0x004088BB) and only then calls ApplyValues (0x004088CE).
// Callees: CUserProfileManager::ResetAssignedInputs 0x004085F0, CCharString(const char*, long = -1) 0x0099EBF0, ~CCharString
//   0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0 / CStartupDefinitionManager::Get 0x0043368D, CDefinitionManager::GetDef
//   <CControlsDef>(const CCharString&, CDefPointer<const CControlsDef>*) const 0x00410820 (same-TU body, as the landed twin),
//   vector<CActionInputControl>::operator= 0x00411B90, the def's Delete (slot 1), CUserProfileManager::ApplyValues 0x00407370.
// Data: g_bAggressiveModeAvailable 0x013B85F3 (bound), 0x013B864F g_KeyRedefinerFlag (the disable-caps-lock option; DESCRIPTIVE,
//   the spelling of the landed NUISystem::CKeyRedefiner::ProcessEvent 0x00557EB0; no PDB data name), CMainGameComponent
//   0x013B86A0 / CStartupDefinitionManager::PClass 0x013B8760 through the inline Get / IsAllocated.
// Shape: the landed twin ResetAssignedInputs (0x004085F0) with one scheme name chosen by the caps-lock option: VC7.1 merges the two
//   identical manager arms into one GetDef call + one copy tail (0x00408892).
// Layout: engine/CUserProfileManager.h PASSIVEINPUTS + RESETASSIGNEDINPUTS (+ overlay RESETASSIGNEDINPUTSWASD, declaration only);
//   CControlsDef restated on the retail def chain as in the landed twin: Controls +0x3c (Ego_r +0x40 - 4).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER   // hides the non-retail CGameDefinitionManager::GetDef(long, CDefPointer<T>&)
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY   // same-TU body of GetDef<T>(const CCharString&, ...) (0x00410820)
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER_ISALLOCATED
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER_PCLASS
#define FABLE_RETAIL_CUSERPROFILEMANAGER_PASSIVEINPUTS
#define FABLE_RETAIL_CUSERPROFILEMANAGER_RESETASSIGNEDINPUTS
#define FABLE_RETAIL_CUSERPROFILEMANAGER_RESETASSIGNEDINPUTSWASD   // e01 overlay: declares ResetAssignedInputsWASD
#define FABLE_RETAIL_STLP_VECTOR_C   // the vector<CActionInputControl>::operator= call 0x00411B90
#include "fable_string.h"
#include "fable_game.h"
#include "engine/CUserProfileManager.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CStartupDefinitionManager.h"
#include "engine/CCountedPointer.h"

class CControlsDef : public CParentDefClassBase {                    // retail layout (Ego_r - 4), as the landed twin 0x004085F0
public:
    std::vector<CActionInputControl> Controls;                       // +0x3c
};

extern unsigned char g_bAggressiveModeAvailable;                     // 0x013B85F3
extern unsigned char g_KeyRedefinerFlag;                             // 0x013B864F (descriptive)

void CUserProfileManager::ResetAssignedInputsWASD()
{
    if (g_bAggressiveModeAvailable) {
        ResetAssignedInputs();
        return;
    }
    {
        CDefPointer<const CControlsDef> pdef;
        CCharString controlDefName(g_KeyRedefinerFlag ? "FABLE_PC_CONTROL_SCHEME_GDD_WASD_NO_CAPSLOCK" : "FABLE_PC_CONTROL_SCHEME_GDD_WASD");
        if (CMainGameComponent::Get()) {
            CGameDefinitionManager::Get().GetDef(controlDefName, &pdef);
            AssignedInputsPassive = pdef->Controls;
            AssignedInputsAggressive = pdef->Controls;
        } else if (CStartupDefinitionManager::IsAllocated()) {
            CStartupDefinitionManager::Get().GetDef(controlDefName, &pdef);
            AssignedInputsPassive = pdef->Controls;
            AssignedInputsAggressive = pdef->Controls;
        }
    }
    ApplyValues();
}