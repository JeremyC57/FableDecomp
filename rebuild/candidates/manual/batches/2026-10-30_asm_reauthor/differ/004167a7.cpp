// CMainGameComponent::GetFrameDifferenceFromCurrent(long frame) const -- world frames elapsed between `frame` and now
// Grade: DIFFER(33v33) -- x87 dead-operand pop placement: retail `fstp st(1)` right after `fdivr st(1)`, VC7.1 here `fstp st(0)` before `ret`; RTM and QFE-4035 give the same, optimize("g",off) 51 bytes. Lane gap candidate. 4 grades.
// Leaf: ?GetFrameDifferenceFromCurrent@CMainGameComponent@@IBEJJ@Z
class CMainGameComponent
{
protected:
    static long ConstantFPS;                                  // 0x01375550
    double GetCurrentGameTime() const;                        // 0x00416231
    double ConvertGTToWF(double game_time) const              // 0x0041623E (out-of-line copy)
    {
        return game_time / (1.0f / ConstantFPS);
    }
    long GetFrameDifferenceFromCurrent(long frame) const;
};

long CMainGameComponent::GetFrameDifferenceFromCurrent(long frame) const
{
    double world_frame;
    {
        double game_time = GetCurrentGameTime();
        world_frame = ConvertGTToWF(game_time);
    }
    return (long)(world_frame - frame);
}
