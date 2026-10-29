// CAIStateGroup_SeekPerceivedThing::OnActivate() -- stamp the last-capture frame a tunable number of seconds in the past.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091B2F0, length 35 bytes. Vtable slot 2 of
// CAIStateGroup_SeekPerceivedThing (0x01289D24).
// Callees: CAIStateGroupBase::GetPWorld @ 0x008FCFA0 (its result is dropped: the frame counter is a static
// reached through the world pointer), GFSecondsToFrames(float) @ 0x00415FC1 (`__stdcall`: `fild [fps] ; fmul ;
// __ftol2 ; ret 4`; the manifest's `_Cons_val` label is propagated), GFGetCurrentFrame @ 0x0049D870 (static).
// Global: the seek-back time in seconds (float) @ 0x0138C6C4.
// Shape: `FrameLastCaptured = GetPWorld()->GetFrameCount() - GFSecondsToFrames(g_seekBackSeconds)` -- the object
// expression is evaluated first, then the right operand (kept in edi), then the static call. Member +0x14 from
// struct_layouts_egor.tsv (no engine header).
class CWorld { public:
    static long __fastcall GetFrame();                // 0x0049D870 (GFGetCurrentFrame)
};

extern long __stdcall GFSecondsToFrames(float seconds);   // 0x00415FC1
extern float g_seekBackSeconds;                 // 0x0138C6C4

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    virtual void v01();
    virtual void OnActivate();                  // slot 2 (+0x08)
    unsigned char _pad[0x10];                   // +0x4..+0x13
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0
public:
};

struct CAIStateGroup_SeekPerceivedThing : CAIStateGroupBase {
    long FrameLastCaptured;                     // +0x14
    virtual void OnActivate();
};

void CAIStateGroup_SeekPerceivedThing::OnActivate()
{
    FrameLastCaptured = PeekWorld().GetFrame() - GFSecondsToFrames(g_seekBackSeconds);
}