// Behaviour test for 0x0082DB00 (copy of the model + stand-in callee bodies).
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __thiscall
#define __forceinline inline
#define __declspec(x)
#endif
// CThingBuilding::LockAllDoors(bool lock, bool checkBlocking) -- walks the building's door list and opens or closes
// (locks) every live door thing whose door component is lockable by its owner; a door the main player is standing near is
// left alone (unless the player is inside this building), and a closing door that would block the player stays open
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082DB00, `ret 8`.
// Layout: CThing presence bits +0x20, Components +0x44, Dead +0x91 bit 0; door-list component keeps its list at +0x0C;
//         CTCDoor +0x0C owner-lock word (descriptive).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
    SEntry* LowerBound(const long* type) const;      // 0x004365B0
};
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
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
struct CThing;
struct CIntelligentPointer {
    unsigned char _pad00[8];
    CThing* GetPItem() const;                        // 0x00A01B50
};
struct SDoorNode {
    SDoorNode*          _M_next;
    SDoorNode*          _M_prev;
    CIntelligentPointer _M_data;                     // +0x08
};
struct CTCDoor {                                     // interface type 0x26
    unsigned char _pad00[0x0c];
    long          OwnerLock;                         // +0x0c (descriptive)
    bool IsLockableByOwner();                        // 0x00730610
    bool IsClosed();                                 // 0x0082CF40
    void ScriptTriggerOpen(bool open);               // 0x007305D0
};
struct CTCDoorList {                                 // interface type 0x21
    unsigned char _pad00[0x0c];
    SDoorNode*    Doors;                             // +0x0c
};
struct CTCCurrentBuilding {                          // interface type 0x75 (descriptive)
    unsigned char _pad00[0x0c];
    CIntelligentPointer Building;                    // +0x0c
};
struct C3DVector { float X, Y, Z; };
struct CTCSwitchableNavigation {                     // interface type 0x79
    bool WouldBlockPos(const C3DVector& pos);        // 0x00787620
};
struct CPlayer;
struct CThingCreatureBase;
struct CPlayerManager {
    const CPlayer* GetMainPlayer() const;            // 0x00449960
    CPlayer* GetMainPlayer();                        // 0x00449970
};
struct CPlayer {
    const CThingCreatureBase* GetCharacter() const;  // 0x00487DD0
    CThingCreatureBase* GetCharacter();              // 0x00487DC0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                  // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                          // +0x91 bit 0
    const CPlayerManager* GetPlayerManager() const;  // 0x004C79F0
    CPlayerManager* GetPlayerManager();              // 0x004C7A00
    const C3DVector& GetPosition() const;            // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31))) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(const T** out, long wanted) const
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31))) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (const T*)it->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing {};
extern const float G_DoorPlayerRadiusSq;             // 0x0125D108
struct CThingBuilding : CThing {
    void LockAllDoors(bool lock, bool checkBlocking);
};
void CThingBuilding::LockAllDoors(bool lock, bool checkBlocking)
{
    CTCDoorList* doors;
    if (GetInterface(&doors, 0x21)) {
        for (SDoorNode* node = doors->Doors->_M_next; node != doors->Doors; node = node->_M_next) {
            const CIntelligentPointer& door = node->_M_data;
            CTCDoor* tc;
            if (door.GetPItem() && !door.GetPItem()->Dead && door.GetPItem() && !door.GetPItem()->Dead
                && door.GetPItem()->GetInterface(&tc, 0x26)) {
                if (!tc->IsLockableByOwner())
                    continue;
                const CThingCreatureBase* hero = GetPlayerManager()->GetMainPlayer()->GetCharacter();
                if (hero && !hero->Dead && lock && tc->OwnerLock) {
                    const C3DVector& a = door.GetPItem()->GetPosition();
                    const C3DVector& b = hero->GetPosition();
                    float dx = b.X - a.X;
                    float dy = b.Y - a.Y;
                    float dz = b.Z - a.Z;
                    if (dz * dz + dy * dy + dx * dx < G_DoorPlayerRadiusSq)
                        continue;
                    const CTCCurrentBuilding* current;
                    if (hero->GetInterface(&current, 0x75) && current->Building.GetPItem() == this)
                        continue;
                    tc->OwnerLock = 0;
                }
                if (!tc->IsClosed() && lock && checkBlocking) {
                    CThingCreatureBase* player = GetPlayerManager()->GetMainPlayer()->GetCharacter();
                    CTCSwitchableNavigation* nav;
                    if (door.GetPItem()->GetInterface(&nav, 0x79) && nav->WouldBlockPos(player->GetPosition()))
                        continue;
                }
                tc->ScriptTriggerOpen(!lock);
            }
        }
    }
}

