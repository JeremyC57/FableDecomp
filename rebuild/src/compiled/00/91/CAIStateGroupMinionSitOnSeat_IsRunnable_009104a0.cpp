// CAIStateGroup_MinionSitOnSeat::IsRunnable() -- past the sit-down frame and the base agrees.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009104A0, length 35 bytes (+ int3 padding). Vtable slot 11
// of CAIStateGroup_MinionSitOnSeat (0x0128CFBC).
// Callees: GFGetCurrentFrame @ 0x0049D870 (__fastcall, no arguments), CAIStateGroup_MinionSitOnSeatBase::IsRunnable
// @ 0x0090F9C0.
// Shape: `frame > FrameToSitDown && Base::IsRunnable()` -- the signed compare (`jle`) and the bool `&&` return
// through a full register (`mov eax,1` / `xor eax,eax`). FrameToSitDown is +0x3c (engine header).
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    unsigned char _pad[0x10];                   // +0x4..+0x13
};

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};     // 0x0049D870

struct CAIStateGroup_UsesSeat : CAIStateGroupBase {
    unsigned char _seat[0x20];                  // +0x14 PSeat, GotSeatPos, Sitting, SeatPosIndex, SeatPos, SittingOnBench
};

struct CAIStateGroup_MinionSitOnSeatBase : CAIStateGroup_UsesSeat {
    unsigned char POwner[8];                    // +0x34 CIntelligentPointer<CThing>
    virtual bool IsRunnable();                  // 0x0090F9C0
};

struct CAIStateGroup_MinionSitOnSeat : CAIStateGroup_MinionSitOnSeatBase {
    long FrameToSitDown;                        // +0x3c
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionSitOnSeat::IsRunnable()
{
    return CWorld::GetFrame() > FrameToSitDown && CAIStateGroup_MinionSitOnSeatBase::IsRunnable();
}