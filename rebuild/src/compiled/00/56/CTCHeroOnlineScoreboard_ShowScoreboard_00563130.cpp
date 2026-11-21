// CTCHeroOnlineScoreboard::ShowScoreboard() -- resets the scoreboard (connection dropped, TaskState 100) and, when the hero has a GUI component, creates the two "connecting" message components, registers them with the GUI, then either reports "not signed in" or starts the state machine (SelectedClan cleared, ScoreboardState 1)
// Leaf: ?ShowScoreboard@CTCHeroOnlineScoreboard@@QAEXXZ
// Naming wave 31 (family hos): ClearScoreboard 0x00562E10 is private in both PDBs (?ClearScoreboard@CTCHeroOnlineScoreboard@@AAEXXZ, the chunk 702 own
//   row; was the public QAE spelling); the bare Leaf "ShowScoreboard" is the emitted symbol ?ShowScoreboard@CTCHeroOnlineScoreboard@@QAEXXZ (Ego_r.pdb / FableWin.pdb)
// Naming wave 18: the global stand-in g_pEngineGlobals_013b86a0 (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe @ 0x00563130, 736 bytes, `ret`. Ego_r.pdb `?ShowScoreboard@CTCHeroOnlineScoreboard@@QAEXXZ`.
// Callees: ClearScoreboard 0x00562E10, LHHttp2::~LHHttp2 0x008410D0 + operator delete 0x00BFE9BC (inline scoped_ptr
// reset), CVectorMap::LowerBound 0x0040F020 (inline CThing::GetTC for interface 0x11), CFrontEndManager::GetInstance
// 0x0041E5F2, CFrontEndManager::CreateComponent(const CCharString&, CComponent*) 0x0041DB1D, CCharString(const char*,
// long) 0x0099EBF0 / ~ 0x0099EAE0, the components' slots 43 (+0xac) and 48 (+0xc0, given a bool), the GUI component's
// slot 82 (+0x148, returns its component vector), vector<CComponent*>::_M_insert_overflow 0x00485DBC,
// CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0, CWideString(const CWideString&) 0x0099B720 / ~ 0x0099B510,
// CWideString::ToCharString 0x0099B2C0, CBasicString<char>::operator==(const char*) 0x004115A0,
// CDataBank::GetTextBySymbol 0x009C95E0 (text bank +0x14 of 0x013B86A0), DisplayErrorMessage 0x0055EF00,
// CWideString::operator=(const CWideString&) 0x0099B7D0 with the empty wide string 0x013BCA24.
// Layout: as the landed AbortWithMessage 0x00563410 / ResetScoreboard 0x00562E70: PHttp +0x0c, SelectedClan +0x3c,
// ScoreboardState +0x50, TaskState +0x54, PClanMenuEntry +0x5c, PConnectingMessage +0x60, PConnectingMessage1 +0x64.
// CUserProfileManager's signed-in user name +0x110 and password +0x114 (CWideString).
// Shape: GetSignedInUserName/GetSignedInPassword are inline by-value CCharString getters, compared to "" with the
// inline CCharString==; the `||` gives the conditional-temporary flags word.
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): 0x013BCA24 is CWideString::NullString (?NullString@CWideString@@2V1@A, public static;
//   Ego_r.pdb + FableWin.pdb `dynamic initializer for 'CWideString::NullString'', initializer 0x01223DA0 landed by agentFAM09); was g_emptyWideString.
// NF213 (2026-11-21, q6333 / q6342): ported onto the shared headers. The global stand-in `struct CComponent` (whose vector bound
//   0x00485DBC as ?_M_insert_overflow@?$vector@PAUCComponent@@@std@@QAE... and CreateComponent as ...PAUCComponent@@...) is the real
//   NUISystem::CComponent (engine/NUISystem_CComponent.h: Initialise slot 43, ChangeState(unsigned long) slot 48, the stand-in SetVisible(bool)); the GUI component at interface 0x11 is
//   the inventory (TCI_INVENTORY, NInventory::CTCInventoryBase::GetComponents slot 82); the local CCharString / CWideString / CDataBank /
//   CMainGameComponent / CUserProfileManager / CThing / CVectorMap / LHHttp2 / CTCHeroOnlineScoreboard models move onto their headers.
//   push_back's growth call binds the protected IAE ?_M_insert_overflow@?$vector@PAVCComponent@NUISystem@@V?$allocator@... (0x00485DBC
//   self leaf; declaration-only as at HEAD); push_back's pick-any COMDAT is the owner 0x00486024's through
//   FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NUISYSTEM_CCOMPONENT_SIZE_BODY (Fidelity: scaffolding, not retail).
//   Slot 48 is NUISystem::CComponent::ChangeState(unsigned long) (the stand-in SetVisible(bool); push 0 / push 1 as before);
//   CreateComponent takes the shared header's (const CCharString&, bool) spelling ?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z
//   (was ...QAEPAUCComponent@@ABVCCharString@@PAU3@@Z, this row only). PHttp.reset() deletes through the declared user destructor
//   ??1LHHttp2@LHNetworkLib@@QAE@XZ 0x008410D0 (FABLE_RETAIL_LHHTTP2_DTOR; without it the implicit destructor is a different in-object copy).
//   The GetTC lookup's CVectorMap::LowerBound is the shared header's pick-any copy under 0x0040F020's own Leaf spelling (cmpbody equal).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_NULLSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED

#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NUISYSTEM_CCOMPONENT_SIZE_BODY
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_NETWORK
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_CLEARSCOREBOARD
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_SHOWSCOREBOARD
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SCOREBOARD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_DTOR
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CDATABANK
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#include "fable_compiler.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCHeroOnlineScoreboard.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"

void CTCHeroOnlineScoreboard::ShowScoreboard()
{
    PClanMenuEntry = 0;
    ClearScoreboard();
    PHttp.reset();
    TaskState = 100;
    NInventory::CTCInventoryBase* gui;
    if (ParentThing->GetTC(TCI_INVENTORY, &gui)) {
        NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
        PConnectingMessage = manager->CreateComponent("UI_CONNECTING", false);
        PConnectingMessage->Initialise();
        PConnectingMessage->ChangeState(0);
        gui->GetComponents().push_back(PConnectingMessage);
        PConnectingMessage1 = manager->CreateComponent("UI_CONNECTING1", false);
        PConnectingMessage1->Initialise();
        gui->GetComponents().push_back(PConnectingMessage1);
        PConnectingMessage1->ChangeState(1);
        bool notSignedIn = GetSignedInUserName() == "" || GetSignedInPassword() == "";
        if (notSignedIn) {
            CWideString text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_SCOREBOARD_ERROR_LOGIN_DETAILS_MISSING");
            DisplayErrorMessage(text);
            return;
        }
        ScoreboardState = 1;
        SelectedClan = CWideString::NullString;
    }
}
