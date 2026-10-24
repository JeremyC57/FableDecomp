// Behaviour test for 0x00864580: StopSpeaking calls SetEmoteText(7, L"", 0, 0) on the emote icon (type 0x45) only
// when the creature has one.
#include <cstdio>
#include <stddef.h>

class CTCBase;

class CWideString
{
public:
    const wchar_t* PStringData;
    CWideString(const wchar_t* text) : PStringData(text) { }
    ~CWideString() { }
};

enum EEmotePriority { EMOTE_PRIORITY_7 = 7 };

class CTCEmoteIcon
{
public:
    int   calls;
    long  priority;
    bool  emptyText;
    float scale, time;
    void SetEmoteText(EEmotePriority p, const CWideString& text, float s, float t)
    {
        ++calls; priority = p; emptyText = text.PStringData[0] == 0; scale = s; time = t;
    }
};

struct CInterfaceMap
{
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type)
    {
        SEntry* it = first;
        while (it != last && it->type < type) ++it;
        return it;
    }
};

class CThing
{
public:
    unsigned int  m_presentBits[9];
    CInterfaceMap m_interfaces;

    template <class T> bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1U << (type & 31)))
        {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CThingCreatureBase : public CThing { };

void StopSpeaking(CThingCreatureBase& creature)
{
    CTCEmoteIcon* emote;
    if (creature.GetInterface(0x45, &emote))
    {
        CWideString text(L"");
        emote->SetEmoteText(EMOTE_PRIORITY_7, text, 0.0f, 0.0f);
    }
}

int main()
{
    CTCEmoteIcon emote; emote.calls = 0; emote.priority = 0; emote.emptyText = false; emote.scale = 1; emote.time = 1;
    CInterfaceMap::SEntry entries[1];
    entries[0].type = 0x45; entries[0].component = (CTCBase*)&emote;
    CThingCreatureBase c;
    for (int i = 0; i < 9; ++i) c.m_presentBits[i] = 0;
    c.m_interfaces.first = entries; c.m_interfaces.last = entries + 1;
    StopSpeaking(c);
    int fails = emote.calls != 0;
    c.m_presentBits[2] = 0x20;
    StopSpeaking(c);
    if (emote.calls != 1 || emote.priority != 7 || !emote.emptyText || emote.scale != 0.0f || emote.time != 0.0f) ++fails;
    if (fails) { std::printf("00864580_TEST FAIL\n"); return 1; }
    std::printf("00864580_TEST PASS\n");
    return 0;
}