// CTCWife::GetNumHoursMarriedFor() const -- game hours since the wedding frame (0 if the wedding frame is still in the future)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CEC90, length 62 bytes (+ int3 padding).
// Callees: GetWorldFrame() @ 0x0049D870 (returns the global @ 0x013B89BC; its "GFGetPreMainMemoryUsed" label is
// propagated). Globals: 0x01375550 = the frames-per-second long (15); float 0x0125ED40 = 3600.0f.
// Ego_r.pdb: ?GetNumHoursMarriedFor@CTCWife@@ABEMXZ. Layout: engine/CTCWife.h FrameGotMarriedToThePlayer +0x20.
// Shape: a zero-initialised float result local in the `push ecx` slot (its 0 stored as a dword); the frame is fetched
// twice (once for the test, once for the difference); the difference reuses the same slot for `fild`, the divisor is
// `fild [fps] ; fmul [3600.0f]`.
#include "engine/CTCWife.h"

long GetWorldFrame();                                       // 0x0049D870
extern long g_FramesPerSecond;                              // 0x01375550

struct CTCWife_Hours : CTCWife {
    float GetNumHoursMarriedFor() const;
};

float CTCWife_Hours::GetNumHoursMarriedFor() const
{
    float hours = 0.0f;
    if (FrameGotMarriedToThePlayer <= GetWorldFrame())
        hours = (float)(GetWorldFrame() - FrameGotMarriedToThePlayer) / (g_FramesPerSecond * 3600.0f);
    return hours;
}
