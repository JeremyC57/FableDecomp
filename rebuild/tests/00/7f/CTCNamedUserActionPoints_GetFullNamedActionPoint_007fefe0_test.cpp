// Behaviour test for CTCNamedUserActionPoints::GetFullNamedActionPoint (0x007FEFE0), g++ model of the retail logic.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
struct Points {
    std::vector<long> FreeActionPointIndexes;
    long NumberOfActionPoints;
    std::vector<std::string> Names;
    const void* PThing;
    bool GetFullNamedActionPoint(const void* thing, std::string name, long& index, long start)
    {
        PThing = thing;
        for (long i = start; i < NumberOfActionPoints; ++i) {
            if (std::strstr(Names[i].c_str(), name.c_str())
                && std::find(FreeActionPointIndexes.begin(), FreeActionPointIndexes.end(), i) == FreeActionPointIndexes.end()) {
                index = i;
                return true;
            }
        }
        return false;
    }
};
int main()
{
    Points p;
    p.NumberOfActionPoints = 4;
    p.Names.push_back("SIT_BENCH"); p.Names.push_back("SIT_CHAIR"); p.Names.push_back("STAND"); p.Names.push_back("SIT_STOOL");
    p.FreeActionPointIndexes.push_back(0); p.FreeActionPointIndexes.push_back(3);
    int thing = 0; long index = -1;
    bool a = p.GetFullNamedActionPoint(&thing, "SIT", index, 0);      // 0 free, 1 in use -> 1
    int ok = a && index == 1 && p.PThing == &thing;
    long index2 = -1;
    bool b = p.GetFullNamedActionPoint(&thing, "SIT", index2, 2);     // 2 no match, 3 free -> none
    ok = ok && !b && index2 == -1;
    bool c = p.GetFullNamedActionPoint(&thing, "", index2, 2);        // empty name matches 2 (in use)
    ok = ok && c && index2 == 2;
    std::printf(ok ? "007fefe0_TEST PASS\n" : "007fefe0_TEST FAIL\n");
    return ok ? 0 : 1;
}