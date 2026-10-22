// Behaviour test for 0x00832E60: the reconstructed body (pragmas stripped) driven through recording stubs.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __forceinline inline
#endif
// CThingAICreature::OnDie() -- a creature killed by a thing that answers the killer query posts the hero-postcard message event; a killer with an opinion deed log posts the murder deeds against the victim's faction and tells the victim's wife it was murdered (otherwise she is told it died); then the base OnDie
// Leaf: OnDie
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetDefinitionManager @ 0x004C7990, CDefinitionManager::GetDef<T>
// @ 0x007923C0, CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, CThing::PeekWorld @ 0x004C7AA0,
// CThingCreatureBase::GetPosition @ 0x006A4D00, CMessageEventManager::AddMessage @ 0x00687730, LowerBound @ 0x0040F020 (non-const) and
// @ 0x004365B0 (const), CTCHeroOpinionDeedLog::PostDeed @ 0x006C6990, CTCWife::Murdered @ 0x006D00A0, CTCWife::Died @ 0x006D0020,
// CThingCreatureBase::OnDie @ 0x00663F50.
// Shape: GetDef gets a same-TU auto_inline(off) stand-in that only writes the def slot (the slot then does not escape, so the
// release at the end is `dec dword ptr [ecx+4] ; jne` off the flags); the def is released by hand through a const local after
// the base OnDie; the faction is read through an inline GetFaction() accessor (second read lands in the dead component's edi);
// the faction component (TC 0x49) goes through the const LowerBound copy.
// Layout: retail CThing interface bitset +0x20, CVectorMap +0x44; CThingAICreature's last-attacker intelligent pointer +0xF4
// (descriptive); the world's message event manager +0x60; the faction component (TC 0x49) keeps its faction at +0x18.
struct C3DVector;
struct CFaction;
struct CTCBase;
struct CMessageEvent;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CMessageEventDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long m_count;                                       // +4
};
struct CDefPointer {
    CMessageEventDef* m_p;
    CDefPointer() : m_p(0) { }
};
struct CDefinitionManager {
    bool GetDef(const CCharString& name, CDefPointer& def);   // 0x007923C0
};
void* FindDef_2e60(const CCharString& name);
 bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer& def)
{
    void* found = FindDef_2e60(name);
    if (!found)
        return false;
    *(void**)&def = found;
    return true;
}

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CThing;
struct CMessageEventManager {
    CMessageEvent* AddMessage(CMessageEventDef* def, const C3DVector* position, CThing* thing1, CThing* thing2, long priority);   // 0x00687730
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* MessageEventManager;          // +0x60
};

#define VF(n) virtual void v##n() {}
struct CThing {
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09)
    VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18) VF(19)
    VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29)
    VF(30) VF(31) VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39)
    VF(40) VF(41) VF(42) VF(43) VF(44) VF(45) VF(46) VF(47) VF(48) VF(49)
    VF(50) VF(51) VF(52) VF(53) VF(54) VF(55) VF(56) VF(57) VF(58) VF(59)
    VF(60) VF(61) VF(62) VF(63) VF(64) VF(65) VF(66) VF(67) VF(68) VF(69)
    VF(70) VF(71) VF(72) VF(73) VF(74) VF(75) VF(76)
    virtual bool IsKiller();                            // slot 77 (+0x134)
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];                   // +0x20
    CInterfaceMap Components;                           // +0x44
    CDefinitionManager* GetDefinitionManager();         // 0x004C7990
    CWorld* PeekWorld();                                // 0x004C7AA0
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template <class T> __forceinline const T* GetConstTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (const T*)it->component;
    }
};

struct CBaseIntelligentPointer {
    CThing* GetPItem() const;                           // 0x00A01B50
};

class CThingAICreature;
struct CTCHeroOpinionDeedLog {
    long PostDeed(long deedType, const C3DVector& position, const CThing* target, CThingAICreature* victim,
                  long postType, bool b1, bool b2, const CFaction* faction, bool b3, long extra);   // 0x006C6990
};
struct CTCFaction {
    unsigned char _pad00[0x18];
    CFaction* Faction;                                  // +0x18
    CFaction* GetFaction() const { return Faction; }
};
struct CTCWife {
    void Murdered();                                    // 0x006D00A0
    void Died();                                        // 0x006D0020
};

