// COpinionTransientOffset::COpinionTransientOffset(const COpinionTransientOffset&, const COpinionPersonalityTraits&) -- a deed's transient opinion offset adapted to one creature's personality: the peak and the persisting offset are scaled by the sign-specific traits, the run-in/peak/run-out frame counts by the time scalings (run-in and run-out at least one frame), and the per-frame slopes recomputed from them
// Leaf: ??0COpinionTransientOffset
// Retail: Fable.exe (TLC, VC7.1) @ 0x00681680, 202 bytes (+ int3 padding; `ret 8`, returns `this`).
// Callees: __ftol2 0x00BFEA70 (three frame counts); float pool 0.0f 0x0122DEDC.
// Layout = Ego_r.pdb: COpinionTransientOffset (24 bytes) {Opinion +0, OffsetPerFrameRunIn +4, FramesToRunIn +8,
//   FramesOfCappedPeak +0xc, OffsetPerFrameRunOut +0x10, FramesToRunOut +0x14}; COpinionPersonalityTraits (36 bytes)
//   {..., TimeScalingRunIn +0xc, TimeScalingRunOut +0x10, PeakScalingIfPos +0x14, PeakScalingIfNeg +0x18, PersistScalingIfPos
//   +0x1c, PersistScalingIfNeg +0x20}.
// Shape: the peak (run-in slope times run-in frames) and the persisting offset (peak plus run-out slope times run-out frames)
//   are float locals; each sign test picks the trait in a ternary (`fcom 0 ; test ah,1`); the slopes are divided last, the
//   run-out one by the integer frame count (fidiv).
struct COpinionPersonalityTraits {                 // 36 bytes
    float BaseOffset;                              // +0x00
    float HeroStatScaling;                         // +0x04
    float VillageOpinionScaling;                   // +0x08
    float TimeScalingRunIn;                        // +0x0c
    float TimeScalingRunOut;                       // +0x10
    float PeakScalingIfPos;                        // +0x14
    float PeakScalingIfNeg;                        // +0x18
    float PersistScalingIfPos;                     // +0x1c
    float PersistScalingIfNeg;                     // +0x20
};
typedef long EOpinion;
class COpinionTransientOffset {
public:
    EOpinion Opinion;                              // +0x00
    float    OffsetPerFrameRunIn;                  // +0x04
    long     FramesToRunIn;                        // +0x08
    long     FramesOfCappedPeak;                   // +0x0c
    float    OffsetPerFrameRunOut;                 // +0x10
    long     FramesToRunOut;                       // +0x14
    COpinionTransientOffset(const COpinionTransientOffset& base, const COpinionPersonalityTraits& traits);
};

COpinionTransientOffset::COpinionTransientOffset(const COpinionTransientOffset& base, const COpinionPersonalityTraits& traits)
    : Opinion(base.Opinion)
{
    float runIn = (float)base.FramesToRunIn;
    float peak = runIn * base.OffsetPerFrameRunIn;
    float persist = (float)base.FramesToRunOut * base.OffsetPerFrameRunOut + peak;
    peak *= (peak >= 0.0f) ? traits.PeakScalingIfPos : traits.PeakScalingIfNeg;
    persist *= (persist >= 0.0f) ? traits.PersistScalingIfPos : traits.PersistScalingIfNeg;
    FramesToRunIn = (long)(runIn * traits.TimeScalingRunIn);
    FramesOfCappedPeak = (long)((float)base.FramesOfCappedPeak * traits.TimeScalingRunOut);
    FramesToRunOut = (long)((float)base.FramesToRunOut * traits.TimeScalingRunOut);
    long runInFrames = FramesToRunIn ? FramesToRunIn : 1;
    FramesToRunIn = runInFrames;
    long runOutFrames = FramesToRunOut ? FramesToRunOut : 1;
    FramesToRunOut = runOutFrames;
    OffsetPerFrameRunIn = peak / runInFrames;
    OffsetPerFrameRunOut = (persist - peak) / runOutFrames;
}
