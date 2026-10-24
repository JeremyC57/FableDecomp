// Behaviour test for CInputTypeStateRecord::ClearState (0x0066FA30): the seven dwords at
// +0x0C..+0x24 become zero, the header words and anything after +0x28 are left alone.
#include <stdio.h>
#include <stddef.h>

class CInputTypeStateRecord
{
public:
    void ClearState();

    unsigned long m_header[3];
    long m_state0C;
    long m_state10;
    long m_state14;
    long m_state18;
    long m_state1C;
    long m_state20;
    long m_state24;
    long m_after;
};

void CInputTypeStateRecord::ClearState()
{
    m_state20 = 0;
    m_state1C = 0;
    m_state18 = 0;
    m_state0C = 0;
    m_state24 = 0;
    m_state14 = 0;
    m_state10 = 0;
}

int main()
{
    CInputTypeStateRecord r;
    long* words = &r.m_state0C;
    int i;
    if (offsetof(CInputTypeStateRecord, m_state0C) != 0x0C || offsetof(CInputTypeStateRecord, m_state24) != 0x24)
    {
        printf("FAIL layout\n");
        return 1;
    }
    r.m_header[0] = 0x101; r.m_header[1] = 0x202; r.m_header[2] = 0x303;
    for (i = 0; i < 7; ++i) words[i] = 1000 + i;
    r.m_after = -5;
    r.ClearState();
    for (i = 0; i < 7; ++i)
        if (words[i] != 0) { printf("FAIL state word %d\n", i); return 1; }
    if (r.m_header[0] != 0x101 || r.m_header[1] != 0x202 || r.m_header[2] != 0x303 || r.m_after != -5)
    {
        printf("FAIL neighbours\n");
        return 1;
    }
    printf("PASS seven-field zero initializer 0x0066FA30\n");
    return 0;
}
