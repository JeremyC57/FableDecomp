// Behaviour test for 0062aba0: final.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// CWorld::EAUseQuickAccessItem(const CGameEvent*)
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CWorld::EAUseQuickAccessItem(const CGameEvent*) -- game-event action: unless the player is in mode 0x2F or its controlled creature's interface 0xA7 blocks it, read the slot from the event and have the hero's inventory use it (-2 = the guild seal, context-sensitive slots first, then quick-access slots)
// Leaf: EAUseQuickAccessItem
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062ABA0, 491 bytes (+ int3 padding), `ret 4`, /GS (cookie 0x0139C8A8).
//   PDB ?EAUseQuickAccessItem@CWorld@@QAEXPBVCGameEvent@@@Z.
// Callees: CWorld::GetPlayer(long) 0x0049D850 (x2), CPlayer::HasMode 0x00634160, CPlayer::GetControlledCreature
//   0x00487DD0 (x2), CPlayer::GetHeroCreature 0x00487DC0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x004365B0
//   (interface 0xA7) and 0x0040F020 (interface 0x11 = CTCInventory) -- distinct retail copies, CGameEvent::GetData
//   0x009F17A0, CMemoryBuffer::Read 0x009F18F0, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0
//   ("OBJECT_GUILD_SEAL_1" 0x01251B4C), CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410,
//   CGamePlayerInterface::GetNumContextSensitiveSlots 0x00445960 (x2), CTCInventory::OnItemSelected (vtable slot 33,
//   +0x84), UseContextSensitiveItem 0x00583A92, UseQuickAccessItem 0x005840A6, the current action's slot 39 (+0x9C),
//   __security_check_cookie 0x00BFE9F9.
// Layout: local models at retail offsets, as the landed siblings EAUseQuickAccessItemInCutscene 0x0062AD90 and
//   EAChargeQuickAccessItem 0x00629B10 (engine/ headers carry the PDB's 16-byte containers): CThing interface bitset
//   +0x20, interface map +0x44, Dead +0x91 bit 0, CThingCreatureBase PCurrentAction +0x100 (PDB +0x11C); CPlayer
//   PlayerInterface +0xC; CWorld DefinitionManager +0x10.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CMemoryBuffer {
    unsigned char Data[0x20];                      // +0x00
    long          Length;                          // +0x20
    long          ReadPos;                         // +0x24
    long          WritePos;                        // +0x28
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data);   // 0x009F18F0
    __forceinline long ReadLong() { long value; Read(4, &value); return value; }
};
class CGameEvent { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    long GetData(CMemoryBuffer& buffer) const;     // 0x009F17A0
};

class CTCBase;
struct CInterfaceMap {                             // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x0040F020
    SEntry* LowerBound2(const long* type);         // 0x004365B0 (a second retail copy)
};
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

class CCreatureActionBase { public:
    virtual void Slot00(); virtual void Slot01(); virtual void Slot02(); virtual void Slot03(); virtual void Slot04();
    virtual void Slot05(); virtual void Slot06(); virtual void Slot07(); virtual void Slot08(); virtual void Slot09();
    virtual void Slot10(); virtual void Slot11(); virtual void Slot12(); virtual void Slot13(); virtual void Slot14();
    virtual void Slot15(); virtual void Slot16(); virtual void Slot17(); virtual void Slot18(); virtual void Slot19();
    virtual void Slot20(); virtual void Slot21(); virtual void Slot22(); virtual void Slot23(); virtual void Slot24();
    virtual void Slot25(); virtual void Slot26(); virtual void Slot27(); virtual void Slot28(); virtual void Slot29();
    virtual void Slot30(); virtual void Slot31(); virtual void Slot32(); virtual void Slot33(); virtual void Slot34();
    virtual void Slot35(); virtual void Slot36(); virtual void Slot37(); virtual void Slot38();
    virtual bool BlocksQuickAccess();              // slot 39 (+0x9C)
};

struct CTCInterfaceA7 {                            // interface 0xA7: a flag at +0xC vetoes item use
    unsigned char _pad[0xc];
    bool Blocking;                                 // +0xC
};

