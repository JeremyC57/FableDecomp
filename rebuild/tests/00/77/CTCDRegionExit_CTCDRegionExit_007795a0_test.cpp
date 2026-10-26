// Behaviour test for ~CTCDRegionExit (0x007795A0): in letterbox mode with the world still showing it, the letterbox ends and a
// living hero's region display finishes showing the name; the entrance pointer and the base are always destroyed.
#include <cstdio>
#include <cstring>
static char g_log[16];
static void Log(char c) { size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing { public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char Flags91;
    template <class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
inline bool IsThingAlive(const CThing* thing) { return thing && !(thing->Flags91 & 1); }
class CTCRegionDisplay { public: void FinishShowingRegionName() { Log('f'); } };
static CThing* g_hero = 0;
class CPlayer { public: CThing* GetHeroCreature() { return g_hero; } };
static CPlayer g_player;
class CPlayerManager { public: CPlayer* GetMainPlayer() { return &g_player; } };
static CPlayerManager g_pm;
class CWorld { public: bool LetterBoxShown; };
static CWorld g_world;
class CDisplayEngine { public: void EndLetterBox() { Log('d'); } };
class CMainGameComponent { public: CDisplayEngine* PDisplayEngine; };
static CDisplayEngine g_de; static CMainGameComponent g_mgc = { &g_de };
CMainGameComponent* g_pMainGameComponent = &g_mgc;
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CThing* m_pItem;
    ~CBaseIntelligentPointer() { Log('p'); }
};
struct CIntelligentPointer : CBaseIntelligentPointer { };
class CTCBase { public:
    virtual ~CTCBase() { Log('b'); }
    CThing* ParentThing;
    const CWorld* GetWorld() const { return &g_world; }
    CPlayerManager* GetPlayerManager() { return &g_pm; }
};
class CTCDRegionExit : public CTCBase { public:
    bool InLetterBoxMode;
    CIntelligentPointer PEntranceConnectedTo;
    virtual ~CTCDRegionExit();
};

CTCDRegionExit::~CTCDRegionExit()
{
    if (InLetterBoxMode && GetWorld()->LetterBoxShown) {
        g_pMainGameComponent->PDisplayEngine->EndLetterBox();
        CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroCreature();
        if (IsThingAlive(hero)) {
            CTCRegionDisplay* display;
            hero->GetInterface(&display, 0x8e);
            display->FinishShowingRegionName();
        }
    }
}

static const char* Run(bool letterbox, bool shown)
{
    CTCDRegionExit* e = new CTCDRegionExit; e->InLetterBoxMode = letterbox; g_world.LetterBoxShown = shown;
    g_log[0] = 0; delete e; return g_log;
}
int main()
{
    int bad = 0;
    CTCRegionDisplay display;
    CThing hero; for (int i = 0; i < 9; ++i) hero.m_presentBits[i] = 0; hero.Flags91 = 0;
    CInterfaceMap::SEntry e[1] = { { 0x8e, (CTCBase*)&display } };
    hero.m_interfaces.first = e; hero.m_interfaces.last = e + 1; hero.m_presentBits[4] = 1UL << 14;
    g_hero = &hero;
    if (std::strcmp(Run(true, true), "dfpb")) bad |= 1;
    if (std::strcmp(Run(true, false), "pb")) bad |= 2;
    if (std::strcmp(Run(false, true), "pb")) bad |= 4;
    hero.Flags91 = 1;
    if (std::strcmp(Run(true, true), "dpb")) bad |= 8;
    g_hero = 0;
    if (std::strcmp(Run(true, true), "dpb")) bad |= 16;
    if (bad) { std::printf("007795a0_TEST FAIL\n"); return 1; }
    std::printf("007795a0_TEST PASS\n");
    return 0;
}