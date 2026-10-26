// CUserProfileManager::ConvertFloatToVideoOptionQuality(float) -- map a 0..3 detail slider value to the EEngineVideoOptionQuality step it has reached
// Retail: Fable.exe @ 0x00406820, 90 bytes (+ int3 padding), `ret 4`, `this` unused. No callees.
// Float pool: 0x0122DEDC = 0.0f, 0x0122DED8 = 1.0f, 0x0122DCDC = 2.0f, 0x0122DED4 = 3.0f.
// Shape: four independent `if (value >= K) quality = n;` tests (`test ah,1 ; jne` is `>=`); the first one assigns
// the value the local already holds, so VC keeps its compare but folds the branch to `jne +0`. The last test is
// a select (`mov eax,3 ; je ; mov eax,ecx`).
enum EEngineVideoOptionQuality {
    VIDEO_OPTION_QUALITY_LOW = 0,
    VIDEO_OPTION_QUALITY_MEDIUM = 1,
    VIDEO_OPTION_QUALITY_HIGH = 2,
    VIDEO_OPTION_QUALITY_VERY_HIGH = 3
};

class CUserProfileManager { public:
    EEngineVideoOptionQuality ConvertFloatToVideoOptionQuality(float value);
};

EEngineVideoOptionQuality CUserProfileManager::ConvertFloatToVideoOptionQuality(float value)
{
    EEngineVideoOptionQuality quality = VIDEO_OPTION_QUALITY_LOW;
    if (value >= 0.0f)
        quality = VIDEO_OPTION_QUALITY_LOW;
    if (value >= 1.0f)
        quality = VIDEO_OPTION_QUALITY_MEDIUM;
    if (value >= 2.0f)
        quality = VIDEO_OPTION_QUALITY_HIGH;
    if (value >= 3.0f)
        quality = VIDEO_OPTION_QUALITY_VERY_HIGH;
    return quality;
}