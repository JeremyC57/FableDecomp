#include <cstdio>
// CBasePin::QueryDirection: E_POINTER for null, else *pPinDir = m_dir.
struct Pin {
    int m_dir;
    int QueryDirection(int* pPinDir)
    {
        if (pPinDir == 0) return (int)0x80004003;
        *pPinDir = m_dir;
        return 0;
    }
};
int main()
{
    Pin p; p.m_dir = 1;
    int d = -1;
    if (p.QueryDirection(0) != (int)0x80004003 || p.QueryDirection(&d) != 0 || d != 1) { std::printf("00ca8490_TEST FAIL\n"); return 1; }
    std::printf("00ca8490_TEST PASS\n");
    return 0;
}