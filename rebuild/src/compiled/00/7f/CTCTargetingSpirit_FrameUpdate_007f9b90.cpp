// CTCTargetingSpirit::FrameUpdate() -- drop a living targeted thing that is flagged (+0x91 bit 5) or no longer targetable, then refresh the target list on every fourth frame (staggered by the owner's UID)
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F9B90, length 100 bytes (padded row 112); CTCTargetingSpirit vtable 0x012419A4 slot 9.
// Callees: CTCBase::FrameUpdate @ 0x006868A0 (empty), virtual slot 33 GetPTargetedThing (+0x84), virtual slot 34 SetPTargetedThing(CThing*)
// (+0x88, null), CWorld::GetWorldFrame (static) @ 0x0049D870, CTCTargetingSpirit::UpdateTargetList @ 0x007F9960 (tail jump).
// Layout: CThing UID (unsigned __int64) +0x18, flags byte +0x91 (bit 0 dead, bit 5), CTCTargeted interface 9 = bit 9 of +0x20.
// Shape: the flags byte is read once into cl; HasTC on the dword (`mov ecx,[eax+20h] ; test ch,2`); `(uid + frame) % 4 == 0` on the
// unsigned 64-bit sum gives `and eax,3 ; xor edx,edx ; or eax,edx`; the UID is a named local read before the frame call (inline it and
// the call runs first).
class CThing {
public:
    unsigned char _pad00[0x18];
    unsigned __int64 UID;                       // +0x18
    unsigned long m_presentBits[9];             // +0x20
    unsigned char _pad44[0x91 - 0x44];
    unsigned char m_flags91;                    // +0x91
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
};
class CWorld { public:
    static long __fastcall GetFrame();                // 0x0049D870
};
class CTCBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void FrameUpdate();                 // slot 9 (0x006868A0)
    CThing* ParentThing;                        // +0x04
};
struct CTCTargetingSpirit : CTCBase {
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetPTargetedThing();        // slot 33 (+0x84)
    virtual void SetPTargetedThing(CThing* thing);   // slot 34 (+0x88)
    virtual void FrameUpdate();
    void UpdateTargetList();                    // 0x007F9960
};

void CTCTargetingSpirit::FrameUpdate()
{
    CTCBase::FrameUpdate();
    CThing* target = GetPTargetedThing();
    if (target) {
        unsigned char flags = target->m_flags91;
        if (!(flags & 1) && ((flags & 0x20) || !target->HasTC(9)))
            SetPTargetedThing(0);
    }
    unsigned __int64 uid = ParentThing->UID;
    if ((uid + CWorld::GetFrame()) % 4 == 0)
        UpdateTargetList();
}