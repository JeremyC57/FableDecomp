// CThingAICreature::OnLevelUnload() -- base OnLevelUnload, add a null suspender (AI suspended while the set is non-empty), then forward the unload to the brain when there is one
// Leaf: OnLevelUnload
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00833BA0, length 92 bytes. CThingAICreature vtable 0x0127293C slot 43.
// Callees: CThing::OnLevelUnload @ 0x004C77D0 (the slot-43 label of the CThing vtable), the Suspenders set's
// insert_unique @ 0x00835560 (landed: hidden pair<iterator,bool> result + const key&), the brain's
// OnLevelUnload @ 0x0088B6F0 (manifest "CAIBrain::OnKill").
// Identity: slot 43 is CThing::OnLevelUnload; the manifest's "CTCCarryable::OnKill" is propagated and rejected.
// Layout: engine/CThingAICreature.h follows Ego_r.pdb, which places PBrain at +0x1d8, the bool bitfield at
// +0x1f8 and Suspenders at +0x1fc; the retail object has them 0x30 bytes lower (+0x1a8, +0x1c8, +0x1cc,
// the size at +0x1d0), so the class is modelled locally with the PDB names.
// Shape: the inline AddSuspender(0) keeps the key (0) and the unused pair result in a 12-byte frame; the
// AISuspended bit (bit 1) is rewritten from `size != 0` with the setne/shl/xor/and/xor bitfield store;
// the brain is the counted pointer's object word, null-tested before the call.
struct SSuspenderNode;

struct SSuspenderIterator {
    SSuspenderNode* m_node;
    SSuspenderIterator() { }
};

struct SSuspenderInsertResult {                     // pair<iterator, bool>
    SSuspenderIterator first;
    bool               second;
    SSuspenderInsertResult() { }
};

struct CSuspenderSet {                              // STLport set<void*>: {header, size}
    SSuspenderNode* m_header;                       // +0
    unsigned long   m_size;                         // +4
    SSuspenderInsertResult InsertUnique(void* const& value);   // 0x00835560
    bool empty() const { return m_size == 0; }
};

struct CAIBrain {
    void OnLevelUnload();                           // 0x0088B6F0
};

struct CThing {
    virtual void v00();
    void OnLevelUnload();                           // 0x004C77D0 (base implementation)
};

struct CThingAICreature : CThing {
    unsigned char _pad04[0x1a8 - 0x04];
    CAIBrain*     PBrain_Data;                      // +0x1a8 CCountedPointer<CAIBrain> (PDB +0x1d8)
    void*         PBrain_Info;                      // +0x1ac
    unsigned char _pad1b0[0x1c8 - 0x1b0];
    bool          PerformingVillageTaskFlag : 1;    // +0x1c8 bit 0 (PDB +0x1f8)
    bool          AISuspended : 1;                  // +0x1c8 bit 1
    CSuspenderSet Suspenders;                       // +0x1cc (PDB +0x1fc)
    void AddSuspender(void* suspender)
    {
        Suspenders.InsertUnique(suspender);
        AISuspended = !Suspenders.empty();
    }
    void OnLevelUnload();
};

void CThingAICreature::OnLevelUnload()
{
    CThing::OnLevelUnload();
    AddSuspender(0);
    if (PBrain_Data)
        PBrain_Data->OnLevelUnload();
}
