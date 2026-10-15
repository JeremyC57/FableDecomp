// CAnimationData_00a4cd70::RelocateData(void* oldBase, void* newBase) -- rebases the five data pointers at +0x00/+0x14/+0x20/+0x24/+0x28 from oldBase to newBase, leaving null pointers alone
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4CD70, 81 bytes (`ret 8`: two stack arguments).
// Callees: none. Shape: argument 1 is the old base (subtracted, esi), argument 2 the new base (added, edx); each
// pointer is `p = p - old + new` on a byte pointer under a null test, the same idiom as the landed
// CParticleProgram::RelocateData (0x00AE04C0).
// Label: "CParticleSimulation::RelocateData" is propagated: the offsets do not fit the CParticleSimulation arrays
// (+0x1c..+0x2c, see 0x00ADFC60) and the only caller is C3DAnimation2::RelocateData (0x00A4E960), which runs it on
// the record its +0x1c points to (when non-null) before rebasing its own pointers. The record's class is not
// established, so it is named by address and the members by role.
struct CAnimationData_00a4cd70 {
    char* m_pData;                                   // +0x00
    unsigned char m_pad04[0x10];
    char* m_pBlock14;                                // +0x14
    unsigned char m_pad18[0x08];
    char* m_pBlock20;                                // +0x20
    char* m_pBlock24;                                // +0x24
    char* m_pBlock28;                                // +0x28
    void RelocateData(void* oldBase, void* newBase);
};

void CAnimationData_00a4cd70::RelocateData(void* oldBase, void* newBase)
{
    if (m_pData != 0)    m_pData    = m_pData    - (long)oldBase + (long)newBase;
    if (m_pBlock14 != 0) m_pBlock14 = m_pBlock14 - (long)oldBase + (long)newBase;
    if (m_pBlock20 != 0) m_pBlock20 = m_pBlock20 - (long)oldBase + (long)newBase;
    if (m_pBlock24 != 0) m_pBlock24 = m_pBlock24 - (long)oldBase + (long)newBase;
    if (m_pBlock28 != 0) m_pBlock28 = m_pBlock28 - (long)oldBase + (long)newBase;
}
