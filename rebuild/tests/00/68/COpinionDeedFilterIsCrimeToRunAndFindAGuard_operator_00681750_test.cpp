#include <cstdio>
// Behaviour test for COpinionDeedFilter_IsCrimeToRunAndFindAGuard::operator() @ 0x00681750
struct COpinionDeed {
    long DeedType;
    bool IsCrimeReportedToVillage;
    bool IsCrime;
    long ExtraData;
};
struct COpinionDeedFilter_IsCrimeToRunAndFindAGuard {
    bool operator()(const COpinionDeed& deed) const
    {
        switch (deed.DeedType) {
        case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
            if (deed.IsCrime && !deed.IsCrimeReportedToVillage)
                return true;
            break;
        case 53:
            return deed.ExtraData != -1 ? true : false;
        }
        return false;
    }
};
static bool Check(long type, bool crime, bool reported, long extra)
{
    COpinionDeed d; d.DeedType = type; d.IsCrime = crime; d.IsCrimeReportedToVillage = reported; d.ExtraData = extra;
    COpinionDeedFilter_IsCrimeToRunAndFindAGuard f;
    return f(d);
}
int main()
{
    if (!Check(2, true, false, -1) || !Check(9, true, false, -1) || Check(5, true, true, 0) || Check(5, false, false, 0) ||
        Check(1, true, false, 0) || Check(10, true, false, 0) || !Check(53, false, false, 0) || Check(53, true, false, -1) ||
        Check(54, true, false, 3)) { std::printf("00681750_TEST FAIL\n"); return 1; }
    std::printf("00681750_TEST PASS\n");
    return 0;
}