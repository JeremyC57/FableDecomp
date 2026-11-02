// Behaviour test for 0x0121F220: VALUE_DISPLAY_DURATION = 0.5f * CMainGameComponent::GetConstantFPS() (half a second in frames).
#include <cstdio>
class CMainGameComponent {
protected:
    static long ConstantFPS;
public:
    static long GetConstantFPS() { return ConstantFPS; }
};
long CMainGameComponent::ConstantFPS = 30;               // statically initialised before any dynamic initializer
namespace {
const float VALUE_DISPLAY_DURATION = 0.5f * CMainGameComponent::GetConstantFPS();
}
int main()
{
    float d = VALUE_DISPLAY_DURATION - 15.0f;
    if (d > 0.0001f || d < -0.0001f || (long)VALUE_DISPLAY_DURATION != 15) { std::printf("0121f220_TEST FAIL\n"); return 1; }
    std::printf("0121f220_TEST PASS\n");
    return 0;
}