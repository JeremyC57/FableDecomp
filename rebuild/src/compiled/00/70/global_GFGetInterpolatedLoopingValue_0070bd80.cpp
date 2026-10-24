// GFGetInterpolatedLoopingValue(float last, float current, float alpha, float range, bool forwards) -- interpolate an animation completion fraction from last to current by alpha, wrapping through range in the play direction
// Leaf: ?GFGetInterpolatedLoopingValue@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070BD80, length 149 bytes (+ int3 padding). Static `__fastcall`: the bool in cl,
//         four floats on the stack, `ret 10h`, result on the FPU stack.
// Callees: _CIfmod @ 0x00BFEB28 (the fmod intrinsic, `fld qword 1.0` as the divisor). Float pool: 0x0122DEDC = 0.0f,
//          0x01237F00 = 1.0 (double).
// Identity: Ego_r.pdb `?GFGetInterpolatedLoopingValue@@YIMMMMM_N@Z` (Ego_r 0x00578880, right after CAnimateThing::Update in
//   the CAnimateThing TU): float (float, float, float, float, bool) __fastcall -- the bool is the only register-sized
//   argument, so it travels in cl while the four floats stay on the stack in order. Single caller CAnimSet::
//   GetInterpolatedTime (0x0070C800, landed), which declares it as "AnimationInterpolation_CalcValue(bool, float x4)" --
//   the same calling sequence under the descriptive manifest label.
// Shape: forwards: `current >= last` (`test ah,1 ; jne`) lerps, else the wrapped span `(range - last + current) * alpha +
//   last` goes through fmod(x, 1.0); backwards: `current <= last` (`test ah,41h ; jp`) lerps down from last, else the
//   wrapped value is pulled back into range with a `while (value < 0.0f) value += range;` loop (`test ah,5 ; jp`). The
//   one `current` vs `last` compare is shared by both branches (VC7.1 hoists it above the bool test).
extern "C" double __cdecl fmod(double x, double y);
#pragma intrinsic(fmod)

float __fastcall GFGetInterpolatedLoopingValue(float last, float current, float alpha, float range, bool forwards)
{
    if (forwards) {
        if (current >= last)
            return (current - last) * alpha + last;
        return (float)fmod((range - last + current) * alpha + last, 1.0);
    }
    if (current <= last)
        return last - (last - current) * alpha;
    float value = last - (range - current + last) * alpha;
    while (value < 0.0f)
        value += range;
    return value;
}