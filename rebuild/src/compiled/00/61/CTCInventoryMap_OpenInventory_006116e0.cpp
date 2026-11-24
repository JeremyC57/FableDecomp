// CTCInventoryMap::OpenInventory() -- opens the world-map screen: remembers the previous hero-info screen, opens the base inventory, fades the sound channels, builds the map menu and the mouse pointer and keeps both components
// Leaf: ?OpenInventory@CTCInventoryMap@@QAEXXZ
// Flags: /d2inlS12
// Inline-budget: /d2inlS12 -- only the size-optimised scaffolding COMDATs see it (the row body is /O2): it expands
//   allocator::allocate -> malloc in the _M_insert_overflow copy as the owner 0x00485DBC does (smallest N, the 0x005414A0 sweep).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 27 (family acc, DECISIONS_W26 D5): the callee FindTeleports 0x005FC620 is private in both PDBs (AAE).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006116E0, 535 bytes (+ int3 padding), speed TU (no frame pointer, `sub esp,10h`).
// Sound manager (agentSND2, 2026-11-10): PSoundManager is the shared engine/CASoundManager.h one; the local CSoundSystem model is gone and its slots carry the header names (FadeChannel -> SetGainFader slot 46).
// Callees: NHeroInformationScreens::CBase slots 2 (+0x08) / 3 (+0x0C) through the +0x130 secondary base,
//   CTCInventoryBase 0x005BC6ED (manifest "_Dest_val<...>" is propagated; named descriptively) and OpenInventory 0x005BD4EA,
//   CManager::GetInstance 0x0041E5F2 + manager slots 16 (+0x40) / 26 (+0x68) + CreateComponent 0x0041DB1D,
//   CPlayerGui::GetPHeroInfoScreensManager 0x00437CD0 ((&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) 0x013B8790), CManager::GetCurrentScreen 0x0066E7BD /
//   SelectScreenOfType 0x0066E799, the sound system's slot 46 (+0xB8) (g 0x013B8394), OnOpenMenus 0x0041D122, the input
//   controller's slot 76 (+0x130), FindTeleports 0x005FC620, the map-menu builder 0x0060EEB0 (manifest
//   "CTCInventoryMap_BuildMapMenu"), component slots 43 (+0xAC) / 48 (+0xC0), this's slots 32 (+0x80) / 46 (+0xB8),
//   vector _M_insert_overflow 0x00485DBC (same-TU stand-in body, as the landed CTCCreditsUI::Activate 0x007E4B60).
// Shape: the player-GUI global is read into a named local before the screen-type call; the failed-build arm (CloseInventory)
//   is the else of `if (screen)` so VC7.1 places it after the push_back tail as retail does.
// NF213 (2026-11-21, q6333 / q6342): the header-less CRetailVector stand-in (its same-TU explicit instantiation bound 0x00485DBC as
//   ?_M_insert_overflow@?$CRetailVector@PAVCComponent@NUISystem@@@@QAE...K_N@Z) is the STLport vector<NUISystem::CComponent*> of
//   stlport/fable_containers.h (Ego_r.pdb NInventory::CTCInventoryBase +0x0C Components). push_back's growth call binds the
//   protected IAE ?_M_insert_overflow@?$vector@PAVCComponent@NUISystem@@V?$allocator@... (the 0x00485DBC self leaf). The body
//   stays visible and out of line (OVERFLOW_OUTOFLINE, as the HEAD same-TU body); its pick-any copy is the owner's through
//   FABLE_RETAIL_STLP_VECTOR_OVERFLOW_NUISYSTEM_CCOMPONENT_SIZE_BODY with COPY_TRIVIAL_OUTOFLINE + FILL_N_FASTCALL, and push_back's
//   through FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NUISYSTEM_CCOMPONENT_SIZE_BODY (Fidelity: scaffolding, not retail; owners 0x00485DBC /
//   0x00486024). The other local models (CCharString, NUISystem::CComponent / CManager, ...) are HEAD debt, unchanged.
// NF214 (2026-11-21, q6333 / q6342, NF213 option d): the overflow body's callee fill_n<NUISystem::CComponent**> is visible and out of
//   line through FABLE_RETAIL_STLP_FILL_N_NUISYSTEM_CCOMPONENT_SIZE_BODY (optimize("s", on), as its owner row 0x0048514C sets): with it
//   declared only, the caller lost retail's register allocation (DIFFER 546v535). Its pick-any copy grades MATCH against 0x0048514C
//   (23 bytes); the overflow copy equals the owner 0x00485DBC (137 = 137, bytes and relocations, both naming the same fill_n symbol).
// NF217 (2026-11-21, q12372 / q12373): the local NHeroInformationScreens::CBase / CManager models and the local CCharString are gone:
//   the row uses the shared engine/NHeroInformationScreens_CManager.h (+ _CBase.h) and fable_string.h. 0x0066E799 now binds the PDB
//   ?SelectScreenOfType@CManager@NHeroInformationScreens@@QAEXW4EScreenType@2@@Z (was the stand-in SetCurrentScreenType(long)) and
//   0x0066E7BD the const-return ?GetCurrentScreen@CManager@NHeroInformationScreens@@QAEPBVCBase@2@XZ (was QAEPAV), both PDBs (Ego_r.pdb
//   0x006A0C9B / 0x006A0CAB, FableWin.pdb 0x02115ADC / 0x0211514F) and the reloc_bindings witness 0x00577216. The secondary base's slot 3
//   is the header's IsInventoryScreen (was the stand-in IsActive; 0x00577216 calls it there), PreviousScreenType is EScreenType (Ego_r.pdb).
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_COPY_TRIVIAL_OUTOFLINE
#define FABLE_RETAIL_STLP_FILL_N_FASTCALL
#define FABLE_RETAIL_STLP_FILL_N_NUISYSTEM_CCOMPONENT_SIZE_BODY
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_NUISYSTEM_CCOMPONENT_SIZE_BODY
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NUISYSTEM_CCOMPONENT_SIZE_BODY
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"                              // CCharString(const char*, long = -1) 0x0099EBF0, ~CCharString 0x0099EAE0
namespace NUISystem {
class CComponent { public:
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39)
    V(40) V(41) V(42)
    virtual void Reset();                               // slot 43 (+0xac)
    V(44) V(45) V(46) V(47)
#undef V
    virtual void ChangeState(long state);               // slot 48 (+0xc0)
};
class CManager { public:
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15)
    virtual void AddActiveTC(void* tc);                 // slot 16 (+0x40)
    V(17) V(18) V(19) V(20) V(21) V(22) V(23) V(24) V(25)
    virtual void SetActiveThing(void* thing);           // slot 26 (+0x68)
