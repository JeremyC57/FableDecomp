// behaviour test for 0x0040BC80 CUserProfileManager::Apply: ApplyValues always, ApplyVideoValues only when
// ApplyVideoValuesOnLoad is set (links the row's object)
#include <cstdio>
#include <cstring>
class CUserProfileManager {
public:
    unsigned char _pad00[0x33];
    bool          ApplyVideoValuesOnLoad;
    void Apply();
protected:
    void ApplyValues();
    void ApplyVideoValues();
};
static int g_values = 0, g_video = 0;
void CUserProfileManager::ApplyValues() { ++g_values; }
void CUserProfileManager::ApplyVideoValues() { ++g_video; }
int main()
{
    CUserProfileManager a;
    std::memset(&a, 0, sizeof(a));
    a.ApplyVideoValuesOnLoad = true;
    a.Apply();
    if (g_values != 1 || g_video != 1) { std::printf("FAIL branch-taken %d %d\n", g_values, g_video); return 1; }
    a.ApplyVideoValuesOnLoad = false;
    a.Apply();
    if (g_values != 2 || g_video != 1) { std::printf("FAIL branch-skip %d %d\n", g_values, g_video); return 1; }
    std::printf("CUserProfileManager_0040bc80_TEST PASS\n");
    return 0;
}
