// CTCDrunkenness::FrameUpdate() -- sober up by the frames elapsed since the last update, clamp at zero, sober instantly under script supervision, and set Drunk from the threshold
// Leaf: FrameUpdate
//
// Retail 0x007BB770, 140 bytes (plain `ret`; vtable 0x0126DEB4 slot 9, Ego_r.pdb
// `?FrameUpdate@CTCDrunkenness@@UAEXXZ`; the catalog "CTCDrunkenness::vfunc_9" is the slot name).
// Callees: CWorld::GetWorldFrame() @ 0x0049D870 (static, twice; manifest "GFGetPreMainMemoryUsed" is
// propagated), CThing::IsUnderScriptSupervision() @ 0x004C7E90. Globals: 0x0126DEA8 the sobering rate
// per second (0.005f, a static float member read from memory), 0x0122DEDC the 0.0f literal.
// Layout = Ego_r.pdb CTCDrunkenness (0x20): Drunk +0xc, Drunkenness +0x10, FrameLastUpdatedSobering
// +0x14, SecondsPerFrame +0x18, ThisDrunkennessThreshold +0x1c.
// Shape: `x -= (float)frames * SecondsPerFrame * rate` (fild through the frame word, two fmul, fsubr);
// the frame store sits after the first compare's fcomp; the clamp zeroes store immediates; the Drunk
// flag is the comparison result materialised as `mov eax,1` / `xor eax,eax`.
class CWorld { public:
    static long GetWorldFrame();                            // 0x0049D870
};

class CThing { public:
    bool IsUnderScriptSupervision() const;                  // 0x004C7E90
};

class CTCDrunkenness { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    unsigned char _pad0a[2];
    bool    Drunk;                                          // +0x0c
    float   Drunkenness;                                    // +0x10
    long    FrameLastUpdatedSobering;                       // +0x14
    float   SecondsPerFrame;                                // +0x18
    float   ThisDrunkennessThreshold;                       // +0x1c
    static float SoberingPerSecond;                         // 0x0126DEA8
    void FrameUpdate();
};

void CTCDrunkenness::FrameUpdate()
{
    if (FrameLastUpdatedSobering > 0)
        Drunkenness -= (float)(CWorld::GetWorldFrame() - FrameLastUpdatedSobering) * SecondsPerFrame * SoberingPerSecond;
    FrameLastUpdatedSobering = CWorld::GetWorldFrame();
    if (Drunkenness < 0.0f)
        Drunkenness = 0.0f;
    if (Drunkenness > 0.0f && ParentThing->IsUnderScriptSupervision())
        Drunkenness = 0.0f;
    Drunk = Drunkenness > ThisDrunkennessThreshold;
}