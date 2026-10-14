// CAIStateGroup_MinionSitOnSeat::OnReplace() -- base OnReplace, then sit down 80 seconds from now.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090F710, length 34 bytes (+ int3 padding). Vtable slot 10
// of CAIStateGroup_MinionSitOnSeat (0x0128CFBC).
// Callees: CAIStateGroup_UsesSeat::OnReplace @ 0x009387F0, GFGetCurrentFrame @ 0x0049D870 (__fastcall, no
// arguments; the manifest's "GFGetPreMainMemoryUsed" is wrong). Global: the frame-rate long @ 0x01375550.
// Shape: the frame rate is read into a named local before the frame call (only a local keeps the global read in
// edi across the call; the bare expression reloads it after), `g * 80` is `lea ecx,[edi+edi*4] ; shl ecx,4`, then
// `FrameToSitDown = frame + fps * 80` (+0x3c, engine header).
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void OnReplace();                   // slot 10 (+0x28)
    unsigned char _pad[0x10];                   // +0x4..+0x13
};

extern long g_FramesPerSecond;                  // 0x01375550
extern long __fastcall GFGetCurrentFrame();     // 0x0049D870

struct CAIStateGroup_UsesSeat : CAIStateGroupBase {
    unsigned char _seat[0x20];                  // +0x14 PSeat, GotSeatPos, Sitting, SeatPosIndex, SeatPos, SittingOnBench
    virtual void OnReplace();                   // 0x009387F0
};

struct CAIStateGroup_MinionSitOnSeatBase : CAIStateGroup_UsesSeat {
    unsigned char POwner[8];                    // +0x34 CIntelligentPointer<CThing>
};

struct CAIStateGroup_MinionSitOnSeat : CAIStateGroup_MinionSitOnSeatBase {
    long FrameToSitDown;                        // +0x3c
    virtual void OnReplace();
};

void CAIStateGroup_MinionSitOnSeat::OnReplace()
{
    CAIStateGroup_UsesSeat::OnReplace();
    long framesPerSecond = g_FramesPerSecond;   // read before the frame call (kept in edi across it)
    FrameToSitDown = GFGetCurrentFrame() + framesPerSecond * 80;
}
