// CGameScriptInterface::IsPlayerHoldingFirstPersonTargetingButton() const -- is a game-action 0x2D event (button state 2) for the main player's joystick queued in the player's input interface
// Leaf: ?IsPlayerHoldingFirstPersonTargetingButton@CGameScriptInterface@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00890640, 38 bytes, thiscall, `ret`.
// Identity (proven): vtable 0x01260F0C (RTTI .?AVCGameScriptInterface@@) slot 68; ghidra_out/cgsi_slots_aligned.tsv (Ego_r.pdb vtable order, neighbours 64 PostAddScriptedEntities 0x006E7460 and 69 IsHeroInProjectileWeaponMode 0x00890670 landed) names the slot ?IsPlayerHoldingFirstPersonTargetingButton@CGameScriptInterface@@UBE_NXZ; the game action it queries is 0x2D.
// Callees: CPlayerManager::GetMainPlayer() const 0x00449960 (Ego_r.pdb QBE, const CPlayer&: it calls the const GetPlayer
//   0x00449910; the non-const pair is 0x00449970 / 0x004498C0, same order as the Ego_r.pdb donors), CPlayer::GetJoystickDeviceNumber() const 0x00487510,
//   CGamePlayerInterface::IsGameActionEventInQueue(EGameAction, EInputButtonEventType, long) const 0x00446B10.
// Globals: the main game component 0x013B86A0 (its PPlayerManager scoped_ptr at +0x1C, Ego_r.pdb CMainGameComponent; the
//   global's spelling g_pMainGameComponent is descriptive -- no PDB symbol names it).
// Body: CMainGameComponent::GetPlayerManager() and the scoped_ptr dereference inline (`mov ecx,[eax+1Ch]`); the player's
//   PlayerInterface (+0xC) is read before the device-number call.
enum EGameAction { EGameAction_FIRST };
enum EInputButtonEventType { EInputButtonEventType_FIRST };
class CGamePlayerInterface {
public:
    bool IsGameActionEventInQueue(EGameAction action, EInputButtonEventType type, long device) const;   // 0x00446B10
};
class CPlayer {
public:
    unsigned char         _pad00[0x0C];
    CGamePlayerInterface* PlayerInterface;                 // +0x0C
    long GetJoystickDeviceNumber() const;                  // 0x00487510
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                              // 0x00449970
    const CPlayer& GetMainPlayer() const;                  // 0x00449960
};
template <class T> class scoped_ptr {
public:
    T* ptr;
    T& operator*() const { return *ptr; }
};
class CMainGameComponent {
public:
    unsigned char              _pad00[0x1C];
    scoped_ptr<CPlayerManager> PPlayerManager;             // +0x1C
    const CPlayerManager& GetPlayerManager() const { return *PPlayerManager; }   // inline (spelling unproven, no symbol)
};
extern CMainGameComponent* g_pMainGameComponent;           // 0x013B86A0
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool CGameScriptInterface::IsPlayerHoldingFirstPersonTargetingButton() const
{
    const CPlayer& player = g_pMainGameComponent->GetPlayerManager().GetMainPlayer();
    CGamePlayerInterface* playerInterface = player.PlayerInterface;
    return playerInterface->IsGameActionEventInQueue((EGameAction)0x2D, (EInputButtonEventType)2, player.GetJoystickDeviceNumber());
}