CThing* CIntelligentPointer::GetPItem() const { return *(CThing* const*)_pad00; }
static int g_lockable = 1, g_closed = 0, g_block = 0;
static int g_opens = 0; static int g_lastOpen = -1; static CTCDoor* g_lastDoor = 0;
bool CTCDoor::IsLockableByOwner() { return g_lockable != 0; }
bool CTCDoor::IsClosed() { return g_closed != 0; }
void CTCDoor::ScriptTriggerOpen(bool open) { ++g_opens; g_lastOpen = open ? 1 : 0; g_lastDoor = this; }
static const C3DVector* g_blockPos = 0;
bool CTCSwitchableNavigation::WouldBlockPos(const C3DVector& pos) { g_blockPos = &pos; return g_block != 0; }
static CThingCreatureBase g_hero; static CThingCreatureBase* g_heroPtr = &g_hero;
static CPlayer g_player; static CPlayerManager g_pm;
const CPlayerManager* CThing::GetPlayerManager() const { return &g_pm; }
CPlayerManager* CThing::GetPlayerManager() { return &g_pm; }
const CPlayer* CPlayerManager::GetMainPlayer() const { return &g_player; }
CPlayer* CPlayerManager::GetMainPlayer() { return &g_player; }
const CThingCreatureBase* CPlayer::GetCharacter() const { return g_heroPtr; }
CThingCreatureBase* CPlayer::GetCharacter() { return g_heroPtr; }
static C3DVector g_heroPos = { 0.0f, 0.0f, 0.0f }; static C3DVector g_doorPos = { 10.0f, 0.0f, 0.0f };
const C3DVector& CThing::GetPosition() const { return this == &g_hero ? g_heroPos : g_doorPos; }
const float G_DoorPlayerRadiusSq = 25.0f;
static void SetBit(CThing* t, long k) { t->m_presentBits[k >> 5] |= 1UL << (k & 31); }
static void SetPtr(CIntelligentPointer& p, CThing* t) { *(CThing**)p._pad00 = t; }

