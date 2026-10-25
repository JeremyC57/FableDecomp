// Behaviour test for 0x00AC6CE0 QuadricOptimiserInternals::CQuadric::Clear().
#include <cstdio>
#include <cstring>

namespace QuadricOptimiserInternals {
class CQuadric {
public:
    double AA, AB, AC, AD;
    double BB, BC, BD;
    double CC, CD;
    double DD;
    double Weight;
    void Clear()
    {
        AA = 0.0; AB = 0.0; AC = 0.0; AD = 0.0; BB = 0.0; BC = 0.0; BD = 0.0; CC = 0.0; CD = 0.0; DD = 0.0; Weight = 0.0;
    }
};
}

int main()
{
    QuadricOptimiserInternals::CQuadric q;
    q.AA = 1.0; q.AB = 2.0; q.AC = 3.0; q.AD = 4.0; q.BB = 5.0; q.BC = 6.0; q.BD = 7.0; q.CC = 8.0; q.CD = 9.0; q.DD = 10.0; q.Weight = 11.0;
    q.Clear();
    if (!(q.AA == 0.0 && q.AB == 0.0 && q.AC == 0.0 && q.AD == 0.0 && q.BB == 0.0 && q.BC == 0.0 && q.BD == 0.0 && q.CC == 0.0 && q.CD == 0.0 && q.DD == 0.0 && q.Weight == 0.0)) {
        std::printf("00ac6ce0_TEST FAIL\n");
        return 1;
    }
    std::printf("00ac6ce0_TEST PASS\n");
    return 0;
}