#include <cstdio>
#include <cstring>
#include <cmath>
class CFractal {
public:
    double ExponentArray[128]; double Lacunarity; double Octaves;
    void NewFractalData(double lacunarity, double fractalIncrement, double octaves)
    {
        Lacunarity = lacunarity;
        Octaves = octaves;
        int exponent = 0;
        for (int i = 0; i < 128; ++i) {
            ExponentArray[i] = std::pow(Lacunarity, exponent * fractalIncrement);
            --exponent;
        }
    }
};

int main()
{
    CFractal f; f.NewFractalData(2.0, 1.0, 6.0);
    if (f.Lacunarity != 2.0 || f.Octaves != 6.0) do { std::printf("00f35da0_TEST FAIL\n"); return 1; } while (0);
    if (f.ExponentArray[0] != 1.0 || f.ExponentArray[1] != 0.5 || f.ExponentArray[3] != 0.125) do { std::printf("00f35da0_TEST FAIL\n"); return 1; } while (0);
    f.NewFractalData(3.0, 0.5, 4.0);
    for (int i = 0; i < 128; ++i)
        if (std::fabs(f.ExponentArray[i] - std::pow(3.0, -0.5 * i)) > 1e-12 * std::pow(3.0, -0.5 * i)) do { std::printf("00f35da0_TEST FAIL\n"); return 1; } while (0);
    std::printf("00f35da0_TEST PASS\n");
    return 0;
}