// CThingAnimatable::SetSuspendAnimation(bool suspend, void* suspender) -- add or remove a suspender from the creature's suspender set, then mark the AI suspended while the set is non-empty
// Retail: Fable.exe (TLC, VC7.1) @ 0x00833A10, 89 bytes. Callees: the Suspenders set's insert_unique @ 0x00835560
// (hidden pair<iterator,bool> result + const key&) and erase(const key&) @ 0x00835960. 90 call sites.
// Identity: the members are CThingAICreature's (see the landed CThingAICreature::OnLevelUnload 0x00833BA0, which
// inlines the add half): AISuspended is bit 1 of retail +0x1c8 (PDB +0x1f8) and Suspenders the STLport set at
// +0x1cc with its size at +0x1d0. The catalog name is kept for the symbol; engine/CThingAICreature.h has only
// padding there and the PDB offsets, so the class is modelled locally.
// Shape: the key is the `suspender` argument slot itself (`lea eax,[esp+14h]` = a const-reference parameter
// use), the unused pair result sits in the 8-byte frame; the bit is rewritten from `size != 0` with the
// setne/shl/xor/and/xor bitfield merge.
struct SSuspenderNode;

struct SSuspenderIterator {
    SSuspenderNode* m_node;
    SSuspenderIterator() { }
};

struct SSuspenderInsertResult {                      // pair<iterator, bool>
    SSuspenderIterator first;
    bool               second;
    SSuspenderInsertResult() { }
};

struct CSuspenderSet {                               // STLport set<void*>: {header, size}
    SSuspenderNode* m_header;                        // +0
    unsigned long   m_size;                          // +4
    SSuspenderInsertResult InsertUnique(void* const& value);   // 0x00835560
    unsigned long          Erase(void* const& value);          // 0x00835960
    bool empty() const { return m_size == 0; }
};

struct CThingAnimatable {
    void*         __vftable;                         // +0x000
    unsigned char _pad004[0x1c8 - 0x004];
    bool          PerformingVillageTaskFlag : 1;     // +0x1c8 bit 0 (PDB +0x1f8)
    bool          AISuspended : 1;                   // +0x1c8 bit 1
    CSuspenderSet Suspenders;                        // +0x1cc (PDB +0x1fc)
    void SetSuspendAnimation(bool suspend, void* suspender);
};

void CThingAnimatable::SetSuspendAnimation(bool suspend, void* suspender)
{
    if (suspend)
        Suspenders.InsertUnique(suspender);
    else
        Suspenders.Erase(suspender);
    AISuspended = !Suspenders.empty();
}
