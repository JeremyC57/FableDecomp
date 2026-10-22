// Behaviour test for 00a64420 CMemoryAreaChunk::SetPermanent: sets the chunk's Permanent flag and leaves its neighbours alone.
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
    c.SetPermanent();
    if (!c.Permanent || c.DemandPaged || c.TotalDataSize != 5 || c.PNextChunk != &c) { std::printf("00a64420_TEST FAIL\n"); return 1; }
    c.SetPermanent();
    if (!c.Permanent) { std::printf("00a64420_TEST FAIL\n"); return 1; }
    std::printf("00a64420_TEST PASS\n");
    return 0;
}