class CTCInventory { public:
    virtual void Slot00(); virtual void Slot01(); virtual void Slot02(); virtual void Slot03(); virtual void Slot04();
    virtual void Slot05(); virtual void Slot06(); virtual void Slot07(); virtual void Slot08(); virtual void Slot09();
    virtual void Slot10(); virtual void Slot11(); virtual void Slot12(); virtual void Slot13(); virtual void Slot14();
    virtual void Slot15(); virtual void Slot16(); virtual void Slot17(); virtual void Slot18(); virtual void Slot19();
    virtual void Slot20(); virtual void Slot21(); virtual void Slot22(); virtual void Slot23(); virtual void Slot24();
    virtual void Slot25(); virtual void Slot26(); virtual void Slot27(); virtual void Slot28(); virtual void Slot29();
    virtual void Slot30(); virtual void Slot31(); virtual void Slot32();
    virtual void OnItemSelected(long defIndex, bool* handled);   // slot 33 (+0x84)
    void UseContextSensitiveItem(long slot);       // 0x00583A92
    void UseQuickAccessItem(long slot);            // 0x005840A6
};

class CThingCreatureBase { public:
    void* m_pVtable;
    unsigned char _pad04[0x20 - 4];
    unsigned long UsingComponents[9];              // +0x20 bitset<274>
    CInterfaceMap Components;                      // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                        // +0x91 bit 0
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template<class T> __forceinline T* GetTC2(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound2(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    unsigned char _pad92[0x100 - 0x92];
    CCreatureActionBase* PCurrentAction;           // +0x100 CCountedPointer<CCreatureActionBase>::Data
};

class CGamePlayerInterface { public:
    unsigned long GetNumContextSensitiveSlots() const;   // 0x00445960
};
enum EPlayerMode { PLAYER_MODE_0x2F = 0x2f };
class CPlayer { public:
    unsigned char _pad00[0xc];
    CGamePlayerInterface* PlayerInterface;         // +0xC
    bool HasMode(EPlayerMode mode);                // 0x00634160
    CThingCreatureBase* GetHeroCreature();         // 0x00487DC0
    CThingCreatureBase* GetControlledCreature();   // 0x00487DD0
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
class CWorld { public:
    unsigned char _pad00[0x10];
    CDefinitionManager* DefinitionManager;         // +0x10
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EAUseQuickAccessItem(const CGameEvent* event);
};

void CWorld::EAUseQuickAccessItem(const CGameEvent* event)
{
    CPlayer* eventPlayer = GetPlayer(event->Player);
    if (eventPlayer->HasMode(PLAYER_MODE_0x2F))
        return;
    CPlayer* controller = GetPlayer(event->Player);
    CThingCreatureBase* controlled = controller->GetControlledCreature();
    if (controlled && !controlled->Dead) {
        CThingCreatureBase* creature = controller->GetControlledCreature();
        if (creature->HasTC(0xA7) && creature->GetTC2<CTCInterfaceA7>(0xA7)->Blocking)
            return;
    }
    long number = event->Player;
    CPlayer* player = GetPlayer(number);
    CThingCreatureBase* hero = player->GetHeroCreature();
    if (!hero || hero->Dead)
        return;
    if (hero->PCurrentAction && hero->PCurrentAction->BlocksQuickAccess())
        return;
    CMemoryBuffer buffer;
    event->GetData(buffer);
    if (!hero->HasTC(0x11))
        return;
    CTCInventory* inventory = hero->GetTC<CTCInventory>(0x11);
    if (!inventory)
        return;
    long slot = buffer.ReadLong();
    if (slot == -2) {
        long defIndex = DefinitionManager->GetDefGlobalIndexFromName("OBJECT_GUILD_SEAL_1");
        bool handled;
        inventory->OnItemSelected(defIndex, &handled);
        return;
    }
    if (slot > -1) {
        if (slot < (long)player->PlayerInterface->GetNumContextSensitiveSlots())
            inventory->UseContextSensitiveItem(slot);
        else
            inventory->UseQuickAccessItem(slot - player->PlayerInterface->GetNumContextSensitiveSlots());
    }
}

// ---- stubs and driver ----
void CCreatureActionBase::Slot00() { }
void CCreatureActionBase::Slot01() { }
void CCreatureActionBase::Slot02() { }
void CCreatureActionBase::Slot03() { }
void CCreatureActionBase::Slot04() { }
void CCreatureActionBase::Slot05() { }
void CCreatureActionBase::Slot06() { }
void CCreatureActionBase::Slot07() { }
void CCreatureActionBase::Slot08() { }
void CCreatureActionBase::Slot09() { }
void CCreatureActionBase::Slot10() { }
void CCreatureActionBase::Slot11() { }
void CCreatureActionBase::Slot12() { }
void CCreatureActionBase::Slot13() { }
void CCreatureActionBase::Slot14() { }
void CCreatureActionBase::Slot15() { }
void CCreatureActionBase::Slot16() { }
void CCreatureActionBase::Slot17() { }
void CCreatureActionBase::Slot18() { }
void CCreatureActionBase::Slot19() { }
void CCreatureActionBase::Slot20() { }
void CCreatureActionBase::Slot21() { }
void CCreatureActionBase::Slot22() { }
void CCreatureActionBase::Slot23() { }
void CCreatureActionBase::Slot24() { }
void CCreatureActionBase::Slot25() { }
void CCreatureActionBase::Slot26() { }
void CCreatureActionBase::Slot27() { }
void CCreatureActionBase::Slot28() { }
void CCreatureActionBase::Slot29() { }
void CCreatureActionBase::Slot30() { }
void CCreatureActionBase::Slot31() { }
void CCreatureActionBase::Slot32() { }
void CCreatureActionBase::Slot33() { }
void CCreatureActionBase::Slot34() { }
void CCreatureActionBase::Slot35() { }
void CCreatureActionBase::Slot36() { }
void CCreatureActionBase::Slot37() { }
void CCreatureActionBase::Slot38() { }
void CTCInventory::Slot00() { }
void CTCInventory::Slot01() { }
void CTCInventory::Slot02() { }
void CTCInventory::Slot03() { }
void CTCInventory::Slot04() { }
void CTCInventory::Slot05() { }
void CTCInventory::Slot06() { }
void CTCInventory::Slot07() { }
void CTCInventory::Slot08() { }
void CTCInventory::Slot09() { }
void CTCInventory::Slot10() { }
void CTCInventory::Slot11() { }
void CTCInventory::Slot12() { }
void CTCInventory::Slot13() { }
void CTCInventory::Slot14() { }
void CTCInventory::Slot15() { }
void CTCInventory::Slot16() { }
void CTCInventory::Slot17() { }
void CTCInventory::Slot18() { }
void CTCInventory::Slot19() { }
void CTCInventory::Slot20() { }
void CTCInventory::Slot21() { }
void CTCInventory::Slot22() { }
void CTCInventory::Slot23() { }
void CTCInventory::Slot24() { }
void CTCInventory::Slot25() { }
void CTCInventory::Slot26() { }
void CTCInventory::Slot27() { }
void CTCInventory::Slot28() { }
void CTCInventory::Slot29() { }
void CTCInventory::Slot30() { }
void CTCInventory::Slot31() { }
void CTCInventory::Slot32() { }
static int g_hasMode = 0, g_getPlayer = 0, g_getControlled = 0, g_getHero = 0, g_getData = 0, g_read = 0, g_lb2 = 0;
static int g_blocks = 0, g_ctor = 0, g_dtor = 0, g_defName = 0, g_onItem = 0, g_useCS = 0, g_useQA = 0, g_numSlots = 0;
static bool g_modeResult = false, g_blocksResult = false;
static long g_lastPlayerNumber = -99, g_readValue = 0, g_lastSlot = -99, g_lastDef = -99, g_lastOnItemDef = -99;
static bool* g_lastHandled = 0;
static const char* g_lastDefName = 0;
static unsigned long g_numCS = 0;
static CPlayer* g_player = 0;
static CThingCreatureBase* g_controlled = 0;
static CThingCreatureBase* g_hero = 0;
static CMemoryBuffer* g_lastBuffer = 0;
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; ++g_ctor; CHECK(length == -1); }
CCharString::~CCharString() { ++g_dtor; }
void CMemoryBuffer::Read(unsigned long length, void* data) { ++g_read; CHECK(length == 4); CHECK(this == g_lastBuffer); *(long*)data = g_readValue; }
long CGameEvent::GetData(CMemoryBuffer& buffer) const { ++g_getData; g_lastBuffer = &buffer; CHECK(buffer.Length == 0 && buffer.ReadPos == 0 && buffer.WritePos == 0); return 0; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound2(const long* type)
{
    ++g_lb2; CHECK(*type == 0xA7);
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
bool CCreatureActionBase::BlocksQuickAccess() { ++g_blocks; return g_blocksResult; }
void CTCInventory::OnItemSelected(long defIndex, bool* handled) { ++g_onItem; g_lastOnItemDef = defIndex; g_lastHandled = handled; *handled = true; }
void CTCInventory::UseContextSensitiveItem(long slot) { ++g_useCS; g_lastSlot = slot; }
void CTCInventory::UseQuickAccessItem(long slot) { ++g_useQA; g_lastSlot = slot; }
unsigned long CGamePlayerInterface::GetNumContextSensitiveSlots() const { ++g_numSlots; return g_numCS; }
bool CPlayer::HasMode(EPlayerMode mode) { ++g_hasMode; CHECK(mode == PLAYER_MODE_0x2F); CHECK(this == g_player); return g_modeResult; }
CThingCreatureBase* CPlayer::GetHeroCreature() { ++g_getHero; CHECK(this == g_player); return g_hero; }
CThingCreatureBase* CPlayer::GetControlledCreature() { ++g_getControlled; CHECK(this == g_player); return g_controlled; }
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) { ++g_defName; g_lastDefName = (const char*)name.PStringData; return 0x1234; }
CPlayer* CWorld::GetPlayer(long number) { ++g_getPlayer; g_lastPlayerNumber = number; return g_player; }

static unsigned char g_playerBuf[0x40];
static unsigned char g_worldBuf[0x40];
static unsigned char g_creatureBuf[0x200];
static unsigned char g_heroBuf[0x200];
static CGamePlayerInterface g_iface;
static CDefinitionManager g_defs;
static CTCInterfaceA7 g_a7;
static CTCInventory g_inventory;
static CCreatureActionBase g_action;
static CInterfaceMap::SEntry g_creatureEntries[2];
static CInterfaceMap::SEntry g_heroEntries[2];

static void reset()
{
    g_hasMode = g_getPlayer = g_getControlled = g_getHero = g_getData = g_read = g_lb2 = 0;
    g_blocks = g_ctor = g_dtor = g_defName = g_onItem = g_useCS = g_useQA = g_numSlots = 0;
    g_lastPlayerNumber = -99; g_lastSlot = -99; g_lastOnItemDef = -99; g_lastHandled = 0; g_lastDefName = 0; g_lastBuffer = 0;
    memset(g_creatureBuf, 0, sizeof(g_creatureBuf)); memset(g_heroBuf, 0, sizeof(g_heroBuf));
    CThingCreatureBase* creature = (CThingCreatureBase*)g_creatureBuf;
    CThingCreatureBase* hero = (CThingCreatureBase*)g_heroBuf;
    g_creatureEntries[0].type = 0x11; g_creatureEntries[0].component = 0;
    g_creatureEntries[1].type = 0xA7; g_creatureEntries[1].component = (CTCBase*)&g_a7;
    creature->Components.first = g_creatureEntries; creature->Components.last = g_creatureEntries + 2;
    creature->UsingComponents[0xA7 >> 5] |= 1UL << (0xA7 & 31);
    g_a7.Blocking = false;
    g_heroEntries[0].type = 0x11; g_heroEntries[0].component = (CTCBase*)&g_inventory;
    g_heroEntries[1].type = 0xA7; g_heroEntries[1].component = 0;
    hero->Components.first = g_heroEntries; hero->Components.last = g_heroEntries + 2;
    hero->UsingComponents[0x11 >> 5] |= 1UL << (0x11 & 31);
    hero->PCurrentAction = 0;
    g_controlled = creature; g_hero = hero;
    g_modeResult = false; g_blocksResult = false; g_numCS = 3; g_readValue = 1;
}

int main()
{
    memset(g_playerBuf, 0, sizeof(g_playerBuf)); memset(g_worldBuf, 0, sizeof(g_worldBuf));
    g_player = (CPlayer*)g_playerBuf; g_player->PlayerInterface = &g_iface;
    CWorld* world = (CWorld*)g_worldBuf; world->DefinitionManager = &g_defs;
    CGameEvent ev; memset(&ev, 0, sizeof(ev)); ev.Player = 2;
    CThingCreatureBase* creature = (CThingCreatureBase*)g_creatureBuf;
    CThingCreatureBase* hero = (CThingCreatureBase*)g_heroBuf;

    // 1. player mode 0x2F vetoes everything
    reset(); g_modeResult = true;
    world->EAUseQuickAccessItem(&ev);
    CHECK(g_getPlayer == 1 && g_lastPlayerNumber == 2 && g_hasMode == 1 && g_getControlled == 0 && g_getHero == 0 && g_getData == 0);

    // 2. context-sensitive slot: slot 1 < 3 slots
    reset(); g_readValue = 1;
    world->EAUseQuickAccessItem(&ev);
    CHECK(g_getPlayer == 3 && g_getControlled == 2 && g_lb2 == 1 && g_getHero == 1 && g_getData == 1 && g_read == 1);
    CHECK(g_numSlots == 1 && g_useCS == 1 && g_lastSlot == 1 && g_useQA == 0 && g_onItem == 0 && g_blocks == 0);

    // 3. quick-access slot: slot 5 >= 3 slots -> UseQuickAccessItem(2), the slot count read twice
    reset(); g_readValue = 5;
    world->EAUseQuickAccessItem(&ev);
    CHECK(g_numSlots == 2 && g_useQA == 1 && g_lastSlot == 2 && g_useCS == 0);

    // 4. the guild seal (-2): def lookup by name, OnItemSelected with a handled out-pointer, string destroyed
    reset(); g_readValue = -2;
    world->EAUseQuickAccessItem(&ev);
    CHECK(g_defName == 1 && g_lastDefName && strcmp(g_lastDefName, "OBJECT_GUILD_SEAL_1") == 0 && g_ctor == 1 && g_dtor == 1);
    CHECK(g_onItem == 1 && g_lastOnItemDef == 0x1234 && g_lastHandled != 0 && g_useCS == 0 && g_useQA == 0 && g_numSlots == 0);

    // 5. a negative slot other than -2 does nothing
    reset(); g_readValue = -3;
    world->EAUseQuickAccessItem(&ev);
    CHECK(g_read == 1 && g_onItem == 0 && g_useCS == 0 && g_useQA == 0 && g_numSlots == 0);

    // 6. the controlled creature's 0xA7 interface blocks the use
    reset(); g_a7.Blocking = true;
    world->EAUseQuickAccessItem(&ev);
    CHECK(g_getControlled == 2 && g_lb2 == 1 && g_getHero == 0 && g_getData == 0 && g_getPlayer == 2);

    // 7. controlled creature without the 0xA7 interface: no lookup, use proceeds
    reset(); creature->UsingComponents[0xA7 >> 5] = 0;
    world->EAUseQuickAccessItem(&ev);
    CHECK(g_lb2 == 0 && g_getControlled == 2 && g_useCS == 1);

    // 8. dead controlled creature: skipped (one GetControlledCreature call), use proceeds
    reset(); creature->Dead = 1;
    world->EAUseQuickAccessItem(&ev);
    CHECK(g_getControlled == 1 && g_lb2 == 0 && g_useCS == 1);

    // 9. no controlled creature
    reset(); g_controlled = 0;
    world->EAUseQuickAccessItem(&ev);
    CHECK(g_getControlled == 1 && g_lb2 == 0 && g_useCS == 1);

    // 10. no hero / dead hero
    reset(); g_hero = 0;
    world->EAUseQuickAccessItem(&ev);
    CHECK(g_getHero == 1 && g_getData == 0 && g_useCS == 0);
    reset(); hero->Dead = 1;
    world->EAUseQuickAccessItem(&ev);
    CHECK(g_getHero == 1 && g_getData == 0 && g_useCS == 0);

    // 11. the current action vetoes (slot 39) / allows
    reset(); hero->PCurrentAction = &g_action; g_blocksResult = true;
    world->EAUseQuickAccessItem(&ev);
    CHECK(g_blocks == 1 && g_getData == 0 && g_useCS == 0);
    reset(); hero->PCurrentAction = &g_action; g_blocksResult = false;
    world->EAUseQuickAccessItem(&ev);
    CHECK(g_blocks == 1 && g_getData == 1 && g_useCS == 1);

    // 12. the hero lacks the inventory interface: event data is read into the buffer, then nothing
    reset(); hero->UsingComponents[0x11 >> 5] = 0;
    world->EAUseQuickAccessItem(&ev);
    CHECK(g_getData == 1 && g_read == 0 && g_useCS == 0);

    // 13. the inventory component is null
    reset(); g_heroEntries[0].component = 0;
    world->EAUseQuickAccessItem(&ev);
    CHECK(g_getData == 1 && g_read == 0 && g_useCS == 0);

    if (fails == 0) printf("0062aba0_TEST PASS\n"); else printf("0062aba0_TEST FAIL\n");
    return fails;
}