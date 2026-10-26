// Behaviour test for CUserProfileManager::ConvertFloatToVideoOptionQuality (0x00406820).
#include <cstdio>

enum EEngineVideoOptionQuality { Q_LOW = 0, Q_MEDIUM = 1, Q_HIGH = 2, Q_VERY_HIGH = 3 };

class CUserProfileManager { public:
    EEngineVideoOptionQuality ConvertFloatToVideoOptionQuality(float value);
};

EEngineVideoOptionQuality CUserProfileManager::ConvertFloatToVideoOptionQuality(float value)
{
    EEngineVideoOptionQuality quality = Q_LOW;
    if (value >= 0.0f)
        quality = Q_LOW;
    if (value >= 1.0f)
        quality = Q_MEDIUM;
    if (value >= 2.0f)
        quality = Q_HIGH;
    if (value >= 3.0f)
        quality = Q_VERY_HIGH;
    return quality;
}

int main()
{
    CUserProfileManager m;
    float in[] = { -1.0f, 0.0f, 0.5f, 1.0f, 1.99f, 2.0f, 2.5f, 3.0f, 10.0f };
    int want[] = { 0, 0, 0, 1, 1, 2, 2, 3, 3 };
    for (int i = 0; i < 9; ++i) {
        if (m.ConvertFloatToVideoOptionQuality(in[i]) != want[i]) {
            std::printf("00406820_TEST FAIL\n");
            return 1;
        }
    }
    std::printf("00406820_TEST PASS\n");
    return 0;
}