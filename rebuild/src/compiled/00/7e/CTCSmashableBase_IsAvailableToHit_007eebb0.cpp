// CTCSmashableBase::IsAvailableToHit() const -- true once more than one world frame has passed since the last hit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EEBB0, length 24 bytes.
// Callees: CTCBase::PeekWorld() @ 0x00686A80 (returns the world from the game singleton; its result is only the object
// expression of the next call), CWorld::GetWorldFrame() @ 0x0049D870 (static: reads the frame counter global).
// Layout: Ego_r.pdb CTCSmashableBase (0x18 bytes): CTCBase +0x00..+0x0b, LastWorldFrameHit +0x0c, Smashed +0x10,
// Definition (CDefPointer<CSmashableDef const>) +0x14.
// Shape: `PeekWorld()->GetWorldFrame()` calls a STATIC member through an object expression -- VC7.1 still evaluates the
// object expression (the first call) and drops its result, so ecx is never set up for the second call; the subtraction
// then compares `> 1` with `setg al` (a `? true : false` bool result, no `xor eax,eax`).
class CThing;

class CWorld { public:
    static long GetWorldFrame();                         // 0x0049D870
};

class CTCBase { public:
    void*   __vftable;                                   // +0x00
    CThing* ParentThing;                                 // +0x04
    bool    ToBeRemoved;                                 // +0x08
    bool    IsInGlobalUpdateTCs;                         // +0x09
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};

class CTCSmashableBase : public CTCBase { public:
    long  LastWorldFrameHit;                             // +0x0c
    bool  Smashed;                                       // +0x10
    void* Definition;                                    // +0x14 CDefPointer<CSmashableDef const>
    bool IsAvailableToHit() const;
};

bool CTCSmashableBase::IsAvailableToHit() const
{
    return PeekWorld().GetWorldFrame() - LastWorldFrameHit > 1 ? true : false;
}