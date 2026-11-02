// Behaviour test for 0x0121DF1C, the dynamic initializer of NPlayerGui::VALUE_DISPLAY_DURATION = (float)CMainGameComponent::ConstantFPS.
#include <cstdio>
class CMainGameComponentModel {
protected:
    static long ConstantFPS;
public:
    static long GetConstantFPS() { return ConstantFPS; }
};
long CMainGameComponentModel::ConstantFPS = 30;
namespace NPlayerGuiModel {
float VALUE_DISPLAY_DURATION = (float)CMainGameComponentModel::GetConstantFPS();
}
int main()
{
    float d = NPlayerGuiModel::VALUE_DISPLAY_DURATION - 30.0f;
    if (d > 0.001f || d < -0.001f) { std::printf("0121df1c_TEST FAIL\n"); return 1; }
    std::printf("0121df1c_TEST PASS\n");
    return 0;
}