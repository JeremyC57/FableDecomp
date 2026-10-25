// _global::AudioSystem_GetDeviceInfo(unsigned long* pFirst, unsigned long* pSecond) -- libpk audio API: fills each non-null out word from the global audio system's device (its +0x188 record's +4 word, 0x80000000 when absent; and the constant-0 second query)
// Leaf: AudioSystem_GetDeviceInfo
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C33590, 47 bytes, __stdcall `ret 8`. Descriptive name (no PDB covers libpk):
//   the catalog's `WordWrap_SetCallback` is propagated. Caller: the libpk front end 0x00C1C733.
// Callees: the device's getters @ 0x00C3EA60 (`m_188 ? m_188->m_4 : 0x80000000`) and @ 0x00C3EA80 (`return 0`), both
//   __fastcall; the device pointer is the global audio system's member at +4 (0x0143BB58 + 4 = 0x0143BB5C), re-read
//   for each call.
// Shape: each out pointer is loaded into esi, tested, and written with the getter's eax.
struct CAudioDevice {
    unsigned long GetFirstInfo();     // 0x00C3EA60
    unsigned long GetSecondInfo();    // 0x00C3EA80
};
struct CAudioSystem {
    void*         m_vtable;           // +0x00 (not used here)
    CAudioDevice* m_pDevice;          // +0x04
};
extern CAudioSystem g_audioSystem;    // 0x0143BB58

void __stdcall AudioSystem_GetDeviceInfo(unsigned long* pFirst, unsigned long* pSecond)
{
    if (pFirst)
        *pFirst = g_audioSystem.m_pDevice->GetFirstInfo();
    if (pSecond)
        *pSecond = g_audioSystem.m_pDevice->GetSecondInfo();
}