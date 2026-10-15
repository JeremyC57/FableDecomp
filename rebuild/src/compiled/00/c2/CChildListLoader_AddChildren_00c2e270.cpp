// CChildListLoader::AddChildren() -- for every item of the source list (virtual GetCount / GetItem), AddChild(item); stops early as soon as the shared status object reports a failure
// Leaf: ?AddChildren@CChildListLoader
// Retail: Fable.exe @ 0x00C2E270, 60 bytes (+ int3 padding).
// Callees: the source's GetItem(index) (vtable slot 13, +0x34) and GetCount() (slot 15, +0x3c, re-called every pass),
// CChildListLoader::AddChild(item) @ 0x00C2E190 (autonamed "C3DAnimationSequence_ProcessSampleFormatChunk"),
// CLoadStatus::IsOk() @ 0x00C307C0 (`mov al,[ecx+30h] ; ret`, labelled "CTCInventoryBase::GetCanItemBeQuickAssigned").
// Label: the manifest's "CTextContainer::AddChild" is propagated (NUISystem::CTextContainer lives at 0x00546xxx). The only
// caller (0x00C2EB00) runs a sequence of loader steps on the same object, testing the status at +8 after each one, so
// the owner is named for what it does; its class could not be established.
// Shape: a `for` loop over an unsigned index against a re-evaluated virtual count (VC rotates it: the count is tested
// once before the loop and again at the bottom); the early exit is a `return` on the byte flag.
struct CItemSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual void* GetItem(unsigned long index);    // slot 13 (+0x34)
    virtual void v14();
    virtual unsigned long GetCount();              // slot 15 (+0x3c)
};

struct CLoadStatus {
    unsigned char _pad[0x30];
    bool m_ok;                                     // +0x30
    bool IsOk() const;                             // 0x00C307C0
};

struct CChildListLoader {
    CItemSource* m_pSource;                        // +0x00
    void*        m_pTarget;                        // +0x04
    CLoadStatus* m_pStatus;                        // +0x08
    void AddChild(void* item);                     // 0x00C2E190
    void AddChildren();
};

void CChildListLoader::AddChildren()
{
    for (unsigned long i = 0; i < m_pSource->GetCount(); ++i) {
        AddChild(m_pSource->GetItem(i));
        if (!m_pStatus->IsOk())
            return;
    }
}