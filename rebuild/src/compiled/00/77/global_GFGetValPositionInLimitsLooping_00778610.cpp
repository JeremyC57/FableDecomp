// GFGetValPositionInLimitsLooping(float value, float start, float end, float minLimit, float maxLimit, float& position) -- position of a value inside a [start, end] window on a looping [minLimit, maxLimit] scale (the window may wrap past maxLimit); false when the value lies outside the window
// Leaf: ?GFGetValPositionInLimitsLooping@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00778610, 221 bytes (+ int3 padding), `__fastcall` (the out reference in
//   ecx, the five floats on the stack: `ret 14h`), bool in al. Ego_r.pdb / FableWin.pdb:
//   `?GFGetValPositionInLimitsLooping@@YI_NMMMMMAAM@Z` = bool __fastcall (float, float, float, float, float, float&).
// Callers: CTCTimeAppearanceFade::FrameUpdate 0x007786F0 (landed; three calls on the time of day against the fade
//   windows of CTimeAppearanceFadeDef).
// Shape: a plain window (`start < end`, `test ah,5 ; jp`) answers `(value - start) / (end - start)` when start <= value
//   <= end. A wrapping window builds a numerator and a range from the SAME sum `(end - minLimit) + tail` with
//   `tail = maxLimit - end` -- VC7.1 CSEs the two sums: it keeps `tail` on the x87 stack (`fadd st(1)`), spills the sum
//   once, copies it to the second frame word through eax and pops the dead `tail` -- and then answers
//   `offset / range` for a value in [minLimit, end) and `(value - start) / range` for one in [start, maxLimit].
//   (One shared `range` local alone coalesces into one slot and loses the copy and the kept `tail`: 200 v 221.)
//   (The retail arithmetic is reproduced as compiled; the `maxLimit - end` term and the [minLimit, end) numerator look
//   like a source slip for `maxLimit - start` and `(value - minLimit) + (maxLimit - start)`.)
bool __fastcall GFGetValPositionInLimitsLooping(float value, float start, float end, float minLimit, float maxLimit,
                                                float& position)
{
    if (start < end) {
        if (value >= start && value <= end) {
            position = (value - start) / (end - start);
            return true;
        }
        return false;
    }
    float tail = maxLimit - end;
    float offset = (end - minLimit) + tail;
    float range = (end - minLimit) + tail;
    if (value >= minLimit && value < end) {
        position = offset / range;
        return true;
    }
    if (value >= start && value <= maxLimit) {
        position = (value - start) / range;
        return true;
    }
    return false;
}