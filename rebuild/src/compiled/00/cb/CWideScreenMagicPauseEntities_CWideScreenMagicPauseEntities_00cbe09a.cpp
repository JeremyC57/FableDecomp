// CWideScreenMagicPauseEntities::CWideScreenMagicPauseEntities(CGameScriptInterfaceBase* game) -- scope guard for the wide-screen magic cut: keep the game interface and pause every non-scripted entity (GameCalls slot 379 with true); the landed destructor 0x00CBE0B3 un-pauses
// Leaf: ??0CWideScreenMagicPauseEntities@@QAE@PAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBE09A, 25 bytes, thiscall, `ret 4`.
// Layout: Ego_r.pdb CWideScreenMagicPauseEntities (4 bytes): CGameScriptInterfaceBase* Game +0x00. No vtable.
// Callees: Game's virtual slot 379 (+0x5EC) PauseAllNonScriptedEntities(bool) -- the name the landed
//   CDF_ShineLight::Main 0x00E6EA30 gives the same GameCalls slot; the destructor calls it with false.
// Shape: init-list store of Game, then the virtual call through the just-stored pointer (still in ecx).
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)

// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"

class CWideScreenMagicPauseEntities {
public:
    CWideScreenMagicPauseEntities(CGameScriptInterfaceBase* game);
    CGameScriptInterfaceBase* Game;         // +0x00
};

CWideScreenMagicPauseEntities::CWideScreenMagicPauseEntities(CGameScriptInterfaceBase* game)
    : Game(game)
{
    Game->PauseAllNonScriptedEntities(true);
}