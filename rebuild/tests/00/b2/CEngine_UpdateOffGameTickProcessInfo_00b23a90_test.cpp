#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
// Behaviour test: the model and logic below are a copy of the src/ file; the callees are test doubles.
// CEngine::UpdateOffGameTickProcessInfo(bool) -- off-world tick throttling: when processing is enabled, accumulate (now - last) * MinProcessFrequency ticks, decide whether this frame processes (spending one tick), clamp the accumulator to [0, MaxProcessTicksBehind] and remember the time stamp
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B23A90, 184 bytes (+ int3 padding).
// Callees: GFGetTime @ 0x009E1BC0 (`__fastcall`, double on the FPU). Float pool: 0x0122DED8 = 1.0f, 0x0122DEDC = 0.0f.
// Layout: Ego_r.pdb <unnamed-type-OffWorldTickProcessInfo> (0x20 bytes: ProcessThisFrame +0, ProcessingEnabled +1,
//   ProcessTicksBehindAccumilator +4, MaxProcessTicksBehind +8, MaxProcessTicksBehindBeforeCatchup +0xc,
//   MinProcessFrequency +0x10, double LastFrameTimeStamp +0x18). engine/CEngine.h places OffWorldTickProcessInfo at +0x100
//   but the retail body addresses it at +0xF8 (ProcessThisFrame +0xF8 .. LastFrameTimeStamp +0x110), so `this` is modelled
//   locally with the PDB member names and the record 8 bytes below the header.
// Shape: `now` is kept on the x87 stack across the whole body (`fld st(0)` before the subtraction, `fstp qword` into the
//   time stamp on both exits); the accumulator sum stays on the stack for the catch-up compare (`fst`) and the decrement;
//   `x < c` is `test ah,5 ; jp`, `x > c` is `test ah,41h ; jne`; the upper clamp is an integer copy.
double __fastcall GFGetTime();                         // 0x009E1BC0

struct SOffWorldTickProcessInfo {                      // <unnamed-type-OffWorldTickProcessInfo>, 0x20 bytes
    bool   ProcessThisFrame;                           // +0x00
    bool   ProcessingEnabled;                          // +0x01
    float  ProcessTicksBehindAccumilator;              // +0x04
    float  MaxProcessTicksBehind;                      // +0x08
    float  MaxProcessTicksBehindBeforeCatchup;         // +0x0c
    float  MinProcessFrequency;                        // +0x10
    double LastFrameTimeStamp;                         // +0x18
};

struct CEngine {
    unsigned char            _pad0[0xf8];
    SOffWorldTickProcessInfo OffWorldTickProcessInfo;  // +0xF8 (retail; PDB +0x100)
    void UpdateOffGameTickProcessInfo(bool allowSkip);
};

void CEngine::UpdateOffGameTickProcessInfo(bool allowSkip)
{
    if (!OffWorldTickProcessInfo.ProcessingEnabled) {
        OffWorldTickProcessInfo.ProcessThisFrame = true;
        return;
    }
    double now = GFGetTime();
    OffWorldTickProcessInfo.ProcessTicksBehindAccumilator +=
        (now - OffWorldTickProcessInfo.LastFrameTimeStamp) * OffWorldTickProcessInfo.MinProcessFrequency;
    if (allowSkip && OffWorldTickProcessInfo.ProcessTicksBehindAccumilator < OffWorldTickProcessInfo.MaxProcessTicksBehindBeforeCatchup) {
        OffWorldTickProcessInfo.ProcessThisFrame = false;
    } else {
        OffWorldTickProcessInfo.ProcessTicksBehindAccumilator -= 1.0f;
        OffWorldTickProcessInfo.ProcessThisFrame = true;
    }
    if (OffWorldTickProcessInfo.ProcessTicksBehindAccumilator < 0.0f) {
        OffWorldTickProcessInfo.LastFrameTimeStamp = now;
        OffWorldTickProcessInfo.ProcessTicksBehindAccumilator = 0.0f;
        return;
    }
    if (OffWorldTickProcessInfo.ProcessTicksBehindAccumilator > OffWorldTickProcessInfo.MaxProcessTicksBehind)
        OffWorldTickProcessInfo.ProcessTicksBehindAccumilator = OffWorldTickProcessInfo.MaxProcessTicksBehind;
    OffWorldTickProcessInfo.LastFrameTimeStamp = now;
}

// ---- test doubles and driver ----
static double g_now = 0.0;
double __fastcall GFGetTime() { return g_now; }
static CEngine e;
static SOffWorldTickProcessInfo& I() { return e.OffWorldTickProcessInfo; }
static void Set(bool enabled, float acc, float maxBehind, float catchup, float freq, double last)
{
    I().ProcessThisFrame = 7; I().ProcessingEnabled = enabled; I().ProcessTicksBehindAccumilator = acc;
    I().MaxProcessTicksBehind = maxBehind; I().MaxProcessTicksBehindBeforeCatchup = catchup; I().MinProcessFrequency = freq;
    I().LastFrameTimeStamp = last;
}
int main()
{
    bool ok = true;
    // disabled: only the flag is set
    Set(false, 5.0f, 10.0f, 3.0f, 30.0f, 1.0); g_now = 2.0; e.UpdateOffGameTickProcessInfo(true);
    ok = ok && I().ProcessThisFrame == true && I().ProcessTicksBehindAccumilator == 5.0f && I().LastFrameTimeStamp == 1.0;
    // skip allowed and below catch-up: accumulate 0.5*2 = 1 -> 1.5, no processing this frame
    Set(true, 0.5f, 10.0f, 3.0f, 2.0f, 1.0); g_now = 1.5; e.UpdateOffGameTickProcessInfo(true);
    ok = ok && I().ProcessThisFrame == false && I().ProcessTicksBehindAccumilator == 1.5f && I().LastFrameTimeStamp == 1.5;
    // skip not allowed: spend one tick (1.5 - 1 = 0.5) and process
    Set(true, 0.5f, 10.0f, 3.0f, 2.0f, 1.0); g_now = 1.5; e.UpdateOffGameTickProcessInfo(false);
    ok = ok && I().ProcessThisFrame == true && I().ProcessTicksBehindAccumilator == 0.5f && I().LastFrameTimeStamp == 1.5;
    // goes negative: clamped to 0
    Set(true, 0.25f, 10.0f, 3.0f, 2.0f, 1.0); g_now = 1.25; e.UpdateOffGameTickProcessInfo(false);
    ok = ok && I().ProcessThisFrame == true && I().ProcessTicksBehindAccumilator == 0.0f && I().LastFrameTimeStamp == 1.25;
    // above catch-up and above the max: process, then clamp to MaxProcessTicksBehind
    Set(true, 20.0f, 10.0f, 3.0f, 2.0f, 1.0); g_now = 2.0; e.UpdateOffGameTickProcessInfo(true);
    ok = ok && I().ProcessThisFrame == true && I().ProcessTicksBehindAccumilator == 10.0f && I().LastFrameTimeStamp == 2.0;
    if (!ok) { std::printf("00b23a90_TEST FAIL\n"); return 1; }
    std::printf("00b23a90_TEST PASS\n");
    return 0;
}