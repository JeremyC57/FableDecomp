// Behaviour test for CTCShop::OpenShoppingInterface (0x00773B20): an alive hero owning interface 0x17 gets the shop thing set
// on that component; the game event 0x1a carries the main player's number and the 4-byte payload 0xc either way.
#include <cstdio>
#include <cstring>
class CThing;
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
    unsigned char m_flags91;
    bool GetInterface(long type, CTCBase** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = it->component;
            return true;
        }
        return false;
    }
};
struct CTCShoppingComponent {
    CThing* Shop;
    void SetCurrentShop(CThing* shop) { Shop = shop; }
};
class CPlayer { public:
    CThing* Hero;
    CThing* GetHeroThing() { return Hero; }
};
class CPlayerManager { public:
    CPlayer* Main; long Number;
    CPlayer* GetMainPlayer() const { return Main; }
    long GetMainPlayerNumber() { return Number; }
};
struct CGameSubsystem {
    unsigned char _pad00[0x1c];
    CPlayerManager* PlayerManager;
};
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;
    signed char   Player;
    unsigned char Data[32];
    unsigned char EndPos;
    bool          Valid;
    bool          Replacement;
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddULONG(unsigned long value) { Copy(4, &value); }
};
void CMemoryBuffer::Copy(unsigned long length, const void* data)
{
    CGameEvent* e = static_cast<CGameEvent*>(this);
    std::memcpy(e->Data + e->EndPos, data, length);
    e->EndPos = (unsigned char)(e->EndPos + length);
}
class CMainGameComponent { public:
    int Count; long Type; signed char Player; unsigned long Payload; unsigned char End; bool Valid;
    void AddGameEvent(const CGameEvent& event)
    {
        ++Count; Type = event.Type; Player = event.Player; End = event.EndPos; Valid = event.Valid;
        std::memcpy(&Payload, event.Data, 4);
    }
};
CMainGameComponent* g_pMainGameComponent;
class CTCShop { public:
    virtual void v00() { }
    CThing* ParentThing;
    CPlayerManager* Players; CGameSubsystem* Subsystem;
    CPlayerManager* GetPlayerManager() const { return Players; }
    CGameSubsystem* GetGameSubsystem() { return Subsystem; }
    void OpenShoppingInterface();
};

void CTCShop::OpenShoppingInterface()
{
    CThing* hero = GetGameSubsystem()->PlayerManager->GetMainPlayer()->GetHeroThing();
    CTCShoppingComponent* shopping;
    if (hero && !(hero->m_flags91 & 1) && hero->GetInterface(0x17, (CTCBase**)&shopping))
        shopping->SetCurrentShop(ParentThing);
    CGameEvent event(0x1a);
    event.AddULONG(0xc);
    event.Player = (signed char)GetPlayerManager()->GetMainPlayerNumber();
    g_pMainGameComponent->AddGameEvent(event);
}

int main()
{
    int bad = 0;
    CTCShoppingComponent comp; comp.Shop = 0;
    CInterfaceMap::SEntry e[2] = { { 0x10, 0 }, { 0x17, (CTCBase*)&comp } };
    CThing hero; std::memset(hero.m_presentBits, 0, sizeof(hero.m_presentBits));
    hero.m_presentBits[0] = 1UL << 0x17; hero.m_interfaces.first = e; hero.m_interfaces.last = e + 2; hero.m_flags91 = 0;
    CPlayer player; player.Hero = &hero;
    CPlayerManager pm; pm.Main = &player; pm.Number = 1;
    CGameSubsystem sub; sub.PlayerManager = &pm;
    CMainGameComponent mgc; mgc.Count = 0; g_pMainGameComponent = &mgc;
    CThing shopThing;
    CTCShop shop; shop.ParentThing = &shopThing; shop.Players = &pm; shop.Subsystem = &sub;
    shop.OpenShoppingInterface();
    if (comp.Shop != &shopThing) bad |= 1;
    if (mgc.Count != 1 || mgc.Type != 0x1a || mgc.Player != 1 || mgc.Payload != 0xc || mgc.End != 4 || !mgc.Valid) bad |= 2;
    comp.Shop = 0; hero.m_flags91 = 1; pm.Number = 0;
    shop.OpenShoppingInterface();                    // dead hero: no shop set, event still sent
    if (comp.Shop || mgc.Count != 2 || mgc.Player != 0) bad |= 4;
    hero.m_flags91 = 0; hero.m_presentBits[0] = 0;
    shop.OpenShoppingInterface();                    // no shopping interface
    if (comp.Shop || mgc.Count != 3) bad |= 8;
    player.Hero = 0;
    shop.OpenShoppingInterface();
    if (comp.Shop || mgc.Count != 4) bad |= 16;
    if (bad) { std::printf("00773b20_TEST FAIL\n"); return 1; }
    std::printf("00773b20_TEST PASS\n");
    return 0;
}