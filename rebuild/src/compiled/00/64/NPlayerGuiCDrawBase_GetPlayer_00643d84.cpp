// NPlayerGui::CDrawBase::GetPlayer() const @ 0x00643D84 -- the main player of the draw object's player manager
// Leaf: ?GetPlayer@CDrawBase@NPlayerGui@@QBEABVCPlayer@@XZ
// Naming wave 31 (family dgp, c702): Ego_r.pdb / FableWin.pdb ?GetPlayer@CDrawBase@NPlayerGui@@QBEABVCPlayer@@XZ. Ego_r places the non-const / const GetPlayer pair as two
//   adjacent 8-byte bodies (0x00686C81 / 0x00686C89), retail the same pair 0x00643D7C / 0x00643D84 (identical bytes, not folded); chunk 702
//   binds this spelling (caller 0x00657200). Was the invented forwarder T::Run over Sub::Run (catalog "_global::Forward_GetMainPlayer_via0x8_00643d84",
//   Leaf "CEntry>_const&>").
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00643D84, 8 bytes: `mov ecx,[ecx+8] ; jmp 0x00449970` (CPlayerManager::GetMainPlayer, the
//   non-const one -- PlayerManager is a CPlayerManager* member, so the const member calls through a pointer to non-const).
// Layout: Ego_r.pdb NPlayerGui::CDrawBase (vptr, PlayerGui +0x04, PlayerManager +0x08, ...), as the landed c702 rows 0x00657200 / 0x00657D90.
#include "engine/CPlayerManager.h"                       // GetMainPlayer() 0x00449970 / const 0x00449960
class CPlayerGui;
class CWorld;
class CMainGameComponent;
class CGraphicDataBank;
class CEnginePrimitivePrepareInfo;

namespace NPlayerGui {
class CDrawBase {                                           // RTTI .?AVCDrawBase@NPlayerGui@@
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
}

const CPlayer& NPlayerGui::CDrawBase::GetPlayer() const
{
    return PlayerManager->GetMainPlayer();
}