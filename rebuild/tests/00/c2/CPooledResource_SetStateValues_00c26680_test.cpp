#include <cstdio>

// Behaviour model of CPooledResource::SetStateValues (0x00C26680): two flagged sets on the +0x24 block.
struct State {
    unsigned int flags; unsigned int v2, v3;
    void Set2(unsigned int v) { v2 = v; flags |= 4; }
    void Set3(unsigned int v) { v3 = v; flags |= 8; }
};
struct Resource { State m_state24; void SetStateValues(int, unsigned int a, unsigned int b) { m_state24.Set2(a); m_state24.Set3(b); } };

int main()
{

    Resource r; r.m_state24.flags = 1; r.m_state24.v2 = r.m_state24.v3 = 0;
    r.SetStateValues(0, 11, 22);
    if (r.m_state24.v2 != 11 || r.m_state24.v3 != 22 || r.m_state24.flags != 13) { std::printf("00c26680_TEST FAIL\n"); return 1; }
    std::printf("00c26680_TEST PASS\n");
    return 0;
}