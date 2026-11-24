// Leaf: ?Rest@CGameScriptInterface@@IBEXM@Z
// CGameScriptInterface::Rest(float seconds) -- block the calling script for the given time: compute the end frame (now + frames-per-second * seconds) and, while the game frame is still before it and the script runner has not been told to stop, yield one script frame
// Naming wave 18: the global stand-in g_FramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E71F0, length 128 bytes. `ret 4`.
// Callees: the global frame-counter getter @ 0x0049D870 (`mov eax,[0x013B89BC] ; ret`; catalog "GFGetPreMainMemoryUsed" is wrong), the
// script runner's virtual slot 2 (+8, yield one frame; as the landed NewScriptFrame 0x006E7410, which tests the same stop byte +5).
// Globals: the script runner @ 0x013D2838, the frames-per-second count @ 0x01375550.
// Shape: the end frame is a float homed in the dead `seconds` slot; the frame rate is read before the clock call; the loop is a
// `while` whose test is duplicated at the bottom (`fild ; fcomp ; test ah,5 ; jp/jnp`), the runner global re-read every pass.
// NF52 (round 7): the running script process at 0x013D2838 is the established ?g_pScriptRunner@@3PAVCSuspendableProcessMicrothread@@A
// (descriptive, the spelling of the landed 0x00A44660 InternalSwitchToProcess / 0x00A44880 RunSuspendableProcess that store it) over
// engine/CSuspendableProcessMicrothread.h: StopRunning +0x05 (CASuspendableProcess) and the slot-2 virtual SuspendSuspendableProcess
// (the frame yield). Was the local CScriptRunner stand-in (m_bStopRequested / YieldFrame).
#include "engine/CSuspendableProcessMicrothread.h"
extern CSuspendableProcessMicrothread* g_pScriptRunner;   // 0x013D2838
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                        // 0x0049D870

class CGameScriptInterface { public:
    protected:
    void Rest(float seconds) const;
    public:
};

void CGameScriptInterface::Rest(float seconds) const
{
    if (g_pScriptRunner) {
        long fps = CMainGameComponent::GetConstantFPS();
        float endFrame = (float)CWorld::GetFrame() + (float)fps * seconds;
        while ((float)CWorld::GetFrame() < endFrame) {
            if (g_pScriptRunner->StopRunning)
                break;
            g_pScriptRunner->SuspendSuspendableProcess();
        }
    }
}