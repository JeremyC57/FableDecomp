// Grade: DIFFER(33v33) -- x87 stack cleanup placement only: retail pops the dead game-time operand right after the divide (`fdivr st1 ; fstp st1`), the lane keeps it until after __ftol2 (`fstp st0` before `ret 4`). Tried: named double result, __forceinline ConvertGTToWF, optimize("s"), const double& parameter, in-place `/=` on the parameter (6 grades). Next: ConvertGTToWF as a member reading the time itself, or a float temporary.
// CMainGameComponent::GetFrameDifferenceFromCurrent(long frame) const -- world frames elapsed since `frame`
// Retail 0x004167A7, 33 bytes (Ego_r.pdb ?GetFrameDifferenceFromCurrent@CMainGameComponent@@IBEJJ@Z).
// Body: the current game time (GetCurrentGameTime 0x00416231, a double: GFGetTime() minus the
// game start time) converted to world frames by the inlined ConvertGTToWF -- gt / (1.0f / WFPS)
// with WFPS the global frame rate 0x01375550 (`fild ; fdivr [1.0f] ; fdivr st1`; the out-of-line
// copy of ConvertGTToWF sits at 0x0041623E) -- minus the frame argument (`fisub`), truncated to
// long by __ftol2 (0x00BFEA70, emitted by the cast). `this` stays in ecx for the first call.
// Leaf: ?GetFrameDifferenceFromCurrent@CMainGameComponent

extern long g_framesPerSecond;          // 0x01375550 world frames per second

class CMainGameComponent
{
public:
    long GetFrameDifferenceFromCurrent(long frame) const;
    double GetCurrentGameTime() const;  // 0x00416231
    double ConvertGTToWF(double gameTime) const
    {
        return gameTime / (1.0f / g_framesPerSecond);
    }
};

long CMainGameComponent::GetFrameDifferenceFromCurrent(long frame) const
{
    return (long)(ConvertGTToWF(GetCurrentGameTime()) - frame);
}
