// CTCOpinionSource_007712f0::OnActivate(long, float) -- count an activation, remember its source id and set the
// expiry frame to now + duration * frame rate.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007712F0, length 69 bytes, `ret 8` (a long and a float).
// Callees: GFGetCurrentFrame @ 0x0049D870 (__fastcall, no arguments), __ftol2 @ 0x00BFEA70 for the float->long
// conversion. Global: the frame-rate long @ 0x01375550, copied into a frame slot BEFORE the frame call (its
// read precedes the call in source order) and loaded with `fild`. The manifest module label
// "CAIStateGroup_KidWatchHopscotch" is propagated; the body sits among CTCOpinionSource members (+0x80..+0x88)
// so the class is named after its neighbours, the leaf keeps the manifest name.
// Shape: `++m_count84; m_source80 = id; long fps = g_fps; m_expiryFrame88 = (long)((float)GetFrame() + (float)fps
// * duration);` -- the named fps local is what reads the global before the call and parks it in the `push ecx`
// frame slot (a bare global read is sunk below the call); the x87 order `fild frame ; fild fps ; fmul duration
// ; faddp` is the left-to-right sum with the call result parked in the first argument slot. Member names descriptive.
long __fastcall GFGetCurrentFrame();        // 0x0049D870
extern long g_framesPerSecond;              // 0x01375550

struct CTCOpinionSource_007712f0 {
    unsigned char _pad[0x80];
    long m_sourceId;                        // +0x80
    long m_activations;                     // +0x84
    long m_expiryFrame;                     // +0x88
    void OnActivate(long sourceId, float duration);
};

void CTCOpinionSource_007712f0::OnActivate(long sourceId, float duration)
{
    ++m_activations;
    m_sourceId = sourceId;
    long framesPerSecond = g_framesPerSecond;
    m_expiryFrame = (long)((float)GFGetCurrentFrame() + (float)framesPerSecond * duration);
}
