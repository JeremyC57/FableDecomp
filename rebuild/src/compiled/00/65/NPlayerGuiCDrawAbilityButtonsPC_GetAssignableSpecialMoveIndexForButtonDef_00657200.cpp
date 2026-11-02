// NPlayerGui::CDrawAbilityButtonsPC::GetAssignableSpecialMoveIndexForButtonDef(const CAbilityButtonGuiDef& def) const -- the special-move slot the main player's interface assigns to this button def's mouse button
// Leaf: ?GetAssignableSpecialMoveIndexForButtonDef@CDrawAbilityButtonsPC@NPlayerGui@@MBEJABVCAbilityButtonGuiDef@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00657200, 26 bytes, thiscall, tail `jmp` with the argument slot rewritten (long result).
// Identity (proven): slot 3 of the NPlayerGui::CDrawAbilityButtonsPC vtable 0x0125A7D8 (rtti-vtables.tsv, the only reference);
//   Ego_r.pdb / FableWin.pdb ?GetAssignableSpecialMoveIndexForButtonDef@CDrawAbilityButtonsPC@NPlayerGui@@MBEJABVCAbilityButtonGuiDef@@@Z
//   (protected virtual const); donor order Draw 0x006993D0 (0x10) / this 0x006993E0 (0x20) / DrawAbilityList 0x00699400 is retail's
//   slots 2 / 3 / 4 at 0x006571F0 / 0x00657200 / 0x00657220 (same 0x10 / 0x20 spacing); the base override (slot 3 of
//   CDrawAbilityButtons 0x0125A6C8, 0x00643FC8) reads ControllerButton where this PC override reads MouseButton. The catalog
//   label "vfunc_3" is a placeholder.
// Callees: NPlayerGui::CDrawBase::GetPlayer() const 0x00643D84 (`mov ecx,[ecx+8] ; jmp CPlayerManager::GetMainPlayer`; Ego_r
//   places ?GetPlayer@CDrawBase@NPlayerGui@@QAEAAVCPlayer@@XZ / ...QBEABVCPlayer@@XZ as two adjacent 8-byte bodies 0x00686C81 /
//   0x00686C89, retail 0x00643D7C / 0x00643D84; out of line here because it lives in the CDrawBase TU),
//   CGamePlayerInterface::GetAssignableSpecialMoveIndexForMouseButton(EMouseButtonControl) const 0x00445F40 (Ego_r QBEJW4EMouseButtonControl@@@Z).
// Layout: CDrawBase PlayerGui +0x04, PlayerManager +0x08 (Ego_r.pdb, as the landed 0x00643FC8); CPlayer::PlayerInterface +0x0C
//   (engine/CPlayer.h); CAbilityButtonGuiDef = retail CParentDefClassBase (0x3C, engine/CParentDefClassBase.h retail opt-in) +
//   ControllerType +0x3C, ControllerButton +0x40, MouseButton +0x44 ... (Ego_r +64/+68/+72 four lower, as the landed copy ctor
//   0x0046F1B4 copies eight dwords +0x3C..+0x58).
// Shape: the interface goes into a NAMED local first (GetPlayer() before the argument read; the one-expression chain pushes
//   the argument before the call, 24 bytes), then the def's MouseButton rewrites the argument slot for the tail jump.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#include "engine/CPlayer.h"

class CPlayerGui;
class CPlayerManager;
class CWorld;
class CMainGameComponent;
class CGraphicDataBank;
class CEnginePrimitivePrepareInfo;

enum EControllerType { };
enum EXboxControllerButton { };
enum EMouseButtonControl { };

class C2DVector {
public:
    float X;
    float Y;
};

class CAbilityButtonGuiDef : public CParentDefClassBase {   // vtable 0x0123610C, retail 0x5C bytes
public:
    EControllerType       ControllerType;                   // +0x3C
    EXboxControllerButton ControllerButton;                 // +0x40
    EMouseButtonControl   MouseButton;                      // +0x44
    long                  ButtonGraphic;                    // +0x48
    C2DVector             Position;                         // +0x4C
    C2DVector             Offset;                           // +0x54
};

class CGamePlayerInterface {                                // engine/CGamePlayerInterface.h is a flat data layout
public:
    long GetAssignableSpecialMoveIndexForMouseButton(EMouseButtonControl button) const;   // 0x00445F40
};

namespace NPlayerGui {
class CDrawBase {
public:
    virtual void ClearPrimitivesIfNecessary();              // slot 0
    virtual void Draw(const CEnginePrimitivePrepareInfo& info);
    CPlayer&       GetPlayer();                             // 0x00643D7C
    const CPlayer& GetPlayer() const;                       // 0x00643D84
    CPlayerGui*         PlayerGui;                          // +0x04
    CPlayerManager*     PlayerManager;                      // +0x08
    CWorld*             World;                              // +0x0C
    CMainGameComponent* Component;                          // +0x10
    CGraphicDataBank*   Bank;                               // +0x14
    bool                Drawing;                            // +0x18
};

class CDrawAbilityButtons : public CDrawBase {              // vtable 0x0125A6C8
protected:
    virtual long GetAssignableSpecialMoveIndexForButtonDef(const CAbilityButtonGuiDef& def) const;   // slot 3, 0x00643FC8
};

class CDrawAbilityButtonsPC : public CDrawAbilityButtons {  // vtable 0x0125A7D8
protected:
    virtual long GetAssignableSpecialMoveIndexForButtonDef(const CAbilityButtonGuiDef& def) const;
};

long CDrawAbilityButtonsPC::GetAssignableSpecialMoveIndexForButtonDef(const CAbilityButtonGuiDef& def) const
{
    CGamePlayerInterface* playerInterface = GetPlayer().PlayerInterface;
    return playerInterface->GetAssignableSpecialMoveIndexForMouseButton(def.MouseButton);
}
} // namespace NPlayerGui