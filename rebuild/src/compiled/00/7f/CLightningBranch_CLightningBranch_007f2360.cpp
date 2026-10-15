// CLightningBranch::~CLightningBranch() -- kill the branch's effect thing (+0x08) and strip component 0x10b from its target thing (+0x00) unless either is already flagged dead, then destroy both intelligent pointers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F2360, length 112 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three calls per pointer), CThing::Kill(bool) @ 0x004C9B80,
//          CThing::RemoveTC(long) @ 0x004C9840 (manifest "CBaseObject::OnPropertyChanged" is propagated; see GOTCHAS
//          chunk 140), CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (members last first, the
//          last one a tail jump).
// Shape: a non-polymorphic class of two 8-byte intelligent pointers; the dead flag is bit 0 of the thing's byte
// +0x91; every use calls GetPItem again (no local).
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_stateFlags;                    // +0x91 bit 0: dead / being removed
    void Kill(bool immediate);                     // 0x004C9B80
    void RemoveTC(long type);                      // 0x004C9840
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                      // 0x00A01B50
    ~CBaseIntelligentPointer();                    // 0x00A01C10
};
struct CLightningBranch {
    CBaseIntelligentPointer m_target;              // +0x00
    CBaseIntelligentPointer m_effect;              // +0x08
    ~CLightningBranch();
};

CLightningBranch::~CLightningBranch()
{
    if (m_effect.GetPItem() && !(m_effect.GetPItem()->m_stateFlags & 1))
        m_effect.GetPItem()->Kill(true);
    if (m_target.GetPItem() && !(m_target.GetPItem()->m_stateFlags & 1))
        m_target.GetPItem()->RemoveTC(0x10b);
}