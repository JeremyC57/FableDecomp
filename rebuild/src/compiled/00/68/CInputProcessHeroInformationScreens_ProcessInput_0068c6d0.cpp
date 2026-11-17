// CInputProcessHeroInformationScreens::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- the override only queries
//   IsEventButtonPressed(event) and drops the answer (the handling compiled out of the retail build)
// Leaf: ?ProcessInput@CInputProcessHeroInformationScreens@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068C6D0, 13 bytes: `mov eax,[esp+4] ; push eax ; call 0x00687A90 ; ret 8`.
// NF64 (round 7, q10652): re-authored on engine/CInputProcessHeroInformationScreens.h (overlay opt-in
//   FABLE_RETAIL_CINPUTPROCESSHEROINFORMATIONSCREENS_PROCESSINPUT: the Ego_r.pdb 0x006B83C0 / FableWin.pdb 0x021E12B0 virtual override) with
//   the protected const CInputProcessGameBase::IsEventButtonPressed (engine/CInputProcessGameBase.h ZTARGET; Ego_r.pdb 0x006B2BA0 /
//   FableWin.pdb 0x021D3D20). Was the extern "C" stdcall stand-in `CInputProcessHeroInformationScreens_ProcessInput` calling a made-up
//   `Helper_68c6d0` (bound _Helper_68c6d0@4 at 0x00687A90).
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_CINPUTPROCESSHEROINFORMATIONSCREENS_PROCESSINPUT
#include "engine/EGameAction.h"
class CInputEvent;
#include "engine/CInputProcessHeroInformationScreens.h"

void CInputProcessHeroInformationScreens::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    IsEventButtonPressed(event);
}
