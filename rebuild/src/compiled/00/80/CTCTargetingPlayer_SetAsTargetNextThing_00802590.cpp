// CTCTargetingPlayer::SetAsTargetNextThing() -- queues a "target the next thing" request: method 1 while a live thing is lock-targeted, else method 0
// Grade: RELOCATION_MATCH -- the method-0 arm is laid out right after the flag test when the condition is a negated inline IS_THING_ALIVE(locked) helper (`if (!IS_THING_ALIVE(locked)) method 0; else method 1;`); the open-coded `!locked || dead` and `locked && !dead` if/else forms always place the method-1 arm first
// Leaf: ?SetAsTargetNextThing@CTCTargetingPlayer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00802590, length 64 bytes.
// Callees: the virtual GetPTargetLockedThing() (vtable 0x0124280C slot 36 = 0x004E2328, `+0x90`), the global frame getter
// @ 0x0049D870 (`__cdecl`, as in the landed SetAsTargetNextThingInDirection 0x008023C0; manifest "GFGetPreMainMemoryUsed"
// is propagated).
// Layout: engine/CTCTargetingPlayer.h (Ego_r.pdb): TargetNextThing +0x3c, TargetNextThingMethod +0x40,
// FrameSetAsTargetNextThing +0x44. The header is a flattened POD (`void*` vptr) and the body makes a virtual call, so the
// class is a local polymorphic model with the header's member names and offsets. The locked thing's byte +0x91 bit 0 is
// its "being removed" flag (the same test as CTCTavernGame::GetHeroStats 0x005EB4B0).
// Shape: the inline SetAsTargetNextThingInDirection(method) (landed 0x008023C0) is expanded in both arms of an if/else, so
// retail tail-duplicates the three stores and the frame call; the 1 is hoisted into ecx and serves as both the method
// and the bool.
struct CThing {
    unsigned char _pad_0x00[0x91];
    unsigned char m_removalFlags;            // +0x91 (bit 0)
};

typedef long ETargetNextThingMethod;

extern long GFGetCurrentFrame();             // 0x0049D870

struct CTCTargetingPlayer {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual CThing* GetPTargetLockedThing();          // slot 36 (+0x90)
    unsigned char          _pad_0x04[0x34];           // CTCTargetingBase members
    long                   ZLockTargetSelectionTimer; // +0x38
    bool                   TargetNextThing;           // +0x3c
    bool                   ForcedToHaveNoTarget;      // +0x3d
    unsigned char          _pad_0x3e[2];
    ETargetNextThingMethod TargetNextThingMethod;     // +0x40
    long                   FrameSetAsTargetNextThing; // +0x44

    void SetAsTargetNextThingInDirection(ETargetNextThingMethod method)   // landed 0x008023C0, inline here
    {
        TargetNextThing = true;
        TargetNextThingMethod = method;
        FrameSetAsTargetNextThing = GFGetCurrentFrame();
    }
    void SetAsTargetNextThing();
};

// IS_THING_ALIVE on a raw pointer: set and the dead bit (+0x91 bit 0) clear (inlined)
inline bool IS_THING_ALIVE(const CThing* thing)
{
    if (thing != 0) {
        if (!(thing->m_removalFlags & 1))
            return true;
    }
    return false;
}
void CTCTargetingPlayer::SetAsTargetNextThing()
{
    CThing* locked = GetPTargetLockedThing();
    if (!IS_THING_ALIVE(locked))
        SetAsTargetNextThingInDirection(0);
    else
        SetAsTargetNextThingInDirection(1);
}