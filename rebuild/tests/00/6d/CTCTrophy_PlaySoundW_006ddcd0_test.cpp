#include <cstdio>
struct CCharString { const char* text; };
struct CSoundComponent {
    int plays; const CCharString* last;
    unsigned long PlaySound(const CCharString& name, bool, long, long, long) { ++plays; last = &name; return 1000 + plays; }
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
static unsigned long g_InvalidSoundHandle = 0xFFFFFFFFUL;
struct CTCTrophy {
    CThing* Thing;
    unsigned long PlaySoundW(const CCharString& name)
    {
        CSoundComponent* sound;
        if (Thing->HasTC(16) && Thing->GetInterface(&sound, 16))
            return sound->PlaySound(name, true, 0, 0, -1);
        return g_InvalidSoundHandle;
    }
};
int main()
{
    CSoundComponent snd; snd.plays = 0; snd.last = 0;
    CInterfaceMap::SEntry entries[3] = { { 3, 0 }, { 16, (CTCBase*)&snd }, { 0, 0 } };
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    CTCTrophy t; t.Thing = &thing;
    CCharString name; name.text = "x";
    if (t.PlaySoundW(name) != 0xFFFFFFFFUL || snd.plays != 0) { std::printf("006ddcd0_TEST FAIL\n"); return 1; }
    thing.m_presentBits[0] = 1UL << 16;
    if (t.PlaySoundW(name) != 1001 || snd.plays != 1 || snd.last != &name) { std::printf("006ddcd0_TEST FAIL\n"); return 1; }
    std::printf("006ddcd0_TEST PASS\n");
    return 0;
}