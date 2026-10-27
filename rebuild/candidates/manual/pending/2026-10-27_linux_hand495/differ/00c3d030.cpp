// CAudioSystem::GetChannelCounts(long* counts) -- report four per-group counts: with a device the requested count (+0x6c..) is replaced by the device record's live count (+0x28 + 0x14*i) when positive, without one the requested counts are copied
// Grade: DIFFER(92v94) -- retail keeps counts in eax and the values in edx (counts loaded before the device branch), our build loads counts per arm into edx; and the last device store is tail-duplicated in retail (`jle ; mov ecx,[ecx+64h] ; mov [eax+0Ch],ecx ; ret`), ours merges it
// Leaf: ?GetChannelCounts@CAudioSystem@@QAEXPAJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3D030, 94 bytes, thiscall `ret 4`.
// Identity: reached only through the libpk C-API forwarder 0x00C333F0 (`mov ecx,0143BB58h ; jmp`), the audio-system
//   singleton (CAudioSystem, as the landed SetError 0x00C3D090); +4 is its device pointer. Descriptive (libpk, no PDB/RTTI):
//   the four words are counts the device can override; their meaning is not proven.
// Shape: four explicit `x > 0 ? live : requested` stores per arm; VC tail-duplicates the last store.
struct SChannelGroup_00c3d030 {
    unsigned char _pad[0xc];
    long m_liveCount;                            // +0xc
    unsigned char _pad10[4];
};

class CAudioSystem {
public:
    void* m_0;                                   // +0x00
    void* m_pDevice;                             // +0x04
    unsigned char _pad08[0x1c - 0x08];
    SChannelGroup_00c3d030 m_groups[4];          // +0x1c (live counts +0x28, +0x3c, +0x50, +0x64)
    long m_requested[4];                         // +0x6c
    void GetChannelCounts(long* counts);
};

void CAudioSystem::GetChannelCounts(long* counts)
{
    long count = m_requested[0];
    if (m_pDevice) {
        if (count > 0)
            count = m_groups[0].m_liveCount;
        counts[0] = count;
        count = m_requested[1];
        if (count > 0)
            count = m_groups[1].m_liveCount;
        counts[1] = count;
        count = m_requested[2];
        if (count > 0)
            count = m_groups[2].m_liveCount;
        counts[2] = count;
        count = m_requested[3];
        if (count > 0)
            counts[3] = m_groups[3].m_liveCount;
        else
            counts[3] = count;
    } else {
        counts[0] = count;
        counts[1] = m_requested[1];
        counts[2] = m_requested[2];
        counts[3] = m_requested[3];
    }
}
