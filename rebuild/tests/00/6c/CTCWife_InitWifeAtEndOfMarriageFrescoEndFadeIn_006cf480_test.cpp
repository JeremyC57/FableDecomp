// Behaviour test for 006cf480: src/006cf480.cpp copied verbatim; with an owner def that has a CWifeDef sub-def with a
// positive Dowry, the live hero's CTCHeroStats (interface 4) gets AddMoney(Dowry, true); the sub-def is released.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCWife::InitWifeAtEndOfMarriageFrescoEndFadeIn() -- once the wedding fresco has faded in, the wife's def sub-def (CWifeDef) is fetched from her thing's def and a positive Dowry is paid into the living hero's CTCHeroStats
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CF480, 163 bytes (+13 int3). __fastcall, `ret`.
// Callees: CDefClassBase::GetSubDef<CWifeDef>(CDefPointer<const CWifeDef>*) 0x006D3DC0 (manifest label names the
//          CAICreatureWillPowerIndicatorDef instantiation; propagated), CTCBase::GetPlayerManager() 0x00686D40,
//          CPlayerManager::GetMainPlayer() 0x00449970, CPlayer::GetHeroThing() 0x00487DC0, LowerBound 0x0040F020,
//          CTCHeroStats::AddMoney(long, bool) 0x0057B327, the sub-def's slot 1 on release.
// Shape: the owner's def pointer (CThing PDef +0x70) is read into a local and null-tested first; the sub-def pointer is
//        zeroed between that load and its test (`mov [esp+4],0`); GetSubDef is a same-TU noinline stand-in body that fills
//        it through `T**` (writes only the out slot), so it is read back into edi right after the call (before `test al,al`)
//        and kept there for the Dowry reads and the hand release; the Dowry test guards the hero lookup; interface 4
//        (CTCHeroStats, byte +0x20 bit 4) is fetched through the out-parameter GetInterface whose end/hit choice is one
//        ternary (the component lands straight in ecx) into the shared key word; AddMoney gets the re-read Dowry and true.
//        The early return on a null owner def skips the release because the sub-def is known null.
// Layout: CTCWife ParentThing +0x04; CThing PDef +0x70; CWifeDef Dowry +0x28 (PDB 40).
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
};
class CWifeDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x28 - 0x08];
    long Dowry;                                 // +0x28
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CWifeDef** out); // 0x006D3DC0
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CWifeDef** out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    *out = (CWifeDef*)def;
    return true;
}
#pragma auto_inline(on)
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 (the other TU copy is 0x004365B0)
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefPointer<CDefClassBase> PDef;            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CPlayer { public:
    CThing* GetHeroThing() const;               // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer();                   // 0x00449970
};
class CTCHeroStats { public:
    void AddMoney(long amount, bool showMessage);   // 0x0057B327
};
class CTCWife { public:
    virtual void v00();
    CThing* ParentThing;                         // +0x04
    CPlayerManager* GetPlayerManager();          // 0x00686D40 (CTCBase)
    void InitWifeAtEndOfMarriageFrescoEndFadeIn();
};
void CTCWife::InitWifeAtEndOfMarriageFrescoEndFadeIn()
{
    CDefClassBase* thingDef = ParentThing->PDef.Object;
    CWifeDef* def = 0;
    if (thingDef) {
        if (thingDef->GetSubDef(&def) && def->Dowry > 0) {
            CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
            if (hero && !(hero->Flags91 & 1)) {
                CTCHeroStats* stats;
                if (hero->GetInterface(&stats, 4))
                    stats->AddMoney(def->Dowry, true);
            }
        }
        if (def && --def->m_refCount == 0)
            def->Destroy();
    }
}
// ---- harness ----
static int g_destroyed = 0, g_adds = 0, g_heroCalls = 0; static long g_amount = -1; static bool g_show = false;
static const void* g_addThis = 0; static void* g_sub = 0; static const void* g_subOwner = 0;
static CThing* g_hero = 0; static unsigned char g_mgr[4], g_player[4];
void CDefPointeeBase::v00() { }
void CDefPointeeBase::Destroy() { ++g_destroyed; }
void CTCWife::v00() { }
void* Find_CDefClassBase_GetSubDef(const void* owner)
{ g_subOwner = owner; if (g_sub) ++((CDefPointeeBase*)g_sub)->m_refCount; return g_sub; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
CPlayerManager* CTCWife::GetPlayerManager() { return (CPlayerManager*)g_mgr; }
CPlayer* CPlayerManager::GetMainPlayer() { CHECK(this == (CPlayerManager*)g_mgr); return (CPlayer*)g_player; }
CThing* CPlayer::GetHeroThing() const { CHECK(this == (const CPlayer*)g_player); ++g_heroCalls; return g_hero; }
void CTCHeroStats::AddMoney(long amount, bool showMessage) { ++g_adds; g_addThis = this; g_amount = amount; g_show = showMessage; }
int main()
{
    static CDefClassBase ownerDef; ownerDef.m_refCount = 1;
    static CWifeDef wifeDef; wifeDef.m_refCount = 1; wifeDef.Dowry = 500;
    static unsigned char wifeThingBuf[sizeof(CThing)]; CThing& wifeThing = *(CThing*)wifeThingBuf; wifeThing.PDef.Object = &ownerDef;
    static unsigned char statsBuf[4]; CTCHeroStats* stats = (CTCHeroStats*)statsBuf;
    CInterfaceMap::SEntry entries[3] = { { 1, 0 }, { 4, (CTCBase*)stats }, { 9, 0 } };
    static unsigned char heroBuf[sizeof(CThing)]; CThing& hero = *(CThing*)heroBuf;
    hero.m_presentBits[0] = 1UL << 4; hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 3;
    static unsigned char wifeBuf[sizeof(CTCWife)]; CTCWife& wife = *(CTCWife*)wifeBuf; wife.ParentThing = &wifeThing;
    g_sub = &wifeDef; g_hero = &hero;

    wife.InitWifeAtEndOfMarriageFrescoEndFadeIn();
    CHECK(g_subOwner == &ownerDef && g_adds == 1 && g_addThis == stats && g_amount == 500 && g_show);
    CHECK(wifeDef.m_refCount == 1 && g_destroyed == 0);

    // no dowry / dead hero / no hero / no stats interface: nothing paid
    g_adds = 0; wifeDef.Dowry = 0; wife.InitWifeAtEndOfMarriageFrescoEndFadeIn(); CHECK(g_adds == 0 && g_heroCalls == 1);
    wifeDef.Dowry = 7; hero.Flags91 = 1; wife.InitWifeAtEndOfMarriageFrescoEndFadeIn(); CHECK(g_adds == 0);
    hero.Flags91 = 0; g_hero = 0; wife.InitWifeAtEndOfMarriageFrescoEndFadeIn(); CHECK(g_adds == 0);
    g_hero = &hero; hero.m_presentBits[0] = 0; wife.InitWifeAtEndOfMarriageFrescoEndFadeIn(); CHECK(g_adds == 0);
    hero.m_presentBits[0] = 1UL << 4;
    CHECK(wifeDef.m_refCount == 1 && g_destroyed == 0);

    // no sub-def / no owner def
    g_sub = 0; g_heroCalls = 0; wife.InitWifeAtEndOfMarriageFrescoEndFadeIn(); CHECK(g_adds == 0 && g_heroCalls == 0);
    g_subOwner = 0; wifeThing.PDef.Object = 0; wife.InitWifeAtEndOfMarriageFrescoEndFadeIn(); CHECK(g_subOwner == 0 && g_adds == 0);
    wifeThing.PDef.Object = &ownerDef;

    // last reference: destroyed after paying
    g_sub = &wifeDef; wifeDef.m_refCount = 0;
    wife.InitWifeAtEndOfMarriageFrescoEndFadeIn();
    CHECK(g_adds == 1 && g_amount == 7 && g_destroyed == 1);

    if (fails == 0) printf("006cf480_TEST PASS\n");
    return fails;
}