#include <cstdio>

// Model: a thing with an interface bitset + sorted {type, component} map, a sound player that records stops and the
// lightning power's looping-sound handle.
class CTCBase;

unsigned long g_InvalidSoundHandle = 0xffffffffU;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type)
            ++it;
        return it;
    }
};

class CThing {
public:
    unsigned int  m_presentBits[9];
    CInterfaceMap m_interfaces;
    template <class T> bool GetInterface(T** out, long wanted)
    {
        if (m_presentBits[wanted >> 5] & (1U << (wanted & 31))) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

class CTCSoundPlayer {
public:
    int           Stops;
    unsigned long LastStopped;
    void StopSample(unsigned long sampleID) { ++Stops; LastStopped = sampleID; }
};

class CTCWillLightning {
public:
    CThing*       ParentThing;
    unsigned long LoopingSound;
    void StopLoopingSound();
};

void CTCWillLightning::StopLoopingSound()
{
    unsigned long sound = LoopingSound;
    if (sound != g_InvalidSoundHandle) {
        CTCSoundPlayer* player;
        if (ParentThing->GetInterface(&player, 0x10))
            player->StopSample(sound);
        LoopingSound = g_InvalidSoundHandle;
    }
}

int main()
{
    CTCSoundPlayer player;
    player.Stops = 0;
    player.LastStopped = 0;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x10; entries[0].component = (CTCBase*)&player;
    entries[1].type = 0xa7; entries[1].component = 0;

    CThing hero;
    for (int i = 0; i < 9; ++i)
        hero.m_presentBits[i] = 0;
    hero.m_interfaces.first = entries;
    hero.m_interfaces.last = entries + 2;

    CTCWillLightning power;
    power.ParentThing = &hero;

    bool ok = true;
    power.LoopingSound = g_InvalidSoundHandle;           // nothing playing: no stop
    hero.m_presentBits[0] |= 1U << 0x10;
    power.StopLoopingSound();
    ok = ok && player.Stops == 0 && power.LoopingSound == g_InvalidSoundHandle;

    power.LoopingSound = 42;                             // playing, sound player present: stopped and forgotten
    power.StopLoopingSound();
    ok = ok && player.Stops == 1 && player.LastStopped == 42 && power.LoopingSound == g_InvalidSoundHandle;

    hero.m_presentBits[0] = 0;                           // playing, no sound player: just forgotten
    power.LoopingSound = 7;
    power.StopLoopingSound();
    ok = ok && player.Stops == 1 && power.LoopingSound == g_InvalidSoundHandle;

    if (!ok) {
        std::printf("007a0490_TEST FAIL\n");
        return 1;
    }
    std::printf("007a0490_TEST PASS\n");
    return 0;
}