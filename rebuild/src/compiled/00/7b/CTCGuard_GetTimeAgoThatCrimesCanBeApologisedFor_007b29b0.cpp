// CTCGuard::GetTimeAgoThatCrimesCanBeApologisedFor() const -- seconds since the last received apology, less a 15-second grace, clamped to [0, 10]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B29B0, length 76 bytes.
// Callees: the world frame counter GetCurrentFrame @ 0x0049D870 (a no-argument __fastcall static; manifest label
// "GFGetPreMainMemoryUsed" is propagated); globals: the frames-per-second rate @ 0x01375550, the .rdata constant 10 @ 0x0126D7CC.
// Shape: `(frames - fps * 15) / fps` (fps loaded once, `imul edx,edx,0Fh`, `cdq ; idiv`), then the generic Clamp(const T&,
// const T&, const T&) template: the value and the 0 lower bound live in an 8-byte frame (`mov [esp+4],0` is the dead store of
// the bound temporary before the value overwrites the same slot), the result is a REFERENCE (`lea eax,[esp]` / `mov eax,offset`
// then one `mov eax,[eax]`), and the upper bound is a namespace-scope `const long` bound to a const reference, which is why
// retail takes its address (0x0126D7CC) instead of an immediate.
// Local model: no engine header exists for CTCGuard; only FrameLastReceivedApology (+0x4c) is read.
class CThing;
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                         // 0x0049D870
extern long g_FramesPerSecond;                             // 0x01375550
const long MAX_APOLOGY_SECONDS = 10;                       // 0x0126D7CC (.rdata: bound to a const reference)

template<class T> inline const T& Clamp(const T& value, const T& low, const T& high)
{
    return (value < low) ? low : ((high < value) ? high : value);
}

class CTCGuard { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    unsigned char _pad08[0x44];
    long FrameLastReceivedApology;                          // +0x4c
    long GetTimeAgoThatCrimesCanBeApologisedFor() const;
};

long CTCGuard::GetTimeAgoThatCrimesCanBeApologisedFor() const
{
    long frames = CWorld::GetFrame() - FrameLastReceivedApology;
    long seconds = (frames - g_FramesPerSecond * 15) / g_FramesPerSecond;
    return Clamp(seconds, 0L, MAX_APOLOGY_SECONDS);
}