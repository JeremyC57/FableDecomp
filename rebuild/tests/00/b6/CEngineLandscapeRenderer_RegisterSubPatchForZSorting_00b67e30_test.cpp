// Behaviour test for 0x00B67E30 CEngineLandscapeRenderer::RegisterSubPatchForZSorting(long& index, CLandscapeBackgroundPatch* patch).
#include <stdio.h>
#include <vector>
class CLandscapeBackgroundPatch { public: int Id; };
struct CSubPatchSorterEntry { CLandscapeBackgroundPatch* Patch; unsigned char AddedThisFrame; unsigned char IsOcclusionBox; long OcclusionID; };
class CEngineLandscapeRenderer { public:
    std::vector<CSubPatchSorterEntry> SortedSubPatchList;
    void RegisterSubPatchForZSorting(long& index, CLandscapeBackgroundPatch* patch)
    {
        if (index < 0) {
            index = (long)SortedSubPatchList.size();
            CSubPatchSorterEntry entry;
            entry.Patch = patch; entry.AddedThisFrame = 1; entry.IsOcclusionBox = 0; entry.OcclusionID = -1;
            SortedSubPatchList.push_back(entry);
        } else {
            SortedSubPatchList[index].AddedThisFrame = 1;
        }
    }
};
int main()
{
    CEngineLandscapeRenderer r; CLandscapeBackgroundPatch a = {1}, b = {2};
    long ia = -1, ib = -1;
    r.RegisterSubPatchForZSorting(ia, &a);
    r.RegisterSubPatchForZSorting(ib, &b);
    int ok = ia == 0 && ib == 1 && r.SortedSubPatchList.size() == 2 && r.SortedSubPatchList[1].Patch == &b
          && r.SortedSubPatchList[0].OcclusionID == -1 && r.SortedSubPatchList[0].IsOcclusionBox == 0;
    r.SortedSubPatchList[0].AddedThisFrame = 0;
    r.RegisterSubPatchForZSorting(ia, &a);
    ok = ok && ia == 0 && r.SortedSubPatchList.size() == 2 && r.SortedSubPatchList[0].AddedThisFrame == 1;
    printf(ok ? "00b67e30_TEST PASS\n" : "00b67e30_TEST FAIL\n");
    return ok ? 0 : 1;
}