// Behaviour test for 0x01219F40, the dynamic initializer of GVersion = CVersion(25, 19, 5, 125): the first argument lands in Day (+1),
// the second in Month (+0), then Year and Build.
#include <cstdio>
class CVersionModel {
public:
    CVersionModel(unsigned char day, unsigned char month, unsigned int year, unsigned int build)
        : Month(month), Day(day), Year(year), Build(build) { }
    unsigned char Month;
    unsigned char Day;
    unsigned int  Year;
    unsigned int  Build;
};
CVersionModel GVersion(25, 19, 5, 125);
int main()
{
    if (GVersion.Day != 25 || GVersion.Month != 19 || GVersion.Year != 5 || GVersion.Build != 125) {
        std::printf("01219f40_TEST FAIL\n");
        return 1;
    }
    std::printf("01219f40_TEST PASS\n");
    return 0;
}