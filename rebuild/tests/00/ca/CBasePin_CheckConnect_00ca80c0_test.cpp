#include <cstdio>
// CBasePin::CheckConnect: VFW_E_INVALID_DIRECTION (0x80040208) when both pins share a direction, else NOERROR.
struct Peer { int dir; void QueryDirection(int* pd) { *pd = dir; } };
struct Pin {
    int m_dir;
    int CheckConnect(Peer* pPin)
    {
        int pd;
        pPin->QueryDirection(&pd);
        if (pd == m_dir) return (int)0x80040208;
        return 0;
    }
};
int main()
{
    Pin in; in.m_dir = 0;
    Peer out; out.dir = 1;
    Peer in2; in2.dir = 0;
    if (in.CheckConnect(&out) != 0 || in.CheckConnect(&in2) != (int)0x80040208) { std::printf("00ca80c0_TEST FAIL\n"); return 1; }
    std::printf("00ca80c0_TEST PASS\n");
    return 0;
}