// Behaviour test for 0x00766550 CTCObjectAugmentations::ClearAllSounds().
#include <cstdio>
#include <map>

struct SoundSet { bool Played; int Sound; unsigned int Handle; };

struct SoundPlayer {
    unsigned int stopped[8];
    int count;
    SoundPlayer() : count(0) { }
    void StopSample(unsigned int h) { if (count < 8) stopped[count] = h; ++count; }
};

struct Thing {
    unsigned int presentBits[9];
    SoundPlayer* player;
    bool GetPlayer(SoundPlayer** out)
    {
        const int type = 0x10;
        if (presentBits[type >> 5] & (1u << (type & 31))) { *out = player; return true; }
        return false;
    }
};

struct Augs {
    Thing* PThing;
    std::map<int, SoundSet> InitSounds;
    std::map<int, SoundSet> LoopingSounds;
    void ClearAllSounds()
    {
        for (std::map<int, SoundSet>::iterator it = LoopingSounds.begin(); it != LoopingSounds.end(); ++it) {
            if (it->second.Played) {
                SoundPlayer* p;
                if (PThing->GetPlayer(&p)) p->StopSample(it->second.Handle);
            }
        }
        LoopingSounds.clear();
        InitSounds.clear();
    }
};

static SoundSet mk(bool played, unsigned int h) { SoundSet s; s.Played = played; s.Sound = 0; s.Handle = h; return s; }

int main()
{
    int ok = 1;
    if ((0x10 >> 5) != 0 || (1u << (0x10 & 31)) != 0x10000u) ok = 0;   // bitset word +0x20, bit 16
    SoundPlayer player;
    Thing t;
    for (int i = 0; i < 9; ++i) t.presentBits[i] = 0;
    t.player = &player;
    Augs a;
    a.PThing = &t;
    a.LoopingSounds[3] = mk(true, 30);
    a.LoopingSounds[1] = mk(false, 10);
    a.LoopingSounds[2] = mk(true, 20);
    a.InitSounds[5] = mk(true, 50);
    a.ClearAllSounds();                                     // no sound player: nothing stopped, maps cleared
    if (player.count != 0 || !a.LoopingSounds.empty() || !a.InitSounds.empty()) ok = 0;
    t.presentBits[0] = 0x10000u;
    a.LoopingSounds[3] = mk(true, 30);
    a.LoopingSounds[1] = mk(false, 10);
    a.LoopingSounds[2] = mk(true, 20);
    a.InitSounds[5] = mk(true, 50);
    a.ClearAllSounds();                                     // played looping sounds stop in key order
    if (player.count != 2 || player.stopped[0] != 20 || player.stopped[1] != 30) ok = 0;
    if (!a.LoopingSounds.empty() || !a.InitSounds.empty()) ok = 0;
    if (!ok) { std::printf("00766550_TEST FAIL\n"); return 1; }
    std::printf("00766550_TEST PASS\n");
    return 0;
}