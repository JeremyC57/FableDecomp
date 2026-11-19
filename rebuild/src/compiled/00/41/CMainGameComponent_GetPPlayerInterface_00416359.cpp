// CMainGameComponent::GetPPlayerInterface() -- the game's player interface (the scoped_ptr<CGamePlayerInterface> at +0x20)
// Leaf: ?GetPPlayerInterface@CMainGameComponent@@QAEPAVCGamePlayerInterface@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00416359: mov eax,[ecx+20h] ; ret.
// Name fix (agentNF94, q11265): Ego_r.pdb 0x0041560F + FableWin.pdb 0x018A6463 ?GetPPlayerInterface@CMainGameComponent@@QAEPAVCGamePlayerInterface@@XZ
//   (engine/CMainGameComponent.h GETPPLAYERINTERFACE declares it; the landed 0x0044A1A0 / 0x00805600 bind it so). The old row was the generic
//   `T::Get` member-getter shape on a local pack(1) struct; it is now on the shared header, PPlayerInterface typed as its Ego_r.pdb
//   boost::scoped_ptr<CGamePlayerInterface> (overlay opt-in FABLE_RETAIL_CMAINGAMECOMPONENT_PPLAYERINTERFACE).
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPPLAYERINTERFACE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PPLAYERINTERFACE
#include "engine/CMainGameComponent.h"
CGamePlayerInterface* CMainGameComponent::GetPPlayerInterface()
{
    return PPlayerInterface.get();
}
