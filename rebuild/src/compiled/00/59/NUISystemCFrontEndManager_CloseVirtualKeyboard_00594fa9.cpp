#pragma optimize("s", on)
// NUISystem::CFrontEndManager::CloseVirtualKeyboard(bool goBack) -- delete the virtual keyboard at +0x60 (its destructor, operator delete, the pointer cleared), then when asked go to the previous screen (0)
// Leaf: ?CloseVirtualKeyboard@CFrontEndManager@NUISystem@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00594FA9, length 50 bytes.
// Callees: CVirtualKeyboard::~CVirtualKeyboard @ 0x00851720, operator delete @ 0x00BFE9BC, GotoPreviousScreen @ 0x00597BF2 (on `this`, 0; the manifest label
// "CFrontEndManager::GotoPreviousScreen"). Shape: the size-optimised module (`and dword ptr [esi+60h],0` for the clear, `pop ecx` after the delete), `cmp byte
// ptr [esp+0Ch],0` for the argument. The chunk label is propagated; the class is named by address.
// agentNF57 (q10440): Ego_r.pdb 0x00601881 / FableWin.pdb 0x02011FA2 ?CloseVirtualKeyboard@CFrontEndManager@NUISystem@@QAEX_N@Z, the
//   spelling the landed destructor 0x005954AD binds (engine/CFrontEndManager.h declares it); the body deletes the virtual keyboard
//   (m_pVirtualKeyboard +0x60) and optionally goes back (GotoPreviousScreen(bool) 0x00597BF2, new overlay opt-in
//   FABLE_RETAIL_CFRONTENDMANAGER_GOTOPREVIOUSSCREEN). Was the global ?DestroyVirtualKeyboard@CFrontEndManager@@QAEX_N@Z over a local
//   CFrontEndManager / CVirtualKeyboard model; the EnableInput(bool) spelling of 0x00599E3F is rejected (REJECTED.tsv).
#define FABLE_RETAIL_CFRONTENDMANAGER_DESTRUCTOR
#define FABLE_RETAIL_COBSERVER_DESTRUCTOR
#define FABLE_RETAIL_CFRONTENDMANAGER_GOTOPREVIOUSSCREEN
#define FABLE_RETAIL_CVIRTUALKEYBOARD
#include "engine/CFrontEndManager.h"
#include "engine/CVirtualKeyboard.h"

void NUISystem::CFrontEndManager::CloseVirtualKeyboard(bool goBack)
{
    if (m_pVirtualKeyboard) {
        delete m_pVirtualKeyboard;
        m_pVirtualKeyboard = 0;
    }
    if (goBack)
        GotoPreviousScreen(false);
}
