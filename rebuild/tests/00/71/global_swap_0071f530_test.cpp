// Behaviour test for 0x0071F530 swap of two pair<CSkeletalMorphs::CEntry,float> entries.
#include <cstdio>
class CDefString { public: long TablePos; };
struct CSkeletalMorphsEntry { CDefString MorphName; short Priority; bool SupressesLowerMorphs; };
struct SMorphWeight {
    CSkeletalMorphsEntry first;
    float second;
    SMorphWeight() { }
    SMorphWeight(const SMorphWeight& other) : first(other.first), second(other.second) { }
};
static void swap_(SMorphWeight& a, SMorphWeight& b)
{
    SMorphWeight tmp = a;
    a = b;
    b = tmp;
}
int main()
{
    SMorphWeight x, y;
    x.first.MorphName.TablePos = 11; x.first.Priority = 3; x.first.SupressesLowerMorphs = true; x.second = 0.25f;
    y.first.MorphName.TablePos = 22; y.first.Priority = -7; y.first.SupressesLowerMorphs = false; y.second = 8.0f;
    swap_(x, y);
    if (x.first.MorphName.TablePos != 22 || x.first.Priority != -7 || x.first.SupressesLowerMorphs || x.second != 8.0f
        || y.first.MorphName.TablePos != 11 || y.first.Priority != 3 || !y.first.SupressesLowerMorphs || y.second != 0.25f) {
        std::printf("0071f530_TEST FAIL\n");
        return 1;
    }
    std::printf("0071f530_TEST PASS\n");
    return 0;
}