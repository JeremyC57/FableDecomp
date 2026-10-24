// Behaviour test for 0x00A78680 CNavRegion::CNavRegion(long, long): packed RegionID, an empty connection list whose
// sentinel links to itself, MostRecentSearchID zero.
#include <cstdio>
#include <cstdlib>
struct SNode { SNode* next; SNode* prev; void* data; void* info; };
struct CNavRegionModel {
    long RegionID;
    SNode* Connections;
    long MostRecentSearchID;
    CNavRegionModel(long mapIndex, long internalIndex)
    {
        Connections = 0;
        SNode* n = (SNode*)std::malloc(sizeof(SNode));
        n->next = n;
        n->prev = n;
        Connections = n;
        RegionID = (mapIndex << 16) ^ (internalIndex & 0xFFFF);
        MostRecentSearchID = 0;
    }
};
int main()
{
    CNavRegionModel r(3, 0x12345);
    if (r.RegionID != 0x32345 || r.MostRecentSearchID != 0 || r.Connections == 0 ||
        r.Connections->next != r.Connections || r.Connections->prev != r.Connections) {
        std::printf("00a78680_TEST FAIL\n"); return 1;
    }
    CNavRegionModel q(0x7FFF, -1);
    if (q.RegionID != (long)0x7FFFFFFF) { std::printf("00a78680_TEST FAIL\n"); return 1; }
    std::free(r.Connections); std::free(q.Connections);
    std::printf("00a78680_TEST PASS\n");
    return 0;
}