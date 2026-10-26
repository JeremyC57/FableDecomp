// Behaviour test for 0x007329E0 CTCClock::PlaySoundW(const CCharString&).
#include <stdio.h>
#include <string.h>
class CTCBase;
static int g_live = 0;
class CCharString { public:
    char Text[32];
    CCharString(const char* t) { strcpy(Text, t); ++g_live; }
    CCharString(const CCharString& o) { strcpy(Text, o.Text); ++g_live; }
    ~CCharString() { --g_live; }
};
CCharString operator+(const char* left, const CCharString& right)
{
    char buf[32];
    strcpy(buf, left);
    strcat(buf, right.Text);
    return CCharString(buf);
}
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing {
public:
    unsigned int m_presentBits[9];
    CInterfaceMap m_interfaces;
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1U << (interfaceType & 31)))
            return true;
        return false;
    }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
class CSoundPair { public: long FirstSound; long LastSound; CSoundPair(long f, long l) : FirstSound(f), LastSound(l) {} };
class CSoundInit { public: float Volume; CSoundInit() : Volume(1.0f) {} };
class CTCSoundPlayer { public:
    int calls; long first, last; bool f, l, a;
    unsigned int PlayAttachedSample(const CSoundPair& s, const CSoundInit& init, bool flag, bool loop, bool attached)
    {
        ++calls; first = s.FirstSound; last = s.LastSound; f = flag; l = loop; a = attached;
        return init.Volume == 1.0f ? 1 : 0;
    }
};
class CSoundMap { public:
    struct _Node { CCharString first; long second; };
    _Node* _M_header;
    _Node* nodes;
    int count;
    _Node* _M_find(const CCharString& key) const
    {
        for (int i = 0; i < count; ++i) if (!strcmp(nodes[i].first.Text, key.Text)) return &nodes[i];
        return _M_header;
    }
};
class CClockDef { public:
    CSoundMap Sound;
    long GetSound(const CCharString& name) const
    {
        CSoundMap::_Node* it = Sound._M_find(name);
        if (it != Sound._M_header)
            return it->second;
        return 0;
    }
};
template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
    operator bool() const { return Object != 0; }
};
class CTCClock {
public:
    CThing* ParentThing;
    CDefPointer<const CClockDef> PDef;
    void PlaySoundW(const CCharString& name);
};
void CTCClock::PlaySoundW(const CCharString& name)
{
    if (ParentThing->HasTC(0x10)) {
        CTCSoundPlayer* player = (CTCSoundPlayer*)ParentThing->GetTC(0x10);
        if (PDef) {
            long first = PDef->GetSound("STF_" + name);
            long last = PDef->GetSound("STL_" + name);
            if (first && last)
                player->PlayAttachedSample(CSoundPair(first, last), CSoundInit(), true, false, false);
        }
    }
}
int main()
{
    int fail = 0;
    CTCSoundPlayer player; player.calls = 0;
    CInterfaceMap::SEntry entries[1]; entries[0].type = 0x10; entries[0].component = (CTCBase*)&player;
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_presentBits[0] = 0x10000; thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 1;
    CSoundMap::_Node header = { CCharString(""), 0 };
    CSoundMap::_Node nodes[3] = { { CCharString("STF_Chime"), 40 }, { CCharString("STL_Chime"), 44 }, { CCharString("STF_Tick"), 7 } };
    CClockDef def; def.Sound._M_header = &header; def.Sound.nodes = nodes; def.Sound.count = 3;
    CTCClock clock; clock.ParentThing = &thing; clock.PDef.Object = &def;
    int live = g_live;
    clock.PlaySoundW(CCharString("Chime"));
    if (player.calls != 1 || player.first != 40 || player.last != 44 || !player.f || player.l || player.a) fail |= 1;
    clock.PlaySoundW(CCharString("Tick"));          // no STL_Tick: nothing played
    if (player.calls != 1) fail |= 2;
    clock.PDef.Object = 0;
    clock.PlaySoundW(CCharString("Chime"));         // no definition
    if (player.calls != 1) fail |= 4;
    clock.PDef.Object = &def; thing.m_presentBits[0] = 0;
    clock.PlaySoundW(CCharString("Chime"));         // no sound player
    if (player.calls != 1) fail |= 8;
    if (g_live != live) fail |= 16;                 // every concatenation temporary destroyed
    if (fail) { printf("007329e0_TEST FAIL %d\n", fail); return 1; }
    printf("007329e0_TEST PASS\n");
    return 0;
}