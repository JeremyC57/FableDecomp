// Behaviour test for 0x00773C90 CTCShop::SetAlarm (copy of the source logic over spy callees).
#include <cstdio>
#include <cstring>
class CTCBase { public: long tag; };
class CCharString { public:
    const char* text;
    CCharString(const char* t, long = -1) : text(t) {}
};
static int g_lookups = 0;
class CASoundBank { public:
    long index;
    long GetSoundIndexFromSymbol(const CCharString& symbol) { ++g_lookups; return std::strcmp(symbol.text, "SND_SHOPALARM_01") == 0 ? index : 0; }
};
class CMainGameComponent { public:
    CASoundBank* PSampleBank;
    CASoundBank* GetSampleBank() const { return PSampleBank; }
};
static CASoundBank g_bank = { 42 };
static CMainGameComponent g_mgc = { &g_bank };
static CMainGameComponent* g_pMainGameComponent = &g_mgc;
class CSoundPair { public:
    long First, Second;
    CSoundPair(long first, long second) : First(first), Second(second) {}
};
class CSoundInit { public:
    float Volume, Pitch; long Delay, LoopCount, Priority, Flags, Pan, Id;
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Delay(0), LoopCount(1), Priority(1), Flags(0), Pan(0), Id(-1) {}
};
static long g_playedFirst = 0, g_playedSecond = 0, g_playLoop = 0, g_playFlags = 0; static bool g_playAttach = false;
static unsigned long g_stopped = 0; static int g_stopAll = 0;
class CTCSoundPlayer : public CTCBase { public:
    unsigned long PlayAttachedSample(const CSoundPair& sound, const CSoundInit& init, bool attach, long, long)
    { g_playedFirst = sound.First; g_playedSecond = sound.Second; g_playLoop = init.LoopCount; g_playFlags = init.Flags; g_playAttach = attach; return 0x777; }
    void StopSample_0066ad90(unsigned long sample) { g_stopped = sample; }
    void StopAllAttachedSounds() { ++g_stopAll; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned long HasTC(long type) { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
class CTCShop { public:
    CThing*       ParentThing;
    unsigned long Alarm;
    void SetAlarm(bool on);
};
void CTCShop::SetAlarm(bool on)
{
    if (on) {
        if (Alarm == 0 && ParentThing->HasTC(0x10)) {
            CTCSoundPlayer* player = ParentThing->GetTC<CTCSoundPlayer>(0x10);
            long sound = g_pMainGameComponent->GetSampleBank()->GetSoundIndexFromSymbol("SND_SHOPALARM_01");
            if (sound) {
                CSoundInit init;
                init.Flags = 0x8010;
                init.LoopCount = -1;
                Alarm = player->PlayAttachedSample(CSoundPair(sound, sound), init, true, 0, 0);
            }
        }
    } else {
        unsigned long alarm = Alarm;
        if (alarm && ParentThing->HasTC(0x10)) {
            CTCSoundPlayer* player = ParentThing->GetTC<CTCSoundPlayer>(0x10);
            player->StopSample_0066ad90(alarm);
            player->StopAllAttachedSounds();
        }
        Alarm = 0;
    }
}
static int fail() { std::printf("00773c90_TEST FAIL\n"); return 1; }
int main()
{
    CTCSoundPlayer player;
    CInterfaceMap::SEntry entries[1] = { { 0x10, &player } };
    CThing thing; std::memset(thing.m_presentBits, 0, sizeof thing.m_presentBits);
    thing.m_presentBits[0] = 1UL << 16;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 1;
    CTCShop shop; shop.ParentThing = &thing; shop.Alarm = 0;
    shop.SetAlarm(true);
    if (shop.Alarm != 0x777 || g_playedFirst != 42 || g_playedSecond != 42 || g_playLoop != -1 || g_playFlags != 0x8010 || !g_playAttach) return fail();
    shop.SetAlarm(true);                       // already ringing: no second lookup
    if (g_lookups != 1) return fail();
    shop.SetAlarm(false);
    if (shop.Alarm != 0 || g_stopped != 0x777 || g_stopAll != 1) return fail();
    shop.SetAlarm(false);                      // nothing to stop
    if (g_stopAll != 1) return fail();
    g_bank.index = 0;                          // unknown sample: stays silent
    shop.SetAlarm(true);
    if (shop.Alarm != 0 || g_lookups != 2) return fail();
    std::printf("00773c90_TEST PASS\n");
    return 0;
}