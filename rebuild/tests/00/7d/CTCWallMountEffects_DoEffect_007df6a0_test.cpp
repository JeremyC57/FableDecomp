// Behaviour test for 007df6a0: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCWallMountEffects::DoEffect() -- stamp the use frame, then a teleport mount starts its CTCTeleporter on the living hero and a healing mount heals the living creature using it
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#define __int64 long long
#endif
class CTCBase;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags;                      // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
class CThingPlayerCreature;
class CThingPhysical : public CThing { public:          // CThingCreatureBase derives from it
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual void ModifyHealth(float amount, bool showEffect);   // slot 59 (+0xec) 0x006A68F0 (CThingPhysical)
};

class CTCTeleporter { public:                          // interface 0xdd
    void StartTeleporting(CThingPlayerCreature* hero);   // 0x007DFF90
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

class CPlayer { public:
    CThing* GetHeroThing();                     // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();                   // 0x00449970
};
class CWorld { public:
    static long __fastcall GetFrame();                // 0x0049D870
};

enum EWallMountEffects { WALL_MOUNT_EFFECT_NONE = 0, WALL_MOUNT_EFFECT_TELEPORT = 1, WALL_MOUNT_EFFECT_HEAL = 2 };
class CWallMountEffectsDef { public:
    unsigned char     _pad_00[0x28];
    EWallMountEffects Effect;                   // +0x28
    float             HealingAmount;            // +0x2c
};

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[4];
    CPlayerManager* GetPlayerManager();         // 0x00686D40
};

class CTCWallMountEffects : public CTCBase { public:
    const CWallMountEffectsDef* PDef;           // +0x0c CDefPointer<CWallMountEffectsDef const>
    long                    WaitingForConfirmation;     // +0x10
    CBaseIntelligentPointer CreaturePerformingAction;   // +0x14 CIntelligentPointer<CThingCreatureBase>
    long                    FrameEffectLastUsed;        // +0x1c
    bool                    ObjectWasGamePersistant;    // +0x20
    void DoEffect();
};

void CTCWallMountEffects::DoEffect()
{
    FrameEffectLastUsed = CWorld::GetFrame();
    switch (PDef->Effect) {
    case WALL_MOUNT_EFFECT_TELEPORT: {
        CThing* hero = GetPlayerManager()->GetMainPlayer().GetHeroThing();
        if (hero && !hero->IsDead()) {
            CThing* thing = ParentThing;
            CTCTeleporter* teleporter = 0;
            if (thing->HasTC(0xdd))
                teleporter = (CTCTeleporter*)thing->GetTC(0xdd);
            teleporter->StartTeleporting((CThingPlayerCreature*)hero);
        }
        break;
    }
    case WALL_MOUNT_EFFECT_HEAL:
        if (IS_THING_ALIVE(CreaturePerformingAction))
            ((CThingPhysical*)CreaturePerformingAction.GetPItem())->ModifyHealth(PDef->HealingAmount, true);
        break;
    }
}

// ---- stubs and driver ----
void CThing::v00() {}
#define V(n) void CThingPhysical::v##n() {}
V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19) V(20)
V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39) V(40)
V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49) V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58)
static CThing* g_healed; static float g_healAmount; static bool g_healEffect; static int g_heals;
void CThingPhysical::ModifyHealth(float amount, bool showEffect) { g_healed = this; g_healAmount = amount; g_healEffect = showEffect; ++g_heals; }
static CTCTeleporter* g_teleporter; static CThingPlayerCreature* g_teleported; static int g_teleports;
void CTCTeleporter::StartTeleporting(CThingPlayerCreature* hero) { g_teleporter = this; g_teleported = hero; ++g_teleports; }
void CBaseIntelligentPointer::v00() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
static CThing* g_hero;
CThing* CPlayer::GetHeroThing() { return g_hero; }
static CPlayer g_player;
CPlayer& CPlayerManager::GetMainPlayer() { return g_player; }
static CPlayerManager g_manager;
CPlayerManager* CTCBase::GetPlayerManager() { return &g_manager; }
void CTCBase::v00() {}
long CWorld::GetFrame() { return 4321; }

static CThingPhysical g_creature, g_mount;
static CTCTeleporter g_tp;
static CInterfaceMap::SEntry g_entries[3];

static int Fail(const char* what) { std::printf("007df6a0_TEST FAIL (%s)\n", what); return 1; }

int main()
{
    g_entries[0].type = 0x10;  g_entries[0].component = 0;
    g_entries[1].type = 0xdd;  g_entries[1].component = (CTCBase*)&g_tp;
    g_entries[2].type = 0x120; g_entries[2].component = 0;
    g_mount.m_interfaces.first = g_entries; g_mount.m_interfaces.last = g_entries + 3;
    g_mount.m_presentBits[6] = 0x20000000;
    CWallMountEffectsDef def; std::memset(&def, 0, sizeof(def));
    def.HealingAmount = 12.5f;
    CTCWallMountEffects w;
    w.ParentThing = &g_mount; w.PDef = &def; w.FrameEffectLastUsed = 0; w.CreaturePerformingAction.m_pItem = &g_creature;

    // no effect: only the frame is stamped
    def.Effect = WALL_MOUNT_EFFECT_NONE;
    w.DoEffect();
    if (w.FrameEffectLastUsed != 4321 || g_heals || g_teleports) return Fail("none");

    // teleport: the living hero is handed to the mount's teleporter
    def.Effect = WALL_MOUNT_EFFECT_TELEPORT;
    g_hero = &g_creature;
    w.DoEffect();
    if (g_teleports != 1 || g_teleporter != &g_tp || g_teleported != (CThingPlayerCreature*)&g_creature || g_heals) return Fail("teleport");
    g_hero = 0; w.DoEffect();
    g_hero = &g_creature; g_creature.m_flags = 1; w.DoEffect();
    if (g_teleports != 1) return Fail("teleport no hero / dead hero");
    g_creature.m_flags = 0;

    // heal: the living user gets the def's healing amount with the effect shown
    def.Effect = WALL_MOUNT_EFFECT_HEAL;
    w.DoEffect();
    if (g_heals != 1 || g_healed != &g_creature || g_healAmount != 12.5f || !g_healEffect || g_teleports != 1) return Fail("heal");
    g_creature.m_flags = 1; w.DoEffect();
    w.CreaturePerformingAction.m_pItem = 0; w.DoEffect();
    if (g_heals != 1) return Fail("heal dead / none");

    std::printf("007df6a0_TEST PASS\n");
    return 0;
}