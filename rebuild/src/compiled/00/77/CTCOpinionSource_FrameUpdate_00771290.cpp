// CTCOpinionSource::FrameUpdate() -- clears the virtual deed once the world frame reaches FrameToClearVirtualDeed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00771290, length 38 bytes.
// Callees: CWorld::GetWorldFrame() @ 0x0049D870 (static; returns the frame global, manifest label
// "GFGetPreMainMemoryUsed" is propagated).
// Retail CTCOpinionSource offsets (the PDB's 16-byte DefStack vector is 12 bytes in retail): VirtualDeed +0x80,
// VirtualDeedID +0x84, FrameToClearVirtualDeed +0x88 (unsigned: `jb`).
// Shape: the frame getter is a plain static call (no GetWorld, nothing in ecx); the deed is cleared with an immediate store.
class CWorld { public:
    static long __fastcall GetFrame();                    // 0x0049D870
};

struct CTCBase_00771290 {
    virtual void v00();
    unsigned char _pad04[8];
};

class CTCOpinionSource : public CTCBase_00771290 { public:
    unsigned char _pad0c[0x80 - 0x0c];
    long          VirtualDeed;                      // +0x80 EOpinionDeedType
    long          VirtualDeedID;                    // +0x84
    unsigned long FrameToClearVirtualDeed;          // +0x88
    void FrameUpdate();
};

void CTCOpinionSource::FrameUpdate()
{
    if (VirtualDeed != 0 && (unsigned long)CWorld::GetFrame() >= FrameToClearVirtualDeed)
        VirtualDeed = 0;
}