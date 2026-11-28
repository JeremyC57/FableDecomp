#include <cassert>
#include <cstdio>

namespace {
float Percentage(float health, float maximum)
{
    if (maximum > 0.0f)
        return health / maximum;
    return 0.0f;
}
}

int main()
{
    assert(Percentage(1.0f, 4.0f) == 0.25f);
    assert(Percentage(3.0f, 0.0f) == 0.0f);
    assert(Percentage(3.0f, -1.0f) == 0.0f);
    assert(Percentage(0.125f, 0.25f) == 0.5f);
    assert(Percentage(4.0f, 4.0f) == 1.0f);
    assert(Percentage(8.0f, 4.0f) == 2.0f);
    std::printf("006a4e40_TEST PASS\n");
    return 0;
}
