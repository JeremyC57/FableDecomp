// CParticleSimulation::RelocateData(long, void* oldBase, void* newBase, long) -- rebases the five particle array pointers (+0x1c..+0x2c) from oldBase to newBase, leaving null pointers alone
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADFC60, 83 bytes (`ret 10h`: four stack arguments, the first and last unused).
// Callees: none. Shape: the same four-argument signature as the landed CParticleProgram::RelocateData (0x00AE04C0):
// argument 2 is the old base (subtracted, kept in esi) and argument 3 the new base (added, edx). The five arrays
// are the ones CParticleSimulation::AllocateArrays (0x00ADF2A0) carves out of one block: +0x1c, +0x20, +0x24, +0x28,
// +0x2c. Each is `p = p - old + new` on a byte pointer, guarded by a null test.
class CParticleSimulation { public:
    unsigned char m_pad00[0x1c];
    char* m_pPositions;                              // +0x1c
    char* m_pVelocities;                             // +0x20
    char* m_pColours;                                // +0x24
    char* m_pSizes;                                  // +0x28
    char* m_pAges;                                   // +0x2c
    void RelocateData(long unused, void* oldBase, void* newBase, long unused2);
};

void CParticleSimulation::RelocateData(long unused, void* oldBase, void* newBase, long unused2)
{
    if (m_pPositions != 0)  m_pPositions  = m_pPositions  - (long)oldBase + (long)newBase;
    if (m_pVelocities != 0) m_pVelocities = m_pVelocities - (long)oldBase + (long)newBase;
    if (m_pColours != 0)    m_pColours    = m_pColours    - (long)oldBase + (long)newBase;
    if (m_pSizes != 0)      m_pSizes      = m_pSizes      - (long)oldBase + (long)newBase;
    if (m_pAges != 0)       m_pAges       = m_pAges       - (long)oldBase + (long)newBase;
}