// CTCDParticleEmitter::func_00991620(bool enabled) -- when the bool at +0x26 changes, store it and notify through virtual slot 2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00991620, length 24 bytes (32 with padding). 5 callers (0x0049DCD1,
// 0x0062880D, 0x00628DCE, 0x0088F634, 0x0088F650). The catalogue row 00991620 (manifest label EditFrameUpdate, 80 bytes)
// held this body and the vector scale at 0x00991640 (skipped ledger, chunk 901). The PDB CTCDParticleEmitter keeps
// its bools as bitfields at +0x44, so this is another class in the same region; `this` is modelled locally.
// Shape: the argument loaded as a dword (`mov eax,[esp+4]`), compared as a byte against +0x26, stored, written back
// to its own slot and passed on by the tail `jmp [edx+8]`.
struct CFlaggedObject_Retail {
    virtual void v00();
    virtual void v01();
    virtual void OnFlagChanged(bool enabled);  // slot 2 (+0x08)
    unsigned char _pad_0x04[0x26 - 0x04];
    bool          m_flag26;                    // +0x26
    void func_00991620(bool enabled);
};

void CFlaggedObject_Retail::func_00991620(bool enabled)
{
    if (m_flag26 != enabled) {
        m_flag26 = enabled;
        OnFlagChanged(enabled);
    }
}