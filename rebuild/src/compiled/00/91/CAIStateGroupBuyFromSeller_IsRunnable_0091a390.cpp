// CAIStateGroup_BuyFromSeller::IsRunnable()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091A390, length 73 bytes.
// Callees: GFGetFrameCount @ 0x0049D870 (__fastcall, reads the frame counter global),
// CAIStateGroup_WorkBase::IsInWorkHours @ 0x0091A6F0, HasSeller @ 0x0091A630 (bool: the
// creature's seller list at +0x12c holds exactly two entries), AcquireSeller @ 0x0091A9B0.
// Globals: frames-per-second long @ 0x01375550 (the idiv), the minimum seconds between buys
// (unsigned long @ 0x0138C694, hence the unsigned `jbe`).
// Runnable when enough seconds passed since FrameLastBought (+0x14, engine header), the shop
// is open, and a seller is (or can be) acquired; the final `test al,al ; setne al` is the
// `? true : false` form of the second HasSeller test.
extern long g_framesPerSecond;              // 0x01375550
extern unsigned long g_minSecondsBetweenBuys;   // 0x0138C694
long __fastcall GFGetFrameCount();          // 0x0049D870

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];               // +0x4..+0x13
};

struct CAIStateGroup_WorkBase : CAIStateGroupBase {
    bool IsInWorkHours();                   // 0x0091A6F0
    bool HasSeller();                       // 0x0091A630
    void AcquireSeller();                   // 0x0091A9B0
};

struct CAIStateGroup_BuyFromSeller : CAIStateGroup_WorkBase {
    long FrameLastBought;                   // +0x14
    virtual bool IsRunnable();
};

bool CAIStateGroup_BuyFromSeller::IsRunnable()
{
    if ((GFGetFrameCount() - FrameLastBought) / g_framesPerSecond > g_minSecondsBetweenBuys && IsInWorkHours()) {
        if (!HasSeller())
            AcquireSeller();
        return HasSeller() ? true : false;
    }
    return false;
}