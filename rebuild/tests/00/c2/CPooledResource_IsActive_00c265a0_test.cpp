#include <cstdio>

// Behaviour model of CPooledResource::IsActive (0x00C265A0): state word == 1.
struct CPooledResource {
    short m_state;
    long IsActive() { return m_state == 1; }
};

int main()
{

    CPooledResource r;
    r.m_state = 1; if (r.IsActive() != 1) { std::printf("00c265a0_TEST FAIL\n"); return 1; }
    r.m_state = 2; if (r.IsActive() != 0) { std::printf("00c265a0_TEST FAIL\n"); return 1; }
    r.m_state = 0; if (r.IsActive() != 0) { std::printf("00c265a0_TEST FAIL\n"); return 1; }
    std::printf("00c265a0_TEST PASS\n");
    return 0;
}