#include <cstdio>
class CScriptThing;
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
    CScriptThing GetScriptThing();                       // 0x004C7E50 (by value through the hidden result pointer)
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
        *out = (const T*)((it == map->last || T::INTERFACE_TYPE < it->type) ? map->last : it)->component;
        return true;
    }
};
class CScriptThing {
public:
    CScriptThing();                                      // 0x008CFF40
    CScriptThing(const CScriptThing& other);
    virtual ~CScriptThing();
    void* Data;
};
class CTCTargetingBase {                                // interface 8 (class not confirmed)
public:
    enum { INTERFACE_TYPE = 8 };
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetTarget() const;                   // slot 33 (+0x84, name not confirmed)
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
class CGameScriptInterface {
public:
    virtual CScriptThing GetHeroTargetedThing() const;
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
};
CScriptThing CGameScriptInterface::GetHeroTargetedThing() const
{
    CThing* hero = PlayerManager->GetMainPlayer().GetPPlayerCharacter();
    if (hero)
    {
        const CTCTargetingBase* tc;
        hero->PeekTC(&tc);
        CThing* target = tc->GetTarget();
        if (target)
            return target->GetScriptThing();
    }
    return CScriptThing();
}

static CInterfaceMap::SEntry g_entries[3];
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const { const SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
CScriptThing::CScriptThing() { Data = 0; }
CScriptThing::CScriptThing(const CScriptThing& o) { Data = o.Data; }
CScriptThing::~CScriptThing() {}
CScriptThing CThing::GetScriptThing() { CScriptThing s; s.Data = this; return s; }
static CThingPlayerCreature g_hero; static CThingPlayerCreature* g_heroPtr; static CThing g_target; static CThing* g_aimed;
CThingPlayerCreature* CPlayer::GetPPlayerCharacter() const { return g_heroPtr; }
static CPlayer g_player;
CPlayer& CPlayerManager::GetMainPlayer() { return g_player; }
static CThing* __fastcall TcGetTarget(const void*, int) { return g_aimed; }
int main()
{
    static void* tvt[34]; tvt[33] = (void*)TcGetTarget; void* tobj = tvt;
    for (int i = 0; i < 9; ++i) g_hero.UsingComponents[i] = 0;
    g_hero.UsingComponents[0] = 1u << 8;
    g_entries[0].type = 4; g_entries[0].component = 0;
    g_entries[1].type = 8; g_entries[1].component = (CTCBase*)&tobj;
    g_hero.Components.first = g_entries; g_hero.Components.last = g_entries + 2;
    CPlayerManager pm; CGameScriptInterface gsi; gsi.PlayerManager = &pm;
    g_heroPtr = &g_hero; g_aimed = &g_target;
    CScriptThing a = gsi.GetHeroTargetedThing();
    bool ok = a.Data == &g_target;
    g_aimed = 0; CScriptThing b = gsi.GetHeroTargetedThing(); ok = ok && b.Data == 0;
    g_heroPtr = 0; g_aimed = &g_target; CScriptThing c = gsi.GetHeroTargetedThing(); ok = ok && c.Data == 0;
    std::printf(ok ? "0089a490_TEST PASS\n" : "0089a490_TEST FAIL\n");
    return ok ? 0 : 1;
}