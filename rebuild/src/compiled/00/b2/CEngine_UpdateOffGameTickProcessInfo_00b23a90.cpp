// CEngine::UpdateOffGameTickProcessInfo(bool) -- off-world tick throttling: when processing is enabled, accumulate (now - last) * MinProcessFrequency ticks, decide whether this frame processes (spending one tick), clamp the accumulator to [0, MaxProcessTicksBehind] and remember the time stamp
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B23A90, 186 bytes (+ int3 padding).
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

class CEngine { public:
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