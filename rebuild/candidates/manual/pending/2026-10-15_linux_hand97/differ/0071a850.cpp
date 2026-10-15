// CTCParticleAttachmentTracker::vfunc_16(bool) -- puts every live attached particle into (or out of) limbo
// Grade: DIFFER(ours v retail) 78v78 -- register swap only: retail keeps the node in edi and the value address in esi (pushes edi, ebx, esi), the lane puts the node in esi and the value in edi (pushes esi, ebx, edi); same instructions otherwise. Tried: reference local, pointer locals declared in either order, no local (member path), a VC6 list iterator object.
// Leaf: vfunc_16
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071A850, length 78 bytes.
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 (three times per node), CThing::SetInLimbo(bool) @ 0x004C8CF0.
// Retail CTCParticleAttachmentTracker (PDB): AttachedParticles (std::list<CIntelligentPointer<CThing> >, 4 bytes in retail: the
// head node pointer) at +0x0c; nodes {next +0, prev +4, value +8}.
// Shape: `this` in ebp, the head cached for the entry test and re-read at the loop end, the node in edi, the value's address in
// esi; the landed IS_THING_ALIVE idiom (two fetches) then a third fetch for the call; the argument held in ebx.
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    void SetInLimbo(bool inLimbo);                  // 0x004C8CF0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CThing* GetPItem() const;                       // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct SParticleNode {
    SParticleNode*          m_pNext;                // +0x00
    SParticleNode*          m_pPrev;                // +0x04
    CBaseIntelligentPointer m_value;                // +0x08 CIntelligentPointer<CThing>
};

struct CParticleList {
    SParticleNode* m_pHead;
};

struct CTCBase_0071a850 {
    virtual void v00();
    unsigned char _pad04[8];
};

struct CTCParticleAttachmentTracker : CTCBase_0071a850 {
    CParticleList AttachedParticles;                // +0x0c
    virtual void vfunc_16(bool inLimbo);
};

void CTCParticleAttachmentTracker::vfunc_16(bool inLimbo)
{
    for (SParticleNode* node = AttachedParticles.m_pHead->m_pNext; node != AttachedParticles.m_pHead; node = node->m_pNext) {
        CBaseIntelligentPointer& particle = node->m_value;
        if (IS_THING_ALIVE(particle))
            particle.GetPItem()->SetInLimbo(inLimbo);
    }
}
