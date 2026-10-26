// CTCParticleAttachmentTracker::UpdateAttachedParticles(float time) -- for every attached particle thing that still exists and is not being removed, update its particle-emitter component's attachment
// Leaf: UpdateAttachedParticles
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071A9C0, 118 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined
//   CThing::GetInterface, interface 0x4B = CTCDParticleEmitter: bit 11 of the present-bit dword at CThing+0x28),
//   CTCDParticleEmitter::UpdateEmitterThingAttachment(bool, float) @ 0x006DFFB0.
// Layout: as the landed SetAsDrawable (0x0071A8A0): AttachedParticles at +0x0c is an STLport list held as its header-node pointer;
//   CThing +0x91 bit 0 = scheduled for removal, component bitset +0x20, interface map +0x44.
// Shape: `this` spilled into the push-ecx slot because esi is reused for the interface map; end() re-read every iteration. The
//   emitter comes from the out-parameter GetInterface(T**, type) template with the end/hit ternary inside (`*ppOut = (cond ?
//   last : it)->component`): the out-parameter form gives retail's this/time registers, the ternary loads the component straight
//   into ecx (the if-assign form goes through eax, HasTC + GetTC keeps this in ebx).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_removeFlags;                // +0x91 bit 0 = scheduled for removal
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CTCDParticleEmitter { public:
    void UpdateEmitterThingAttachment(bool attach, float time);   // 0x006DFFB0
};
struct CIntelligentPointer {                    // CIntelligentPointer<CThing> (8 bytes)
    void* m_header;
    void* m_pBlock;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct SParticleNode {
    SParticleNode*      m_next;
    SParticleNode*      m_prev;
    CIntelligentPointer m_value;
};
struct CParticleList {
    SParticleNode* m_node;
    SParticleNode* begin() const { return m_node->m_next; }
    SParticleNode* end() const   { return m_node; }
};
class CTCParticleAttachmentTracker { public:
    void*         __vftable;                    // +0x00 (CTCBase)
    CThing*       ParentThing;                  // +0x04
    bool          ToBeRemoved;                  // +0x08
    bool          IsInGlobalUpdateTCs;          // +0x09
    CParticleList AttachedParticles;            // +0x0c
    void UpdateAttachedParticles(float time);
};

void CTCParticleAttachmentTracker::UpdateAttachedParticles(float time)
{
    for (SParticleNode* it = AttachedParticles.begin(); it != AttachedParticles.end(); it = it->m_next) {
        CThing* thing = it->m_value.GetPItem();
        if (thing && !(thing->m_removeFlags & 1)) {
            CTCDParticleEmitter* particles;
            if (thing->GetInterface(&particles, 0x4b))
                particles->UpdateEmitterThingAttachment(true, time);
        }
    }
}