// CMainGameComponent::UpdateAverageFrameDuration(double) -- feeds the last frame's duration to the frame-rate smoother
// Leaf: UpdateAverageFrameDuration@CMainGameComponent
// Retail: Fable.exe @ 0x00416202, 23 bytes, `ret 8`. Catalog label CEngineSubPrimitiveBase::operator_new is wrong: the body
// calls CFrameRateSmoother::AddFrameTime(double) 0x0049B9E0 (Ego_r.pdb ?AddFrameTime@CFrameRateSmoother@@QAEXN@Z) on
// this+0x16178, which is CMainGameComponent::FrameRateSmoother in Ego_r.pdb (no engine header). Only caller is
// CMainGameComponent::Run 0x00418B30. Ego_r.pdb ?UpdateAverageFrameDuration@CMainGameComponent@@IAEXN@Z is the one
// CMainGameComponent member taking a double.
// Shape: size TU (`push ecx ; push ecx` for the double instead of `sub esp,8`).
#pragma optimize("s", on)
class CFrameRateSmoother {
public:
    void AddFrameTime(double time);                      // 0x0049B9E0
};
class CMainGameComponent {
public:
    unsigned char      _pad00[0x16178];
    CFrameRateSmoother FrameRateSmoother;                // +0x16178
    void UpdateAverageFrameDuration(double time);
};
void CMainGameComponent::UpdateAverageFrameDuration(double time)
{
    FrameRateSmoother.AddFrameTime(time);
}