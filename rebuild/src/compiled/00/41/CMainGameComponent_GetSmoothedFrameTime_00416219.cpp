// CMainGameComponent::GetSmoothedFrameTime() const -- forwards to the frame-rate smoother's CFrameRateSmoother::GetSmoothedTime (member at +0x16178)
// Retail: Fable.exe @ 0x00416219, 12 bytes: `add ecx,16178h ; call 0x0049B990 ; ret` (catalog label "_Cons_val<...>" is wrong).
// Evidence: the only caller 0x004167CD (CMainGameComponent::GetCurrentFrameFinishTimeApproximation) calls it on
// its own `this` and adds the double at +0x58; PDB CMainGameComponent::FrameRateSmoother (CFrameRateSmoother) is at
// 90488 = +0x16178; the callee 0x0049B990 is `fld qword ptr [ecx+28h] ; ret`, CFrameRateSmoother::SmoothedTime
// (PDB +0x30, retail +0x28 with the 12-byte STLport CArray).
// Shape: CMainGameComponent TU (`optimize("y", off)` + `("s", on)`); the double result goes through a named local,
// which gives `call ; ret` instead of a tail jump.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CFrameRateSmoother {
    double GetSmoothedTime() const;                      // 0x0049B990
};
struct CMainGameComponent {
    unsigned char      _pad00[0x16178];
    CFrameRateSmoother FrameRateSmoother;                // +0x16178
    double GetSmoothedFrameTime() const;
};

double CMainGameComponent::GetSmoothedFrameTime() const
{
    double t = FrameRateSmoother.GetSmoothedTime();
    return t;
}