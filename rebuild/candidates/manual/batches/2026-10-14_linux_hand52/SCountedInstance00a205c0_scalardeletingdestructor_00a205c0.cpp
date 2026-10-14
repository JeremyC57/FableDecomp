// SCountedInstance_00a205c0::`scalar deleting destructor'(unsigned int flags) -- count this instance out of the global 0x013D27B8, and out of 0x013D27BC / 0x013D27C0 for the flag bytes at +0x14 / +0x15, then free the record for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A205C0, length 57 bytes.
// Leaf: ??_GSCountedInstance_00a205c0@@QAEPAXI@Z
// The chunk label is propagated; named by effect. Shape: `dec dword ptr [global]` three times, the flag bytes in al, `this` in esi, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
extern long g_instances_013d27b8;                 // 0x013D27B8
extern long g_instancesA_013d27bc;                // 0x013D27BC
extern long g_instancesB_013d27c0;                // 0x013D27C0
struct SCountedInstance_00a205c0 {
    unsigned char _pad[0x14];
    bool          m_flagA;                         // +0x14
    bool          m_flagB;                         // +0x15
    ~SCountedInstance_00a205c0()
    {
        --g_instances_013d27b8;
        if (m_flagA)
            --g_instancesA_013d27bc;
        if (m_flagB)
            --g_instancesB_013d27c0;
    }
};

void Kill_00a205c0(SCountedInstance_00a205c0* object)                      // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}
