// GFSmoothTowards<float>(const float& target, const float& max_step, float& current) -- move current towards target by at most max_step, never overshooting
// Leaf: ??$GFSmoothTowards@M@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006ADC20, length 106 bytes, __fastcall
//   (`YI`): ecx = &target, edx = &max_step, &current on the stack, `ret 4`.
// Callees: none. Literal: 0.0f @ 0x0122DEDC.
// Identity: Ego_r.pdb `??$GFSmoothTowards@M@@YIXABM0AAM@Z` (name set only; donor-keyed address). One caller,
//   0x006ABF45.
// Shape: the step is target - current clamped to [-max_step, max_step] (the negated limit spilled over the dead
//   &current argument slot), added to current through the reference, then the overshoot test picks its compare
//   by the sign of the step; the clamp-back stores are integer moves of target.
template <class T>
void __fastcall GFSmoothTowards(const T& target, const T& max_step, T& current)
{
    T step = target - current;
    if (step < -max_step)
        step = -max_step;
    else if (step > max_step)
        step = max_step;
    current += step;
    if (step > 0) {
        if (current > target)
            current = target;
    } else {
        if (current < target)
            current = target;
    }
}
template void __fastcall GFSmoothTowards<float>(const float&, const float&, float&);