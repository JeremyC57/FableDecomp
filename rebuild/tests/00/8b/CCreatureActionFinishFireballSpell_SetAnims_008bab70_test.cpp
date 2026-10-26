// Behaviour test for 0x008BAB70 CCreatureAction_FinishFireballSpell::SetAnims: with a fireball component (0x9d) the
// action's animation becomes the component's release animation name (the temporary string is destroyed); without
// the component nothing is set.
#include <cstring>
#include <cstdio>

class CTCBase { public: int calls; void* arg; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing { public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    int m_killed;
    void Kill(bool removeNow) { m_killed = removeNow ? 2 : 1; m_flags91 |= 1; }
    template <class T> void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
    void Init()
    {
        for (int i = 0; i < 9; ++i) m_presentBits[i] = 0;
        m_interfaces.first = m_interfaces.last = 0; m_flags91 = 0; m_killed = 0;
    }
    void AddTC(CInterfaceMap::SEntry* entries, int n, long type)
    {
        m_presentBits[type >> 5] |= 1UL << (type & 31);
        m_interfaces.first = entries; m_interfaces.last = entries + n;
    }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
static int g_liveStrings = 0;
class CCharString { public:
    char m_text[24];
    CCharString(const char* t) { std::strcpy(m_text, t); ++g_liveStrings; }
    CCharString(const CCharString& o) { std::strcpy(m_text, o.m_text); ++g_liveStrings; }
    ~CCharString() { --g_liveStrings; }
};
template <class T> static bool GetInterfaceB(CThing* t, T** out, long type)
{
    if (t->m_presentBits[type >> 5] & (1UL << (type & 31))) { t->GetInterface(out, type); return true; }
    return false;
}
class CTCFireballSpell : public CTCBase { public: CCharString GetReleaseAnimName() { ++calls; return CCharString("FIREBALL_RELEASE"); } };
class CCreatureAction_FinishFireballSpell { public:
    CBaseIntelligentPointer PCreature; char m_anim[24]; int m_sets;
    void SetAnimationName(const CCharString& n) { std::strcpy(m_anim, n.m_text); ++m_sets; }
    void SetAnims()
    {
        CTCFireballSpell* fireball;
        if (GetInterfaceB(PCreature.GetPItem(), &fireball, 0x9d))
            SetAnimationName(fireball->GetReleaseAnimName());
    }
};
int main()
{
    CTCFireballSpell fb; fb.calls = 0;
    CInterfaceMap::SEntry e[1] = { { 0x9d, &fb } };
    CThing caster; caster.Init();
    CCreatureAction_FinishFireballSpell a; a.PCreature.m_pItem = &caster; a.m_anim[0] = 0; a.m_sets = 0;
    a.SetAnims();
    bool ok = a.m_sets == 0 && fb.calls == 0;
    caster.AddTC(e, 1, 0x9d);
    a.SetAnims();
    ok = ok && a.m_sets == 1 && fb.calls == 1 && std::strcmp(a.m_anim, "FIREBALL_RELEASE") == 0 && g_liveStrings == 0;
    if (!ok) { std::printf("008bab70_TEST FAIL\n"); return 1; }
    std::printf("008bab70_TEST PASS\n");
    return 0;
}