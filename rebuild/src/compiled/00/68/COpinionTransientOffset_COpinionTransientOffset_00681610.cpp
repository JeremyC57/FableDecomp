// COpinionTransientOffset::COpinionTransientOffset(EOpinion opinion, float peakOffset, long framesToRunIn, long framesToRunOut, float finalOffset) -- a transient opinion bump: ramp to the peak over framesToRunIn frames, then towards the final offset over framesToRunOut frames (a zero frame count means one frame and the full step at once)
// Leaf: ??0COpinionTransientOffset@@QAE@W4EOpinion@@MJJM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00681610, length 100 bytes (padded to 112). No callees.
// Layout: PDB COpinionTransientOffset (0x18 bytes): Opinion +0, OffsetPerFrameRunIn +4, FramesToRunIn +8,
// FramesOfCappedPeak +0xc, OffsetPerFrameRunOut +0x10, FramesToRunOut +0x14 (no engine header).
// Shape: each per-frame step is first stored undivided and overwritten by the division only when the
// frame count is non-zero; the count member is stored after the join from the (possibly bumped) parameter;
// the run-out difference is a named local kept on the FPU across the test.
// Name fix (naming agent NF21, 2026-11-11): the first parameter and Opinion are the PDB enum EOpinion: Ego_r.pdb
//   ??0COpinionTransientOffset@@QAE@W4EOpinion@@MJJM@Z (0x0058F680, between the default and the traits-scaling ctors as here);
//   it was spelled with long (J).
enum EOpinion;                      // MS forward enum (Ego_r.pdb)
class COpinionTransientOffset { public:
    EOpinion Opinion;               // +0x00
    float OffsetPerFrameRunIn;      // +0x04
    long  FramesToRunIn;            // +0x08
    long  FramesOfCappedPeak;       // +0x0c
    float OffsetPerFrameRunOut;     // +0x10
    long  FramesToRunOut;           // +0x14
    COpinionTransientOffset(EOpinion opinion, float peakOffset, long framesToRunIn, long framesToRunOut, float finalOffset);
};

COpinionTransientOffset::COpinionTransientOffset(EOpinion opinion, float peakOffset, long framesToRunIn,
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