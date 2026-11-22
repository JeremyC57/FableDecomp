// CFrontendGameComponent::Run(CGameComponent** nextComponent) -- pump the front end until it quits, then hand over to the new front end or to a main game built from the chosen world and save (setting the hero's joystick), or shut down with no successor
// Leaf: ?Run@CFrontendGameComponent@@UAE_NPAPAVCGameComponent@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00496CF0, 411 bytes, `ret 4`.
// Callees: the engine system accessor 0x009A4EC0 and its Update 0x009A6460, CFrontendGameComponent::Input 0x004963A0 /
//   Output 0x00494900 / shutdown 0x00495780, operator new 0x00BFEA1A, CNewFrontendGameComponent(CGame&, const Init&) 0x0042EA8F
//   and CMainGameComponent(CGame&, const Init&) 0x00418DCA (incremental-link thunks), CWideString() 0x0099AED0 / operator=
//   0x0099B7D0 / dtor 0x0099B510, CCharString() 0x0099E4B0 / dtor 0x0099EAE0, CGameJoystickManager::Get 0x0049BB90 (static
//   fastcall) / SetMainPlayerJoystickDeviceNumber 0x0049BB30; the new component's virtual slot 1. Global 0x013B7D58.
// Layout: PDB CFrontendGameComponent with retail offsets (Game +0xc, Quit +8, Running +9, CreateGame +0x10, joystick +0x14,
//   CreateReleaseFrontEnd +0x19, SaveToLoad +0x1e2c, WorldToLoad +0x1e40; the network arrays are smaller in retail).
// Naming (NF234, 2026-11-22, q12650): Shutdown 0x00495780 is private in Ego_r.pdb 0x0049E380 + FableWin.pdb 0x01A41790
//   (?Shutdown@CFrontendGameComponent@@AAEXXZ); respelled in place in the local model with its self row.
// Header port (NF235, 2026-11-22, q12656): the self leaf is the PDB one, ?Run@CFrontendGameComponent@@UAE_NPAPAVCGameComponent@@@Z
//   (Ego_r.pdb + FableWin.pdb + Ego_d.pdb: public virtual, CGameComponent**; retail RTTI vtable 0x01238768 slot 2), which the vtable
//   emitters 0x00495620 / 0x00496070 bind; was the non-virtual ?Run@..@@QAE_NAAPAVCGameComponent@@@Z of a local model. The local
//   CWideString / CCharString / CSystemManager / CGameJoystickManager / CGameComponent / CNewFrontendGameComponent / CMainGameComponentInit /
//   CMainGameComponent / CFrontendGameComponent models replaced by the shared headers (fable_string.h, engine/CGameJoystickManager.h MAINPLAYERJOYSTICK, engine/CNewFrontendGameComponent.h CTOR_DTOR (the ctor), engine/CMainGameComponentInit.h
//   INLINE_CTOR_DTOR (expanded here), engine/CMainGameComponent.h, engine/CFrontendGameComponent.h CTOR_DTOR). Input / Output bind their
//   PDB spellings ?Input@ / ?Output@CFrontendGameComponent@@AAEXXZ (private, all three PDBs; the self leaves of 0x004963A0 / 0x00494900).
//   The global 0x013B7D58 is Ego_r.pdb / FableWin.pdb / Ego_d.pdb ?GOldGameComponentToDelete@@3PAVCGameComponent@@A (Ego_r publics:
//   0003:477344, 4 past ?m_strProfileName@CUserProfileManager@@0VCWideString@@A 0003:477340, as retail 0x013B7D58 sits 4 past
//   m_strProfileName 0x013B7D54); was the descriptive ?g_pLastFrontend@@3PAUCFrontendGameComponent@@A.
//   CNewFrontendGameComponentInit (no shared header; Ego_r.pdb one-long struct) stays the HEAD local shape.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CSTOPWATCH_CTOR
#define FABLE_RETAIL_CGAMEJOYSTICKMANAGER_MAINPLAYERJOYSTICK
#define FABLE_RETAIL_CMAINGAMECOMPONENTINIT_INLINE_CTOR_DTOR
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_FILENAMES_FILETIMES
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_CTOR_DTOR
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_SETQUIT
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_CTOR_DTOR
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CTexture.h"
// Naming (NF129, 2026-11-20, q4289): CSystemManager::Update returns ESystemReturn (Ego_r.pdb 0x00AF4520 + FableWin.pdb 0x02F8AB00
//   ?Update@CSystemManager@@QAE?AW4ESystemReturn@@XZ; GFPlayFullScreenXBoxMovie 0x006286F0 binds it); respelled in place in the local
//   CSystemManager model, the enum from engine/CSystemManager.h's partial mode FABLE_RETAIL_CSYSTEMMANAGER_ESYSTEMRETURN_ONLY (the full
//   header needs the Win32 types; the HEAD local model stays, in place).
#define FABLE_RETAIL_CSYSTEMMANAGER_ESYSTEMRETURN_ONLY
#include "engine/CSystemManager.h"
class CSystemManager { public: ESystemReturn Update(); };                 // 0x009A6460
CSystemManager* __fastcall GFGetSystemManager();                        // 0x009A4EC0
#include "engine/CGameJoystickManager.h"
#include "engine/CMainGameComponentInit.h"
#include "engine/CGameComponent.h"
#include "engine/CNewFrontendGameComponent.h"
#include "engine/CMainGameComponent.h"
#include "engine/CFrontendGameComponent.h"

class CNewFrontendGameComponentInit { public: long Nothing; CNewFrontendGameComponentInit() : Nothing(0) { } };
extern CGameComponent* GOldGameComponentToDelete;        // 0x013B7D58

bool CFrontendGameComponent::Run(CGameComponent** nextComponent)
{
    Running = true;
    while (!Quit) {
        long state = GFGetSystemManager()->Update();
        if (state == 2)
            Quit = true;
        else if (state == 1) {
            Input();
            Output();
        }
    }
    Running = false;
    if (CreateReleaseFrontEnd) {
        Shutdown();
        CNewFrontendGameComponentInit init;
        CGameComponent* component = new CNewFrontendGameComponent(Game, init);
        component->Init();
        *nextComponent = component;
        GOldGameComponentToDelete = this;
        return true;
    }
    if (CreateGame) {
        Shutdown();
        CMainGameComponentInit init;
        init.InitialWorldName = WorldToLoad;
        init.SaveGameName = SaveToLoad;
        CGameJoystickManager::Get().SetMainPlayerJoystickDeviceNumber(CreateGameJoystickDeviceNumber);
        CGameComponent* component = new CMainGameComponent(Game, init);
        component->Init();
        *nextComponent = component;
        GOldGameComponentToDelete = this;
        return true;
    }
    Shutdown();
    GOldGameComponentToDelete = this;
    return false;
}