struct CThingCreatureBase : CThing {
    const C3DVector& GetPosition() const;               // 0x006A4D00
    void OnDie();                                       // 0x00663F50
};

class CThingAICreature : public CThingCreatureBase {
public:
    unsigned char _pad[0xf4 - 0x4c];
    CBaseIntelligentPointer LastAttacker;               // +0xf4
    void OnDie();
};

void CThingAICreature::OnDie()
{
    CDefPointer def;
    CThing* killer = LastAttacker.GetPItem();
    if (killer && killer->IsKiller()) {
        bool found;
        {
            CCharString name("STANDARD_MESSAGE_EVENT_MURDER", -1);
            found = GetDefinitionManager()->GetDef(name, def);
        }
        if (found) {
            CMessageEventManager* manager = PeekWorld()->MessageEventManager;
            manager->AddMessage(def.m_p, &GetPosition(), killer, this, 0);
        }
    }
    CTCWife* wife = 0;
    if (HasTC(0xd7))
        wife = GetTC<CTCWife>(0xd7);
    if (killer && killer->HasTC(0x51)) {
        CTCHeroOpinionDeedLog* log = killer->GetTC<CTCHeroOpinionDeedLog>(0x51);
        if (HasTC(0x49)) {
            const CTCFaction* faction = GetConstTC<CTCFaction>(0x49);
            if (faction->GetFaction()) {
                log->PostDeed(9, GetPosition(), this, 0, 2, true, false, faction->GetFaction(), false, 0);
                log->PostDeed(0x3a, GetPosition(), this, 0, 7, false, false, faction->GetFaction(), true, 0);
            }
        }
        if (wife)
            wife->Murdered();
    }
    else if (wife)
        wife->Died();
    CThingCreatureBase::OnDie();
    const CMessageEventDef* object = def.m_p;
    if (object && --const_cast<CMessageEventDef*>(object)->m_count == 0)
        const_cast<CMessageEventDef*>(object)->Destroy();
}

// ---- recording stubs ----
struct C3DVector { float X, Y, Z; };
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
static int g_destroyed = 0, g_strings = 0, g_messages = 0, g_deeds = 0, g_murdered = 0, g_died = 0, g_baseDie = 0;
static long g_deedTypes[4];
static long g_postTypes[4];
static const CFaction* g_deedFaction[4];
static bool g_deedB1[4], g_deedB3[4];
static CThing* g_msgThing1 = 0;
static CThing* g_msgThing2 = 0;
static CMessageEventDef* g_msgDef = 0;
static CThing* g_killer = 0;
static bool g_isKiller = false;
static bool g_defFound = false;
void CMessageEventDef::v00() { }
void CMessageEventDef::Destroy() { ++g_destroyed; }
static CMessageEventDef g_def;
CCharString::CCharString(const char* text, long length) { m_pData = (void*)text; ++g_strings; }
CCharString::~CCharString() { --g_strings; }
void* FindDef_2e60(const CCharString& name)
{
    if (!g_defFound || strcmp((const char*)name.m_pData, "STANDARD_MESSAGE_EVENT_MURDER") != 0) return 0;
    return &g_def;
}
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
static CDefinitionManager g_manager;
static CWorld g_world;
static CMessageEventManager g_events;
CDefinitionManager* CThing::GetDefinitionManager() { return &g_manager; }
CWorld* CThing::PeekWorld() { return &g_world; }
bool CThing::IsKiller() { return g_isKiller; }
CMessageEvent* CMessageEventManager::AddMessage(CMessageEventDef* def, const C3DVector* position, CThing* thing1, CThing* thing2, long priority)
{
    if (position == &g_pos && priority == 0) { ++g_messages; g_msgDef = def; g_msgThing1 = thing1; g_msgThing2 = thing2; }
    return 0;
}
CThing* CBaseIntelligentPointer::GetPItem() const { return g_killer; }
long CTCHeroOpinionDeedLog::PostDeed(long deedType, const C3DVector& position, const CThing* target, CThingAICreature* victim,
                                     long postType, bool b1, bool b2, const CFaction* faction, bool b3, long extra)
{
    if (g_deeds < 4 && &position == &g_pos && victim == 0 && !b2 && extra == 0) {
        g_deedTypes[g_deeds] = deedType; g_postTypes[g_deeds] = postType; g_deedFaction[g_deeds] = faction;
        g_deedB1[g_deeds] = b1; g_deedB3[g_deeds] = b3;
    }
    ++g_deeds;
    return 0;
}
void CTCWife::Murdered() { ++g_murdered; }
void CTCWife::Died() { ++g_died; }
const C3DVector& CThingCreatureBase::GetPosition() const { return g_pos; }
void CThingCreatureBase::OnDie() { ++g_baseDie; }

