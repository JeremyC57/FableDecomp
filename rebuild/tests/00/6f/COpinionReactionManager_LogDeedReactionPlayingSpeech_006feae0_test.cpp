#include <cstdio>
// Behaviour test for COpinionReactionManager::LogDeedReactionPlayingSpeech @ 0x006FEAE0
class COpinionReactionManager {
public:
    long DeedReactionSpeechPlaying[92];
    void LogDeedReactionPlayingSpeech(long deedType, bool playing)
    {
        if (DeedReactionSpeechPlaying[deedType] != -1)
            DeedReactionSpeechPlaying[deedType] += playing ? 1 : -1;
    }
};
int main()
{
    COpinionReactionManager m;
    for (int i = 0; i < 92; ++i) m.DeedReactionSpeechPlaying[i] = 0;
    m.DeedReactionSpeechPlaying[7] = -1;
    m.LogDeedReactionPlayingSpeech(3, true); m.LogDeedReactionPlayingSpeech(3, true); m.LogDeedReactionPlayingSpeech(3, false);
    m.LogDeedReactionPlayingSpeech(7, true);
    m.LogDeedReactionPlayingSpeech(91, false);
    if (m.DeedReactionSpeechPlaying[3] != 1 || m.DeedReactionSpeechPlaying[7] != -1 || m.DeedReactionSpeechPlaying[91] != -1 ||
        m.DeedReactionSpeechPlaying[4] != 0) { std::printf("006feae0_TEST FAIL\n"); return 1; }
    std::printf("006feae0_TEST PASS\n");
    return 0;
}