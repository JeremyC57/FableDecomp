// CTCParticleAttachmentTracker::SetAsDrawable(bool drawable) -- forward the drawable state to every attached particle thing that still exists and is not being removed
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071A8A0, 78 bytes (one caller, 0x004C8F62).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times per element), the particle thing's virtual slot 28 (+0x70).
// Layout (PDB CTCParticleAttachmentTracker, 0x18 = CTCBase 0x0c + the list; no engine header): AttachedParticles at +0x0c, an
//   STLport list<CIntelligentPointer<CThing> > held as its header-node pointer (node: next +0, prev +4, value +8).
//   CThing +0x91 bit 0 = scheduled for removal.
// Shape: a begin/end iterator loop; `this` lives in ebp (no frame pointer), end() re-read from the member every iteration
//   (the virtual call may change the list), the element's address (`lea esi,[edi+8]`) kept in esi for the three GetPItem calls.
// Leaf: SetAsDrawable
class CThing { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void SetAsDrawable(bool drawable);   // slot 28 (+0x70)
    unsigned char _pad04[0x8d];             // +0x04
    unsigned char m_removeFlags;            // +0x91 bit 0 = scheduled for removal
};

struct CIntelligentPointer {                // CIntelligentPointer<CThing> (8 bytes)
    void* m_header;
    void* m_pBlock;
    CThing* GetPItem() const;               // 0x00A01B50 (CBaseIntelligentPointer::GetPItem)
};

struct SParticleNode {
    SParticleNode*      m_next;             // +0x00
    SParticleNode*      m_prev;             // +0x04
    CIntelligentPointer m_value;            // +0x08
};

struct CParticleList {                      // STLport list<CIntelligentPointer<CThing> >
    SParticleNode* m_node;
    SParticleNode* begin() const { return m_node->m_next; }
    SParticleNode* end() const   { return m_node; }
};

class CTCParticleAttachmentTracker { public:
    void*         __vftable;                // +0x00 (CTCBase)
    CThing*       ParentThing;              // +0x04
    bool          ToBeRemoved;              // +0x08
    bool          IsInGlobalUpdateTCs;      // +0x09
    CParticleList AttachedParticles;        // +0x0c
    void SetAsDrawable(bool drawable);
};

void CTCParticleAttachmentTracker::SetAsDrawable(bool drawable)
{
    for (SParticleNode* it = AttachedParticles.begin(); it != AttachedParticles.end(); it = it->m_next) {
        CIntelligentPointer& particle = it->m_value;
        if (particle.GetPItem() && !(particle.GetPItem()->m_removeFlags & 1))
            particle.GetPItem()->SetAsDrawable(drawable);
    }
}