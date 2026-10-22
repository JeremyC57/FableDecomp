// Behaviour test for 00a64440 CMemoryAreaChunk::SetDemandPaged: sets the chunk's DemandPaged flag and leaves its neighbours alone.
#include <cstdio>
struct CMemoryAreaChunk {
    long TotalDataSize;
    bool Permanent;
    bool DemandPaged;
    CMemoryAreaChunk* PNextChunk;
    void SetPermanent() { Permanent = true; }
    void SetDemandPaged() { DemandPaged = true; }
};
int main()
{
    CMemoryAreaChunk c; c.TotalDataSize = 5; c.Permanent = false; c.DemandPaged = false; c.PNextChunk = &c;
    c.SetDemandPaged();
    if (!c.DemandPaged || c.Permanent || c.TotalDataSize != 5 || c.PNextChunk != &c) { std::printf("00a64440_TEST FAIL\n"); return 1; }
    c.SetDemandPaged();
    if (!c.DemandPaged) { std::printf("00a64440_TEST FAIL\n"); return 1; }
    std::printf("00a64440_TEST PASS\n");
    return 0;
}