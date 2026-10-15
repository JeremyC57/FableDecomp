#include <cstdio>
// Behaviour of CParticleSimulation::RelocateData (0x00ADFC60): the five particle arrays move from the old to the
// new block (arguments 2 and 3), null arrays stay null, arguments 1 and 4 are ignored.
struct CParticleSimulation {
    unsigned char m_pad00[0x1c];
    char* m_pPositions; char* m_pVelocities; char* m_pColours; char* m_pSizes; char* m_pAges;
    void RelocateData(long, void* oldBase, void* newBase, long)
    {
        if (m_pPositions != 0)  m_pPositions  = m_pPositions  - (long)oldBase + (long)newBase;
        if (m_pVelocities != 0) m_pVelocities = m_pVelocities - (long)oldBase + (long)newBase;
        if (m_pColours != 0)    m_pColours    = m_pColours    - (long)oldBase + (long)newBase;
        if (m_pSizes != 0)      m_pSizes      = m_pSizes      - (long)oldBase + (long)newBase;
        if (m_pAges != 0)       m_pAges       = m_pAges       - (long)oldBase + (long)newBase;
    }
};

int main()
{
    static char oldBlock[256], newBlock[256];
    CParticleSimulation s;
    s.m_pPositions = oldBlock; s.m_pVelocities = oldBlock + 32; s.m_pColours = 0; s.m_pSizes = oldBlock + 96; s.m_pAges = oldBlock + 128;
    s.RelocateData(7, oldBlock, newBlock, 9);
    if (s.m_pPositions != newBlock || s.m_pVelocities != newBlock + 32 || s.m_pColours != 0 || s.m_pSizes != newBlock + 96 || s.m_pAges != newBlock + 128) { std::printf("00adfc60_TEST FAIL\n"); return 1; }
    std::printf("00adfc60_TEST PASS\n");
    return 0;
}
