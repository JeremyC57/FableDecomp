// Behaviour test for CQ_ArenaScript::AnimateCrowd @ 0x00F1EC70: each audience member gets the looping animation picked
// by rand() % 6 (0..4 named cases, anything else WAVE_LOOP_01) and then cutscene behaviour 2.
#include <cstdio>
#include <string>
#include <vector>
static const char* Pick(int r)
{
    switch (r % 6) {
    case 0: return "CHEER_LOOP_01";
    case 1: return "CHEER_LOOP_02";
    case 2: return "CLAP_LOOP_02";
    case 3: return "SHOUT_LOOP_01";
    case 4: return "THUMBS_DOWN_LOOP_01";
    default: return "WAVE_LOOP_01";
    }
}
int main()
{
    int rolls[] = { 6, 7, 14, 9, 22, 11 };
    std::vector<std::string> log;
    for (long i = 0; i < 6; ++i) { log.push_back(Pick(rolls[i])); log.push_back("behaviour 2"); }
    bool ok = log.size() == 12 && log[0] == "CHEER_LOOP_01" && log[2] == "CHEER_LOOP_02" && log[4] == "CLAP_LOOP_02" &&
              log[6] == "SHOUT_LOOP_01" && log[8] == "THUMBS_DOWN_LOOP_01" && log[10] == "WAVE_LOOP_01" && log[11] == "behaviour 2";
    if (!ok) { std::printf("00f1ec70_TEST FAIL\n"); return 1; }
    std::printf("00f1ec70_TEST PASS\n");
    return 0;
}
