// Behaviour test for 0079de00 PlaySoundW: with an interface-16 component the sound is played through it (sound, 0, flags, 0, -1); without one the invalid handle comes back.
#include <cstdio>

struct CTCBase { long tag; };

static unsigned long g_lastSound, g_lastFlags; static long g_lastA, g_lastB, g_lastC;

struct CTCSound : CTCBase
{
    unsigned long PlaySound(unsigned long sound, long a, unsigned long flags, long b, long c)
    {
        g_lastSound = sound; g_lastA = a; g_lastFlags = flags; g_lastB = b; g_lastC = c;
        return 1234;
    }
};

struct CInterfaceMap
{
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};

struct CThing
{
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;

    template <class T> bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
        {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

static unsigned long g_InvalidSoundHandle = 0xFFFFFFFFUL;

struct CTCBulletTime
{
    CThing* ParentThing;
    unsigned long PlaySoundW(unsigned long sound, unsigned long flags)
    {
        CTCSound* sounds;
        if (ParentThing->GetInterface(16, &sounds))
            return sounds->PlaySound(sound, 0, flags, 0, -1);
        return g_InvalidSoundHandle;
    }
};

int main()
{
    CTCSound snd; snd.tag = 16;
    CInterfaceMap::SEntry entries[3];
    entries[0].type = 3;  entries[0].component = 0;
    entries[1].type = 16; entries[1].component = &snd;
    entries[2].type = 40; entries[2].component = 0;
    CThing thing;
    for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 3;
    CTCBulletTime bt; bt.ParentThing = &thing;
    if (bt.PlaySoundW(77, 5) != 0xFFFFFFFFUL) { std::printf("0079de00_TEST FAIL\n"); return 1; }
    thing.m_presentBits[0] = 0x10000;
    if (bt.PlaySoundW(77, 5) != 1234 || g_lastSound != 77 || g_lastFlags != 5 || g_lastA != 0 || g_lastB != 0 || g_lastC != -1)
    {
        std::printf("0079de00_TEST FAIL\n");
        return 1;
    }
    std::printf("0079de00_TEST PASS\n");
    return 0;
}
