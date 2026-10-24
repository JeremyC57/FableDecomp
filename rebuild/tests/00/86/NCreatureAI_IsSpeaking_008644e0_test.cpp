// Behaviour test for 0x008644E0: IsSpeaking is true when talking (type 0x0F) or when a non-replaceable,
// active, type-0 emote icon (type 0x45) shows.
#include <cstdio>

class CTCBase;

struct CInterfaceMap
{
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type) const
    {
        SEntry* it = first;
        while (it != last && it->type < type) ++it;
        return it;
    }
};

class CTCEmoteIcon
{
public:
    long ActiveTimer;
    long EmoteType;
    bool Replaceable;
};

class CTCTalk
{
public:
    bool m_talking;
    bool IsTalking() const { return m_talking; }
};

class CThing
{
public:
    unsigned int  m_presentBits[9];
    CInterfaceMap m_interfaces;

    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1U << (type & 31)))
            return true;
        return false;
    }
    CInterfaceMap::SEntry* FindEntry(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        CInterfaceMap::SEntry* sel;
        if (it == end) sel = end;
        else if (it->type <= type) sel = it;
        else sel = end;
        return sel;
    }
    template <class T> bool GetInterface(long type, T** out) const
    {
        if (m_presentBits[type >> 5] & (1U << (type & 31)))
        {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
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

bool IsSpeaking(const CThingCreatureBase& creature)
{
    bool emoting = false;
    const CTCEmoteIcon* emote;
    if (creature.GetInterface(0x45, &emote))
    {
        if (!emote->Replaceable && emote->ActiveTimer != 0 && emote->EmoteType == 0)
            emoting = true;
    }
    return (creature.HasTC(0x0F) && ((const CTCTalk*)creature.FindEntry(0x0F)->component)->IsTalking()) || emoting;
}

int main()
{
    CTCTalk talk; talk.m_talking = false;
    CTCEmoteIcon emote; emote.ActiveTimer = 0; emote.EmoteType = 0; emote.Replaceable = false;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x0F; entries[0].component = (CTCBase*)&talk;
    entries[1].type = 0x45; entries[1].component = (CTCBase*)&emote;
    CThingCreatureBase c;
    for (int i = 0; i < 9; ++i) c.m_presentBits[i] = 0;
    c.m_interfaces.first = entries; c.m_interfaces.last = entries + 2;
    int fails = 0;
    talk.m_talking = true;
    if (IsSpeaking(c)) ++fails;                       // no bits set
    c.m_presentBits[0] = 0x8000;                      // talk present
    if (!IsSpeaking(c)) ++fails;
    talk.m_talking = false;
    if (IsSpeaking(c)) ++fails;
    c.m_presentBits[2] = 0x20;                        // emote present
    emote.ActiveTimer = 3;
    if (!IsSpeaking(c)) ++fails;
    emote.Replaceable = true;
    if (IsSpeaking(c)) ++fails;
    emote.Replaceable = false; emote.EmoteType = 2;
    if (IsSpeaking(c)) ++fails;
    emote.EmoteType = 0; emote.ActiveTimer = 0;
    if (IsSpeaking(c)) ++fails;
    if (fails) { std::printf("008644e0_TEST FAIL\n"); return 1; }
    std::printf("008644e0_TEST PASS\n");
    return 0;
}