// behaviour test for 0x0048DB13
#include <stdio.h>
enum ERegionLoadStatus { REGION_LOAD_NONE = 0, REGION_LOAD_FINISHED = 9 };
class CWorld {
public:
    unsigned char     _pad00[0x104];
    ERegionLoadStatus RegionLoadStatus;                  // +0x104
    bool IsPerformingRegionLoad() const;
};
bool CWorld::IsPerformingRegionLoad() const
{
    return RegionLoadStatus != REGION_LOAD_NONE && RegionLoadStatus != REGION_LOAD_FINISHED;
}
int main()
{
    CWorld w; int ok = 1;
    w.RegionLoadStatus = REGION_LOAD_NONE; ok &= !w.IsPerformingRegionLoad();
    w.RegionLoadStatus = REGION_LOAD_FINISHED; ok &= !w.IsPerformingRegionLoad();
    w.RegionLoadStatus = (ERegionLoadStatus)3; ok &= w.IsPerformingRegionLoad();
    w.RegionLoadStatus = (ERegionLoadStatus)1; ok &= w.IsPerformingRegionLoad();
    printf(ok ? "0048db13_TEST PASS\n" : "0048db13_TEST FAIL\n");
    return ok ? 0 : 1;
}