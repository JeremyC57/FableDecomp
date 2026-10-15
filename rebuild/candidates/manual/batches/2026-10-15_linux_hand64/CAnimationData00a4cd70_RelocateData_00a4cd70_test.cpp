#include <cstdio>
// Behaviour of CAnimationData_00a4cd70::RelocateData (0x00A4CD70): every non-null pointer moves by (newBase - oldBase),
// null pointers stay null.
struct CRecord {
    char* m_pData; unsigned char m_pad04[0x10]; char* m_pBlock14; unsigned char m_pad18[8];
    char* m_pBlock20; char* m_pBlock24; char* m_pBlock28;
    void RelocateData(void* oldBase, void* newBase)
    {
        if (m_pData != 0)    m_pData    = m_pData    - (long)oldBase + (long)newBase;
        if (m_pBlock14 != 0) m_pBlock14 = m_pBlock14 - (long)oldBase + (long)newBase;
        if (m_pBlock20 != 0) m_pBlock20 = m_pBlock20 - (long)oldBase + (long)newBase;
        if (m_pBlock24 != 0) m_pBlock24 = m_pBlock24 - (long)oldBase + (long)newBase;
        if (m_pBlock28 != 0) m_pBlock28 = m_pBlock28 - (long)oldBase + (long)newBase;
    }
};

int main()
{
    static char oldBlock[64], newBlock[64];
    CRecord r;
    r.m_pData = oldBlock + 4; r.m_pBlock14 = 0; r.m_pBlock20 = oldBlock + 16; r.m_pBlock24 = oldBlock; r.m_pBlock28 = 0;
    r.RelocateData(oldBlock, newBlock);
    if (r.m_pData != newBlock + 4 || r.m_pBlock14 != 0 || r.m_pBlock20 != newBlock + 16 || r.m_pBlock24 != newBlock || r.m_pBlock28 != 0) { std::printf("00a4cd70_TEST FAIL\n"); return 1; }
    std::printf("00a4cd70_TEST PASS\n");
    return 0;
}
