// Behaviour test for 0x0121F7E0: CTCCreatureModeManager::CModeBase::NO_FRAMES_BEFORE_PLAYING_IDLE_ANIMS = fps * 12.
#include <cstdio>
class CMainGameComponent {
protected:
    static long ConstantFPS;
public:
    static long GetConstantFPS() { return ConstantFPS; }
};
long CMainGameComponent::ConstantFPS = 30;
class CTCCreatureModeManager {
public:
    class CModeBase {
    public:
        static const long NO_FRAMES_BEFORE_PLAYING_IDLE_ANIMS;
    };
};
const long CTCCreatureModeManager::CModeBase::NO_FRAMES_BEFORE_PLAYING_IDLE_ANIMS = CMainGameComponent::GetConstantFPS() * 12;
int main()
{
    if (CTCCreatureModeManager::CModeBase::NO_FRAMES_BEFORE_PLAYING_IDLE_ANIMS != 360) { std::printf("0121f7e0_TEST FAIL\n"); return 1; }
    std::printf("0121f7e0_TEST PASS\n");
    return 0;
}