// CMainGameComponent::GetPredictedTimeSinceLastRenderFrame() const -- the game time since the last render frame started
// Leaf: ?GetPredictedTimeSinceLastRenderFrame@CMainGameComponent@@IBENXZ
// Identity (naming wave 11): Ego_r.pdb 0x004154B7 + FableWin.pdb (protected const, double); Ego_r member order GetCurrentGameTime /
//   ConvertGTToWF / GetRenderInterpolate / GetPredictedTimeSinceLastRenderFrame = retail 0x00416231 / 0x0041623E / 0x00416251 / 0x00416258.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00416258, 16 bytes: push esi ; mov esi,ecx ; call 0x00416231 ; fsub [esi+0x161A8] ; pop esi ; ret.
class CMainGameComponent {
public:
    char   _pad[0x161a8];
    double LastRenderFrameStartTime;        // +0x161a8 (ForceRender 0x004164E0 stores GetCurrentGameTime() here)
protected:
    double GetCurrentGameTime() const;      // 0x00416231
    double GetPredictedTimeSinceLastRenderFrame() const;
};
double CMainGameComponent::GetPredictedTimeSinceLastRenderFrame() const
{
    return GetCurrentGameTime() - LastRenderFrameStartTime;
}
