#include <cstdio>
#include <cstring>
#include <cmath>
class CMatrix4x4 {
public:
    float M[4][4];
    CMatrix4x4(int) { SetIdentity(); }
    void SetIdentity() { for (int j = 0; j < 4; ++j) for (int i = 0; i < 4; ++i) M[i][j] = (i == j) ? 1.0f : 0.0f; }
};

int main()
{
    CMatrix4x4 m(0);
    for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) if (m.M[i][j] != (i == j ? 1.0f : 0.0f)) do { std::printf("01224340_TEST FAIL\n"); return 1; } while (0);
    std::printf("01224340_TEST PASS\n");
    return 0;
}