#undef V
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    static CManager* __fastcall GetInstance(); static void __fastcall OnOpenMenus();   // 0x0041E5F2 ?GetInstance@CManager@NUISystem@@SIPAV12@XZ; 0x0041D122 ?OnOpenMenus@CManager@NUISystem@@SIXXZ (Ego_r + FableWin)
};
}
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#include "engine/NHeroInformationScreens_CManager.h"   // CBase (GetScreenType slot 2, IsInventoryScreen slot 3), CManager
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
#include "engine/CASoundManager.h"
// NF338: 0x0041D122 is the PDB public static NUISystem::CManager::OnOpenMenus() (?OnOpenMenus@CManager@NUISystem@@SIXXZ), declared in the model above
struct CInputController {
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39)
    V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49) V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59)
    V(60) V(61) V(62) V(63) V(64) V(65) V(66) V(67) V(68) V(69) V(70) V(71) V(72) V(73) V(74) V(75)
#undef V
    virtual void SetEnabled(bool enabled);              // slot 76 (+0x130)
};
class CDisplayEngine { public: unsigned char _pad[0x2c]; CInputController* Input; };   // +0x2c
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public: unsigned char _pad[0x28]; CDisplayEngine* Display; };   // +0x28
struct CTCInventoryBase {
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31)
    virtual void CloseInventory();                      // slot 32 (+0x80)
    V(33) V(34) V(35) V(36) V(37) V(38) V(39) V(40) V(41) V(42) V(43) V(44) V(45)
    virtual void RefreshScreen(long mode);              // slot 46 (+0xb8)
#undef V
    void* ParentThing;                                  // +0x04
    unsigned char _pad08[0x0c - 0x08];
    std::vector<NUISystem::CComponent*> Components;   // +0x0c
    unsigned char _pad18[0x130 - 0x18];
    void ResetOpenState();                              // 0x005BC6ED
    void OpenInventory();                               // 0x005BD4EA
};
// Layout: retail CTCInventoryMap (landed OnCreate 0x00602E30 / DrawPreparePrimitivesForRendering 0x005FAF50): the
//   NHeroInformationScreens::CBase secondary base +0x130, AccumFadeTime +0x140, the teleports-found flag +0x234,
//   PreviousScreenType +0x244.
class CTCInventoryMap : public CTCInventoryBase, public NHeroInformationScreens::CBase { public:
    unsigned char _pad134[0x140 - 0x134];
    float         AccumFadeTime;                        // +0x140
    unsigned char _pad144[0x234 - 0x144];
    bool          TeleportsFound;                       // +0x234
    unsigned char _pad235[0x244 - 0x235];
    NHeroInformationScreens::EScreenType PreviousScreenType;   // +0x244 (Ego_r.pdb CTCInventoryMap::PreviousScreenType is EScreenType)
private:
    void FindTeleports();                               // 0x005FC620 (private: Ego_r.pdb / FableWin.pdb AAE)
public:
    NUISystem::CComponent* BuildMapMenu(long a, long b);   // 0x0060EEB0
    void OpenInventory();
};
void CTCInventoryMap::OpenInventory()
{
    PreviousScreenType = NHeroInformationScreens::NULL_SCREEN_TYPE;
    if (!IsInventoryScreen()) {
        ResetOpenState();
        AccumFadeTime = 0.0f;
        NUISystem::CManager::GetInstance()->SetActiveThing(ParentThing);
        PreviousScreenType = CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetPHeroInfoScreensManager()->GetCurrentScreen()->GetScreenType();
        RefreshScreen(0);
        TeleportsFound = true;
        FindTeleports();
    }
    if (!IsInventoryScreen()) {
        CPlayerGui* gui = (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get());
        gui->GetPHeroInfoScreensManager()->SelectScreenOfType(GetScreenType());
    }
    CTCInventoryBase::OpenInventory();
    NUISystem::CManager::GetInstance()->AddActiveTC(this);
    if (IsInventoryScreen())
        return;
    if (PSoundManager) {
        PSoundManager->SetGainFader(0x10, 0, 500.0f);
        PSoundManager->SetGainFader(0x40, 0, 500.0f);
        PSoundManager->SetGainFader(0x80, 0, 500.0f);
    }
    NUISystem::CManager::OnOpenMenus();
    CMainGameComponent::Get()->Display->Input->SetEnabled(false);
    NUISystem::CComponent* screen = BuildMapMenu(0, 0);
    if (screen) {
        NUISystem::CComponent* pointer = NUISystem::CManager::GetInstance()->CreateComponent("UI_MOUSE_POINTER", false);
        pointer->Reset();
        Components.push_back(pointer);
        screen->Reset();
        screen->ChangeState(5);
        Components.push_back(screen);
    } else {
        CloseInventory();
    }
}