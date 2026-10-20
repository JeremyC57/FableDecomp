// Grade: DIFFER(211v211) -- register swap only: retail keeps &source->Info in edi and the intro length in ebx, ours the reverse (four forms tried)
// CWeaponDrawState::Initialize(const CSampleSource* source, const SPlayRequest* request, float speed) -- primes a sample playback position: duration, intro/loop/tail lengths from the sample's loop points, the start position from the requested fraction and the section it falls in (0 intro, 1 loop, 2 tail -- which drops the loop handle --, 3 past the end), the start tick and the scaled play time
// Leaf: Initialize
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C351B0, 211 bytes (padded to 224), `ret 0Ch`.
// Identity: the catalog label "CWeaponDrawState" is propagated and unproven: the record is filled from a sample
//   description (callees read a wave-format word at +0xe, a block word at +0xc and a rate at +0x10) by the three
//   callers 0x00C208BF / 0x00C20989 / 0x00C20B2E of the 0x00C2xxxx audio code; member names are descriptive.
// Callees: the sample description's GetDuration 0x00C2D840 (float), GetLoopStart 0x00C2D900 (+4), GetLoopEnd
//   0x00C2D910 (+8, or the length when 0); __ftol2 0x00BFEA70; KERNEL32 GetTickCount (import 0x0143FDFC).
//   Constant 4294967296.0f 0x0122DCB4 (the unsigned-to-float fix-up).
// Shape: `local = Member = value` chains (member stored first, then the register copy); the lengths stay in registers (the tail spilled to the dead `source` slot);
//   `(unsigned long)((float)length * fraction)`; unsigned section compares (`jae`); `Duration * speed` loads speed first.
struct CSampleInfo {
    unsigned long Length;                          // +0x00
    unsigned long LoopStartPos;                    // +0x04
    unsigned long LoopEndPos;                      // +0x08
    unsigned short BlockSize;                      // +0x0c
    unsigned short Format;                         // +0x0e
    unsigned long SampleRate;                      // +0x10
    float GetDuration() const;                     // 0x00C2D840
    unsigned long GetLoopStart() const;            // 0x00C2D900
    unsigned long GetLoopEnd() const;              // 0x00C2D910
};
struct CSampleSource {
    unsigned char _pad00[8];
    CSampleInfo Info;                              // +0x08
};
struct SPlayRequest {
    float StartFraction;                           // +0x00
    void* LoopHandle;                              // +0x04
};
extern "C" __declspec(dllimport) unsigned long __stdcall GetTickCount();   // KERNEL32 (0x0143FDFC)
struct CWeaponDrawState {
    unsigned long _00;                             // +0x00
    float         Duration;                        // +0x04
    void*         LoopHandle;                      // +0x08
    unsigned long Position;                        // +0x0c
    long          Section;                         // +0x10
    unsigned long IntroLength;                     // +0x14
    unsigned long LoopLength;                      // +0x18
    unsigned long TailLength;                      // +0x1c
    unsigned long StartTick;                       // +0x20
    float         PlayTime;                        // +0x24
    void Initialize(const CSampleSource* source, const SPlayRequest* request, float speed);
};
void CWeaponDrawState::Initialize(const CSampleSource* source, const SPlayRequest* request, float speed)
{
    LoopHandle = request->LoopHandle;
    Duration = source->Info.GetDuration();
    unsigned long intro = IntroLength = source->Info.GetLoopStart();
    unsigned long loop = LoopLength = source->Info.GetLoopEnd() - intro;
    unsigned long tail = TailLength = source->Info.Length - loop - intro;
    unsigned long position = (unsigned long)((float)source->Info.Length * request->StartFraction);
    if (position < intro) {
        Section = 0;
        Position = position;
    }
    else if (position < intro + loop) {
        Section = 1;
        Position = position - intro;
    }
    else if (position < tail + loop + intro) {
        Position = position - loop - intro;
        Section = 2;
        if (LoopHandle)
            LoopHandle = 0;
    }
    else
        Section = 3;
    StartTick = GetTickCount();
    PlayTime = Duration * speed;
}
