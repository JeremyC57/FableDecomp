// _global::AudioSystem_SetListenerTarget(void* target) -- libpk audio API: hands the target to the device's optional +0x1D4 sink (virtual slot 7) and, when the device's +0x180 flag is set, stores it in the +0x58 helper's +0x1C word and refreshes that helper (virtual slot 1)
// Leaf: AudioSystem_SetListenerTarget
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C336F0, 57 bytes, __stdcall `ret 4`. Descriptive name (no PDB covers libpk):
//   the catalog's `Constraint_NotifyGlobalManager` is propagated. Caller: the landed
//   CConstraintOwner_00c1bed0::AddConstraintRepeat 0x00C1BED0.
// Shape: the work is an inline member of the device (global audio system +4 = 0x0143BB5C): the device arrives in eax
//   and is copied to esi, the sink is tested in ecx and called through `mov eax,ecx`; the helper is a named local in
//   the inline member (read once although the target store could alias it). A named device/sink local in the API
//   function itself puts the device straight into esi and calls the sink through ecx (4 grades).
struct CAudioSink {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06();
    virtual void SetTarget(void* target);     // slot 7 (+0x1C)
};
struct CAudioHelper {
    virtual void v00();
    virtual void Refresh();                   // slot 1 (+0x04)
    unsigned char _pad04[0x18];
    void*         m_pTarget;                  // +0x1C
};
struct CAudioDevice {
    unsigned char _pad000[0x58];
    CAudioHelper* m_pHelper;                  // +0x58
    unsigned char _pad05c[0x180 - 0x5c];
    long          m_helperActive;             // +0x180
    unsigned char _pad184[0x1d4 - 0x184];
    CAudioSink*   m_pSink;                    // +0x1D4
    void SetListenerTarget(void* target)
    {
        if (m_pSink)
            m_pSink->SetTarget(target);
        if (m_helperActive) {
            CAudioHelper* helper = m_pHelper;
            helper->m_pTarget = target;
            helper->Refresh();
        }
    }
};
struct CAudioSystem {
    void*         m_vtable;                   // +0x00 (not used here)
    CAudioDevice* m_pDevice;                  // +0x04
};
extern CAudioSystem g_audioSystem;            // 0x0143BB58

void __stdcall AudioSystem_SetListenerTarget(void* target)
{
    g_audioSystem.m_pDevice->SetListenerTarget(target);
}