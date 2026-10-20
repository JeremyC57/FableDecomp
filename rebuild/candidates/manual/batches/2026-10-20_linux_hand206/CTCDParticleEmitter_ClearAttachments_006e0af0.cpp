// CTCDParticleEmitter::ClearAttachments() -- detach the emitter: a still-living attached thing gets this emitter's thing as a dependent (interface 0xd8), then the attachment pointer, name, flags and index are reset and the emitter is flagged to reset its position and orientation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E0AF0, 225 bytes (plain `ret`).
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10
// (CIntelligentPointer<CThing> vtable 0x01238C6C), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020,
// CTCDependentThings::AddDependentThing(CThing*) @ 0x0071A8F0 (as the landed OnKill 0x006E0A60), CCharString::operator=
// (const char*) @ 0x0099EFE0 (literal "" @ 0x0122D70E).
// Layout: Ego_r.pdb CTCDParticleEmitter: PAttachedToThing +0x1c, AttachmentFlags +0x24, AttachmentPosName +0x28,
// AttachmentPosIndex +0x2c, the bool:1 group at +0x44/+0x45 (ParticleCreationDisabled bit 3, ResetEmitterPosition bit 4,
// ResetEmitterOrientation bit 5, AttachedToCamera bit 6, Moving bit 7; AttachedToThing +0x45 bit 0).
// Shape: the OnKill alive-and-has-interface test (GetPItem three times, the out-parameter lookup leaving the component in
// eax); the bit-field stores on each side of the calls merge into one read-modify-write per byte; the pointer is cleared
// through a CIntelligentPointer(0) temporary and the inline operator= (`SetPItem(other.GetPItem())`).
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                              // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCDependentThings {                             // interface 0xd8
    void AddDependentThing(CThing* thing);              // 0x0071A8F0
};
struct CCharString {
    void* PStringData;
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
inline bool IS_THING_ALIVE(const CIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
struct CTCDParticleEmitter {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x1c - 0x08];
    CIntelligentPointer PAttachedToThing;               // +0x1c
    unsigned long AttachmentFlags;                      // +0x24
    CCharString AttachmentPosName;                      // +0x28
    long AttachmentPosIndex;                            // +0x2c
    unsigned char _pad30[0x44 - 0x30];
    bool WaitingForParticleToLoad : 1;                  // +0x44 bit 0
    bool ForceParticleToLoad : 1;
    bool IndependantObject : 1;
    bool ParticleCreationDisabled : 1;                  // bit 3
    bool ResetEmitterPosition : 1;                      // bit 4
    bool ResetEmitterOrientation : 1;                   // bit 5
    bool AttachedToCamera : 1;                          // bit 6
    bool Moving : 1;                                    // bit 7
    bool AttachedToThing : 1;                           // +0x45 bit 0
    bool AlwaysUpdate : 1;
    bool Asleep : 1;
    void ClearAttachments();
};

void CTCDParticleEmitter::ClearAttachments()
{
    if (IS_THING_ALIVE(PAttachedToThing)) {
        CTCDependentThings* dependents;
        if (PAttachedToThing.GetPItem()->GetInterface(&dependents, 0xd8))
            dependents->AddDependentThing(ParentThing);
    }
    AttachmentFlags = 1;
    ResetEmitterPosition = true;
    ResetEmitterOrientation = true;
    Moving = false;
    PAttachedToThing = CIntelligentPointer(0);
    AttachmentPosName = "";
    AttachmentPosIndex = -1;
    ParticleCreationDisabled = false;
    AttachedToCamera = false;
    AttachedToThing = false;
}