int main()
{
    static CThingBuilding b; static CThing doorThing; static CTCDoor tc; static CTCDoorList list;
    static CTCSwitchableNavigation nav; static CTCCurrentBuilding cur;
    std::memset(&b, 0, sizeof(b)); std::memset(&doorThing, 0, sizeof(doorThing)); std::memset(&tc, 0, sizeof(tc));
    std::memset(&g_hero, 0, sizeof(g_hero)); std::memset(&cur, 0, sizeof(cur));
    SDoorNode head; SDoorNode n1;
    head._M_next = &n1; head._M_prev = &n1; n1._M_next = &head; n1._M_prev = &head;
    std::memset(&n1._M_data, 0, sizeof(n1._M_data)); SetPtr(n1._M_data, &doorThing);
    list.Doors = &head;
    CInterfaceMap::SEntry be[1]; be[0].type = 0x21; be[0].component = (CTCBase*)&list;
    b.m_interfaces.first = be; b.m_interfaces.last = be + 1;
    CInterfaceMap::SEntry de[2]; de[0].type = 0x26; de[0].component = (CTCBase*)&tc;
    de[1].type = 0x79; de[1].component = (CTCBase*)&nav;
    doorThing.m_interfaces.first = de; doorThing.m_interfaces.last = de + 2;
    CInterfaceMap::SEntry he[1]; he[0].type = 0x75; he[0].component = (CTCBase*)&cur;
    g_hero.m_interfaces.first = he; g_hero.m_interfaces.last = he + 1;
    // no door-list interface -> nothing
    b.LockAllDoors(true, true);
    if (g_opens != 0) { std::printf("0082db00_TEST FAIL\n"); return 1; }
    SetBit(&b, 0x21);
    // door without door component -> skipped
    b.LockAllDoors(false, false);
    if (g_opens != 0) { std::printf("0082db00_TEST FAIL\n"); return 1; }
    SetBit(&doorThing, 0x26);
    // unlock: opens
    b.LockAllDoors(false, true);
    if (g_opens != 1 || g_lastOpen != 1 || g_lastDoor != &tc) { std::printf("0082db00_TEST FAIL\n"); return 1; }
    // dead door -> skipped
    doorThing.Dead = 1; b.LockAllDoors(true, false); if (g_opens != 1) { std::printf("0082db00_TEST FAIL\n"); return 1; } doorThing.Dead = 0;
    // not lockable -> skipped
    g_lockable = 0; b.LockAllDoors(true, false); if (g_opens != 1) { std::printf("0082db00_TEST FAIL\n"); return 1; } g_lockable = 1;
    // lock, owner lock set, hero far away -> clears owner lock and closes
    tc.OwnerLock = 7;
    b.LockAllDoors(true, false);
    if (g_opens != 2 || g_lastOpen != 0 || tc.OwnerLock != 0) { std::printf("0082db00_TEST FAIL\n"); return 1; }
    // hero near -> skipped entirely
    tc.OwnerLock = 7; g_heroPos.X = 8.0f;
    b.LockAllDoors(true, false);
    if (g_opens != 2 || tc.OwnerLock != 7) { std::printf("0082db00_TEST FAIL\n"); return 1; }
    g_heroPos.X = 0.0f;
    // hero far but inside this building -> skipped
    SetBit(&g_hero, 0x75); SetPtr(cur.Building, &b);
    b.LockAllDoors(true, false);
    if (g_opens != 2 || tc.OwnerLock != 7) { std::printf("0082db00_TEST FAIL\n"); return 1; }
    // hero inside another building -> cleared and closed
    SetPtr(cur.Building, &doorThing);
    b.LockAllDoors(true, false);
    if (g_opens != 3 || tc.OwnerLock != 0) { std::printf("0082db00_TEST FAIL\n"); return 1; }
    // dead hero -> owner lock untouched, still closes
    tc.OwnerLock = 7; g_hero.Dead = 1;
    b.LockAllDoors(true, false);
    if (g_opens != 4 || tc.OwnerLock != 7) { std::printf("0082db00_TEST FAIL\n"); return 1; }
    g_hero.Dead = 0; tc.OwnerLock = 0;
    // blocking check: no nav interface -> closes
    b.LockAllDoors(true, true);
    if (g_opens != 5 || g_blockPos != 0) { std::printf("0082db00_TEST FAIL\n"); return 1; }
    SetBit(&doorThing, 0x79);
    g_block = 1; b.LockAllDoors(true, true);
    if (g_opens != 5 || g_blockPos != &g_heroPos) { std::printf("0082db00_TEST FAIL\n"); return 1; }
    g_block = 0; b.LockAllDoors(true, true);
    if (g_opens != 6) { std::printf("0082db00_TEST FAIL\n"); return 1; }
    // already closed -> no blocking test
    g_closed = 1; g_block = 1; b.LockAllDoors(true, true);
    if (g_opens != 7 || g_lastOpen != 0) { std::printf("0082db00_TEST FAIL\n"); return 1; }
    std::printf("0082db00_TEST PASS\n");
    return 0;
}
