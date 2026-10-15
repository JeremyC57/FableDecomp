// CTCHaste::FrameUpdate() -- counts a running pause down (PauseFrames >= 0 is decremented) and, once flagged for termination, removes the haste TC (0x8c) from its owner
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00800000, length 32 bytes (manifest "CTCHaste::vfunc_9": vtable slot 9 of the
// CTCBase interface is FrameUpdate).
// Callees: CThing::RemoveTC(ETCInterfaceType) @ 0x004C9840 (manifest "CBaseObject::OnPropertyChanged" is propagated), with
// interface type 0x8c (the haste TC removes itself).
// Shape: `test ; jl ; dec eax ; mov [ecx+14h],eax` = `if (PauseFrames >= 0) --PauseFrames;`; the bool at +0x10 is tested
// through al; ParentThing (+4) becomes `this` for the call, which is a tail position but VC7.1 keeps `call ; ret` (pushed arg).
// Local model: no engine header exists for CTCHaste.
struct CThing {
    void RemoveTC(long interfaceType);                      // 0x004C9840
};

struct CTCHaste {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    unsigned char _pad08[4];                                // +0x08 CTCBase flags
    float Multiplier;                                       // +0x0c
    bool ToTerminate;                                       // +0x10
    long PauseFrames;                                       // +0x14
    virtual void FrameUpdate();
};

void CTCHaste::FrameUpdate()
{
    if (PauseFrames >= 0)
        --PauseFrames;
    if (ToTerminate)
        ParentThing->RemoveTC(0x8c);                        // this TC's own interface type
}
