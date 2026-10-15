// CTCInventoryExperience::ProcessButtonXHeld() -- base handler, then drive the held "remove" spend event
// Retail 0x0057469F, 88 bytes (vtable 0x0124F108 slot 56; the manifest label ProcessButtonAHeld is
// propagated from slot 54 = 0x00574647, the "add" twin -- named here after the slot order, see 0x0056E60F).
// After the base CTCInventoryBase handler (0x005BC68E, a qualified non-virtual call), and only while
// InventorySelectionState (+0x1C) is 1: an enabled ExperienceSpendRemove event (+0x1D0) tail-calls the
// spend step 0x0057463B (`ExperienceSpendMagnitude = -1; ProcessExperienceAction();`); otherwise the
// first press marks the event initiated (+0x1D1) and arms its delay: time (+0x1D4) =
// def->ExperienceSpendTimeOffset (+0x554 of the CInventoryDef held by PDef, +0x68) * the global
// at 0x01375550, read into a local BEFORE the def copy (retail loads it ahead of the add-ref). The def is reached through a local CDefPointer copy (inline add-ref under a null
// test, `if (--refCount == 0) Destroy()` through vtable slot 1 with no null test, tail-jumped).
// Retail offsets (the PDB header is 0x44 bytes wider ahead of the events), local model.
// Leaf: ProcessButtonXHeld
#pragma optimize("s", on)

struct ExperienceSpendEvent {
    bool enable;            // +0x0
    bool initiate;          // +0x1
    unsigned char _pad[2];
    long time;              // +0x4
};

struct CInventoryDef {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long refCount;                                 // +0x004
    unsigned char _pad_008[0x54C];
    long ExperienceSpendTimeOffset;                // +0x554 (PDB +0x570)
    void AddRef() { ++refCount; }
    void Release() { if (--refCount == 0) Destroy(); }
};

struct CInventoryDefPointer {
    CInventoryDef* m_p;
    CInventoryDefPointer(const CInventoryDefPointer& other) : m_p(other.m_p) { if (m_p) m_p->AddRef(); }
    ~CInventoryDefPointer() { m_p->Release(); }
    CInventoryDef* operator->() const { return m_p; }
};

extern long g_ExperienceSpendTimeScale;            // 0x01375550

struct CTCInventoryBase {
    void ProcessButtonXHeld();                     // 0x005BC68E
};

struct CTCInventoryExperience : CTCInventoryBase {
    unsigned char _pad_000[0x1C];
    long InventorySelectionState;                  // +0x01C
    unsigned char _pad_020[0x48];
    CInventoryDefPointer PDef;                     // +0x068
    unsigned char _pad_06c[0x15C];
    ExperienceSpendEvent ExperienceSpendAdd;       // +0x1C8
    ExperienceSpendEvent ExperienceSpendRemove;    // +0x1D0
    void RemoveExperienceSpend();                  // 0x0057463B
    void ProcessButtonXHeld();
};

void CTCInventoryExperience::ProcessButtonXHeld()
{
    CTCInventoryBase::ProcessButtonXHeld();
    if (InventorySelectionState == 1) {
        if (ExperienceSpendRemove.enable) {
            RemoveExperienceSpend();
        } else if (!ExperienceSpendRemove.initiate) {
            ExperienceSpendRemove.initiate = true;
            long scale = g_ExperienceSpendTimeScale;
            CInventoryDefPointer def(PDef);
            ExperienceSpendRemove.time = def->ExperienceSpendTimeOffset * scale;
        }
    }
}
