// Test for 0x0041C930 CSoundInit::CSoundInit(): every field gets its default.
#include <stdio.h>
#include <string.h>
enum ESoundPlaybackType { SOUND_PLAYBACK_1 = 1 };   // Ego_r.pdb enum (only the value used here is named)

class CSoundInit {
public:
    float              Volume;                  // +0x00
    float              Pitch;                   // +0x04
    float              Pan;                     // +0x08
    long               NoPlays;                 // +0x0C
    ESoundPlaybackType PlaybackType;            // +0x10
    unsigned long      GainFaderBits;           // +0x14
    unsigned long      PitchFaderBits;          // +0x18
    long               OverrideID;              // +0x1C
    CSoundInit();
};

inline void* operator new(size_t, void* p) { return p; }
int main()
{
    if (sizeof(CSoundInit) != 0x20) return 1;
    unsigned char raw[0x20];
    memset(raw, 0xCC, sizeof raw);
    CSoundInit* s = new (raw) CSoundInit;
    if (s->Volume != 1.0f || s->Pitch != 1.0f || s->Pan != 0.0f || s->NoPlays != 1) return 2;
    if (s->PlaybackType != SOUND_PLAYBACK_1 || s->GainFaderBits != 0 || s->PitchFaderBits != 0 || s->OverrideID != -1) return 3;
    puts("0041c930_TEST PASS");
    return 0;
}
