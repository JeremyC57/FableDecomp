// CGame::Uninitialise() -- chains to the protected CInitBaseClass::Uninitialise (VC7.1 tail-calls it: `jmp 0x0099A360`)
// Leaf: ?Uninitialise@CGame@@QAEXXZ
// Identity: Ego_r.pdb ?Uninitialise@CGame@@QAEXXZ (0x0040F510, then SetGameComponent 0x0040F520 / SetQuit 0x0040F530) and
//   FableWin.pdb (0x01878C30); retail 0x00412F60 is followed by SetGameComponent 0x00412F70 (`mov [ecx+8],arg`: CurrentGameComponent)
//   and SetQuit 0x00412F80 (`mov [ecx+20Ch],arg`: Quit), the CGame layout of engine/CGame.h. Its landed caller 0x00401B80 binds
//   ?Uninitialise@CGame@@QAEXXZ. NF132 (q4019): was the free extern "C" forwarder `_Forward` -> `_ForwardTarget_0099a360`.
#define FABLE_RETAIL_CGAME_CORE_PASS23
#define FABLE_RETAIL_CGAME_UNINITIALISE
#define FABLE_RETAIL_CINITBASECLASS_UNINITIALISE
#include "engine/CGame.h"

void CGame::Uninitialise()
{
    CInitBaseClass::Uninitialise();
}
