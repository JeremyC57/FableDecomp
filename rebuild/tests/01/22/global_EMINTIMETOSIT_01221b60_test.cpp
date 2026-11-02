// Behaviour test for 0x01221B60: MIN_TIME_TO_SIT = fps * 20 (twenty seconds in frames).
#include <cstdio>
class CMainGameComponent {
protected:
    static long ConstantFPS;
public:
    static long GetConstantFPS() { return ConstantFPS; }
};
long CMainGameComponent::ConstantFPS = 30;
namespace {
const long MIN_TIME_TO_SIT = CMainGameComponent::GetConstantFPS() * 20;
}
int main()
{
    if (MIN_TIME_TO_SIT != 600) { std::printf("01221b60_TEST FAIL\n"); return 1; }
    std::printf("01221b60_TEST PASS\n");
    return 0;
}