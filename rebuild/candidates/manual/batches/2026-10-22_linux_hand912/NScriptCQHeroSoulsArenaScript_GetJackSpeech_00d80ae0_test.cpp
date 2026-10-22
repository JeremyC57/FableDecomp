#include <cstdio>
#include <cstring>
// Behaviour test for 00d80ae0: CQ_HeroSoulsArenaScript::GetJackSpeech key selection.
static const char* JackSpeech(long wave)
{
    switch (wave) {
    case 1: return "TEXT_QST_B04_JACK_WAVE_ONE";
    case 2: return "TEXT_QST_B04_JACK_WAVE_TWO";
    case 3: return "TEXT_QST_B04_JACK_WAVE_THREE";
    case 4: return "TEXT_QST_B04_JACK_WAVE_FOUR";
    case 5: return "TEXT_QST_B04_JACK_WAVE_FIVE";
    case 6: return "TEXT_QST_B04_JACK_WAVE_SIX";
    case 7: return "TEXT_QST_B04_JACK_WAVE_SEVEN";
    case 8: return "TEXT_QST_B04_JACK_ARENA_VICTORY";
    }
    return "TEXT_QST_B04_JACK_WAVE_ONE";
}
int main()
{
    int ok = std::strcmp(JackSpeech(1), "TEXT_QST_B04_JACK_WAVE_ONE") == 0
          && std::strcmp(JackSpeech(5), "TEXT_QST_B04_JACK_WAVE_FIVE") == 0
          && std::strcmp(JackSpeech(8), "TEXT_QST_B04_JACK_ARENA_VICTORY") == 0
          && std::strcmp(JackSpeech(0), "TEXT_QST_B04_JACK_WAVE_ONE") == 0
          && std::strcmp(JackSpeech(9), "TEXT_QST_B04_JACK_WAVE_ONE") == 0;
    if (!ok) { std::printf("00d80ae0_TEST FAIL\n"); return 1; }
    std::printf("00d80ae0_TEST PASS\n");
    return 0;
}
