// CNewFrontendGameComponent::ResetAttractModeTimer() -- restarts the attract-mode stopwatch (CStopWatch::Reset on the member at +0x100)
// Retail: Fable.exe @ 0x0042DBEE, 11 bytes: `add ecx,100h ; jmp 0x0062F8B0` (catalog label "_Cons_val<...>" is wrong).
// Evidence: both callers (CFrontEndManager::GotoPreviousScreen 0x00597BF2 and CFrontEndManager::Action 0x0059A238)
// call it on the front-end game component global 0x013B871C after user input; callee 0x0062F8B0 is
// CStopWatch::Reset. Ego_r.pdb CNewFrontendGameComponent::m_attractModeTimer (CStopWatch) is at +0x108; retail
// is 8 lower (FileNames / FileTimes are 12-byte STLport vectors), +0x100.
// Pragma: frontend size TU (`optimize("s", on)` + `("y", off)`, as landed 0x0042DDB3); a void forwarder tail-jumps.
#pragma optimize("s", on)
#pragma optimize("y", off)
struct CStopWatch {
    void Reset();                                        // 0x0062F8B0
};
struct CNewFrontendGameComponent {
    unsigned char _pad00[0x100];
    CStopWatch    m_attractModeTimer;                    // +0x100
    void ResetAttractModeTimer();
};

void CNewFrontendGameComponent::ResetAttractModeTimer()
{
    m_attractModeTimer.Reset();
}