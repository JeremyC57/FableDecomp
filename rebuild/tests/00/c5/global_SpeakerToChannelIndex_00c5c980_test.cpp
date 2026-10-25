// Behaviour test for SpeakerToChannelIndex (0x00C5C980): the six SPEAKER_* bits map to L, R, BL, BR, C, LFE slots.
#include <cstdio>
static long SpeakerToChannelIndex(unsigned int speaker)
{
    switch (speaker) {
    case 0x01: return 0;
    case 0x02: return 1;
    case 0x10: return 2;
    case 0x20: return 3;
    case 0x04: return 4;
    case 0x08: return 5;
    }
    return -1;
}
int main()
{
    int ok = SpeakerToChannelIndex(1) == 0 && SpeakerToChannelIndex(2) == 1 && SpeakerToChannelIndex(4) == 4
          && SpeakerToChannelIndex(8) == 5 && SpeakerToChannelIndex(0x10) == 2 && SpeakerToChannelIndex(0x20) == 3
          && SpeakerToChannelIndex(0) == -1 && SpeakerToChannelIndex(3) == -1 && SpeakerToChannelIndex(0x40) == -1
          && SpeakerToChannelIndex(0x21) == -1;
    if (!ok) { std::printf("00c5c980_TEST FAIL\n"); return 1; }
    std::printf("00c5c980_TEST PASS\n");
    return 0;
}