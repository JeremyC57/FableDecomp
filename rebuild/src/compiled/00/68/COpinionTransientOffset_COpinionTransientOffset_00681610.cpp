// COpinionTransientOffset::COpinionTransientOffset(EOpinion opinion, float peakOffset, long framesToRunIn, long framesToRunOut, float finalOffset) -- a transient opinion bump: ramp to the peak over framesToRunIn frames, then towards the final offset over framesToRunOut frames (a zero frame count means one frame and the full step at once)
// Leaf: ??0COpinionTransientOffset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00681610, length 100 bytes (padded to 112). No callees.
// Layout: PDB COpinionTransientOffset (0x18 bytes): Opinion +0, OffsetPerFrameRunIn +4, FramesToRunIn +8,
// FramesOfCappedPeak +0xc, OffsetPerFrameRunOut +0x10, FramesToRunOut +0x14 (no engine header).
// Shape: each per-frame step is first stored undivided and overwritten by the division only when the
// frame count is non-zero; the count member is stored after the join from the (possibly bumped) parameter;
// the run-out difference is a named local kept on the FPU across the test.
class COpinionTransientOffset { public:
    long  Opinion;                  // +0x00 EOpinion
    float OffsetPerFrameRunIn;      // +0x04
    long  FramesToRunIn;            // +0x08
    long  FramesOfCappedPeak;       // +0x0c
    float OffsetPerFrameRunOut;     // +0x10
    long  FramesToRunOut;           // +0x14
    COpinionTransientOffset(long opinion, float peakOffset, long framesToRunIn, long framesToRunOut, float finalOffset);
};

COpinionTransientOffset::COpinionTransientOffset(long opinion, float peakOffset, long framesToRunIn,
                                                 long framesToRunOut, float finalOffset)
{
    Opinion = opinion;
    OffsetPerFrameRunIn = peakOffset;
    if (framesToRunIn == 0)
        framesToRunIn = 1;
    else
        OffsetPerFrameRunIn = peakOffset / framesToRunIn;
    FramesToRunIn = framesToRunIn;
    FramesOfCappedPeak = 0;
    float runOut = finalOffset - peakOffset;
    OffsetPerFrameRunOut = runOut;
    if (framesToRunOut == 0)
        framesToRunOut = 1;
    else
        OffsetPerFrameRunOut = runOut / framesToRunOut;
    FramesToRunOut = framesToRunOut;
}