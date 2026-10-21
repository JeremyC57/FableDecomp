#include <cstdio>
struct CTCBase;
class CInterfaceMap {                                    // CThing::Components, CVectorMap<ETCInterfaceType, CTCBase*> (retail 8 bytes)
public:
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry*       LowerBound(const long* type);          // 0x0040F020
    const SEntry* LowerBound(const long* type) const;    // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char Type;                                  // +0x10 (1 = a creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)               // ??$GetTC@V...@@@CThing@@QAE_NPAPAV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || T::INTERFACE_TYPE < it->type)
            it = map->last;
        *out = (T*)it->component;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const  // ??$PeekTC@$$CBV...@@@CThing@@QBE_NPAPBV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || T::INTERFACE_TYPE < it->type)
            it = map->last;
        *out = (const T*)it->component;
        return true;
    }
};
class CTCTargetingBase {                                // interface 8 (class not confirmed)
public:
    enum { INTERFACE_TYPE = 8 };
    void SetOnlyTarget(CThing* target);                  // 0x008ABC90 (name not confirmed)
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPPlayerCharacter() const;   // 0x00487DC0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                            // 0x00449970
};
class CMainGameComponent {
public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PPlayerManager;                      // +0x1c (scoped_ptr)
};
class CScriptThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                    // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                        // slot 75 (+0x12c)
};
class CGameScriptInterface {
public:
    virtual void SetPlayerCreatureOnlyTarget(const CScriptThing& target) const;
    void*               World;                           // +0x04
    CMainGameComponent* Component;                       // +0x08
};
void CGameScriptInterface::SetPlayerCreatureOnlyTarget(const CScriptThing& target) const
{
    CThing* hero = Component->PPlayerManager->GetMainPlayer().GetPPlayerCharacter();
    if (target.IsValid() && hero && !hero->IsDead())
    {
        CThing* t = target.GetThing();
        CTCTargetingBase* tc;
        if (hero->GetTC(&tc))
            tc->SetOnlyTarget(t);
    }
}

void CScriptThing::v000() {}
void CScriptThing::v001() {}
void CScriptThing::v002() {}
void CScriptThing::v003() {}
void CScriptThing::v004() {}
void CScriptThing::v005() {}
void CScriptThing::v006() {}
void CScriptThing::v007() {}
void CScriptThing::v008() {}
void CScriptThing::v009() {}
void CScriptThing::v010() {}
void CScriptThing::v012() {}
void CScriptThing::v013() {}
void CScriptThing::v014() {}
void CScriptThing::v015() {}
void CScriptThing::v016() {}
void CScriptThing::v017() {}
void CScriptThing::v018() {}
void CScriptThing::v019() {}
void CScriptThing::v020() {}
void CScriptThing::v021() {}
void CScriptThing::v022() {}
void CScriptThing::v023() {}
void CScriptThing::v024() {}
void CScriptThing::v025() {}
void CScriptThing::v026() {}
void CScriptThing::v027() {}
void CScriptThing::v028() {}
void CScriptThing::v029() {}
void CScriptThing::v030() {}
void CScriptThing::v031() {}
void CScriptThing::v032() {}
void CScriptThing::v033() {}
void CScriptThing::v034() {}
void CScriptThing::v035() {}
void CScriptThing::v036() {}
void CScriptThing::v037() {}
void CScriptThing::v038() {}
void CScriptThing::v039() {}
void CScriptThing::v040() {}
void CScriptThing::v041() {}
void CScriptThing::v042() {}
void CScriptThing::v043() {}
void CScriptThing::v044() {}
void CScriptThing::v045() {}
void CScriptThing::v046() {}
void CScriptThing::v047() {}
void CScriptThing::v048() {}
void CScriptThing::v049() {}
void CScriptThing::v050() {}
void CScriptThing::v051() {}
void CScriptThing::v052() {}
void CScriptThing::v053() {}
void CScriptThing::v054() {}
void CScriptThing::v055() {}
void CScriptThing::v056() {}
void CScriptThing::v057() {}
void CScriptThing::v058() {}
void CScriptThing::v059() {}
void CScriptThing::v060() {}
void CScriptThing::v061() {}
void CScriptThing::v062() {}
void CScriptThing::v063() {}
void CScriptThing::v064() {}
void CScriptThing::v065() {}
void CScriptThing::v066() {}
void CScriptThing::v067() {}
void CScriptThing::v068() {}
void CScriptThing::v069() {}
void CScriptThing::v070() {}
void CScriptThing::v071() {}
void CScriptThing::v072() {}
void CScriptThing::v073() {}
void CScriptThing::v074() {}
CThing* CScriptThing::GetThing() const { return 0; }
bool CScriptThing::IsValid() const { return false; }
class CTestScriptThing : public CScriptThing { public: CThing* T; bool Valid; CThing* GetThing() const { return T; } bool IsValid() const { return Valid; } };
static CInterfaceMap::SEntry g_entries[3];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const { const SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
class CTCImpl { public: int pad; };
static int g_calls; static CThing* g_target = (CThing*)1; static CTCTargetingBase* g_who;
void CTCTargetingBase::SetOnlyTarget(CThing* t) { ++g_calls; g_target = t; g_who = this; }
static CThingPlayerCreature g_hero; static CThingPlayerCreature* g_heroPtr;
CThingPlayerCreature* CPlayer::GetPPlayerCharacter() const { return g_heroPtr; }
static CPlayer g_player;
CPlayer& CPlayerManager::GetMainPlayer() { return g_player; }
int main()
{
    static CTCImpl impl; CTCTargetingBase* tc = (CTCTargetingBase*)&impl;
    for (int i = 0; i < 9; ++i) g_hero.UsingComponents[i] = 0;
    g_hero.Dead = 0;
    g_entries[0].type = 3; g_entries[0].component = 0;
    g_entries[1].type = 8; g_entries[1].component = (CTCBase*)tc;
    g_hero.Components.first = g_entries; g_hero.Components.last = g_entries + 2;
    CPlayerManager pm; CMainGameComponent mgc; mgc.PPlayerManager = &pm;
    CGameScriptInterface gsi; gsi.Component = &mgc;
    CThing victim; CTestScriptThing s; s.T = &victim; s.Valid = true;
    g_heroPtr = 0; gsi.SetPlayerCreatureOnlyTarget(s);
    bool ok = g_calls == 0;
    g_heroPtr = &g_hero; gsi.SetPlayerCreatureOnlyTarget(s);
    ok = ok && g_calls == 0;                                     // no interface 8
    g_hero.UsingComponents[0] = 1u << 8; g_hero.Dead = 1; gsi.SetPlayerCreatureOnlyTarget(s);
    ok = ok && g_calls == 0;                                     // dead
    g_hero.Dead = 0; s.Valid = false; gsi.SetPlayerCreatureOnlyTarget(s);
    ok = ok && g_calls == 0;                                     // invalid target
    s.Valid = true; gsi.SetPlayerCreatureOnlyTarget(s);
    ok = ok && g_calls == 1 && g_target == &victim && g_who == tc;
    std::printf(ok ? "00899e10_TEST PASS\n" : "00899e10_TEST FAIL\n");
    return ok ? 0 : 1;
}