static void Reset()
{
    g_destroyed = g_messages = g_deeds = g_murdered = g_died = g_baseDie = 0;
    g_msgThing1 = g_msgThing2 = 0; g_msgDef = 0;
}

int main()
{
    static CThingAICreature victim;
    static CThing killer;
    static CTCWife wife;
    static CTCFaction factionTC;
    static CTCHeroOpinionDeedLog log;
    static CInterfaceMap::SEntry victimEntries[2];
    static CInterfaceMap::SEntry killerEntries[1];
    static int factionObject;
    g_world.MessageEventManager = &g_events;
    victimEntries[0].type = 0x49; victimEntries[0].component = (CTCBase*)&factionTC;
    victimEntries[1].type = 0xd7; victimEntries[1].component = (CTCBase*)&wife;
    victim.Components.first = victimEntries; victim.Components.last = victimEntries + 2;
    victim.UsingComponents[6] = 1UL << 23;
    victim.UsingComponents[2] = 1UL << 9;
    killerEntries[0].type = 0x51; killerEntries[0].component = (CTCBase*)&log;
    killer.Components.first = killerEntries; killer.Components.last = killerEntries + 1;
    killer.UsingComponents[2] = 1UL << 17;
    factionTC.Faction = (CFaction*)&factionObject;
    bool ok = true;

    // 1: a murdering killer: message, two deeds, wife told of the murder, def released
    g_killer = &killer; g_isKiller = true; g_defFound = true; g_def.m_count = 1;
    Reset(); victim.OnDie();
    ok = ok && g_messages == 1 && g_msgDef == &g_def && g_msgThing1 == &killer && g_msgThing2 == &victim
        && g_deeds == 2 && g_deedTypes[0] == 9 && g_postTypes[0] == 2 && g_deedB1[0] && !g_deedB3[0]
        && g_deedTypes[1] == 0x3a && g_postTypes[1] == 7 && !g_deedB1[1] && g_deedB3[1]
        && g_deedFaction[0] == (CFaction*)&factionObject && g_deedFaction[1] == (CFaction*)&factionObject
        && g_murdered == 1 && g_died == 0 && g_baseDie == 1 && g_destroyed == 1 && g_def.m_count == 0 && g_strings == 0;

    // 2: no killer: the wife is told it died
    g_killer = 0;
    Reset(); victim.OnDie();
    ok = ok && g_messages == 0 && g_deeds == 0 && g_murdered == 0 && g_died == 1 && g_baseDie == 1 && g_destroyed == 0;

    // 3: killer without a deed log that is no killer: no message, died
    killer.UsingComponents[2] = 0; g_killer = &killer; g_isKiller = false;
    Reset(); victim.OnDie();
    ok = ok && g_messages == 0 && g_deeds == 0 && g_died == 1 && g_murdered == 0;

    // 4: deed-log killer, victim without a faction, def kept alive (count 2 -> 1)
    killer.UsingComponents[2] = 1UL << 17; g_isKiller = true; g_def.m_count = 2; factionTC.Faction = 0;
    Reset(); victim.OnDie();
    ok = ok && g_messages == 1 && g_deeds == 0 && g_murdered == 1 && g_destroyed == 0 && g_def.m_count == 1;

    // 5: no wife: nothing told
    victim.UsingComponents[6] = 0; g_defFound = false;
    Reset(); victim.OnDie();
    ok = ok && g_messages == 0 && g_murdered == 0 && g_died == 0 && g_baseDie == 1;

    if (ok) printf("00832e60_TEST PASS\n");
    else printf("00832e60_TEST FAIL\n");
    return ok ? 0 : 1;
}