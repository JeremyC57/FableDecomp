// Behaviour test for CLandscapeTesselatorBase::AddPoly (0x00BE42D0): the three indices are appended, in order, to the bridge
// index buffer as 16-bit values (the long arguments narrow), growing the buffer when it is full.
#include <cstdio>
#include <vector>
struct CLandscapeTesselatorBase {
    std::vector<unsigned short> BridgeIndexBuffer;
    void AddPoly(long a, long b, long c)
    {
        BridgeIndexBuffer.push_back((unsigned short)a);
        BridgeIndexBuffer.push_back((unsigned short)b);
        BridgeIndexBuffer.push_back((unsigned short)c);
    }
};
int main()
{
    CLandscapeTesselatorBase t;
    t.AddPoly(1, 2, 3);
    t.AddPoly(0x10004, 65535, 7);
    bool ok = t.BridgeIndexBuffer.size() == 6 && t.BridgeIndexBuffer[0] == 1 && t.BridgeIndexBuffer[2] == 3;
    ok = ok && t.BridgeIndexBuffer[3] == 4 && t.BridgeIndexBuffer[4] == 65535 && t.BridgeIndexBuffer[5] == 7;
    if (!ok) { std::printf("00be42d0_TEST FAIL\n"); return 1; }
    std::printf("00be42d0_TEST PASS\n");
    return 0;
}