// CASoundManager::EnableAtmos() -- store the atmos-enabled flag when it changes and pass it to SetEnableAtmos (slot 2)
// Name (agentSND, 2026-11-10): see NAME_FIXES.tsv / SLOTS.tsv; was CTCDParticleEmitter::func_00991620.
// Leaf: EnableAtmos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00991620, length 24 bytes (32 with padding). 5 callers (0x0049DCD1,
// 0x0062880D, 0x00628DCE, 0x0088F634, 0x0088F650). The catalogue row 00991620 (manifest label EditFrameUpdate, 80 bytes)
// held this body and the vector scale at 0x00991640 (skipped ledger, chunk 901). The PDB CTCDParticleEmitter keeps
// its bools as bitfields at +0x44, so this is another class in the same region; `this` is modelled locally.
// Shape: the argument loaded as a dword (`mov eax,[esp+4]`), compared as a byte against +0x26, stored, written back
// to its own slot and passed on by the tail `jmp [edx+8]`.
struct CASoundManager {
    virtual void v00();
    virtual void v01();
    virtual void SetEnableAtmos(bool enabled);  // slot 2 (+0x08)
    unsigned char _pad_0x04[0x26 - 0x04];
    bool          AtmosEnabled;                    // +0x26
    void EnableAtmos(bool enabled);
};

void CASoundManager::EnableAtmos(bool enabled)
{
    if (AtmosEnabled != enabled) {
        AtmosEnabled = enabled;
        SetEnableAtmos(enabled);
    }
}