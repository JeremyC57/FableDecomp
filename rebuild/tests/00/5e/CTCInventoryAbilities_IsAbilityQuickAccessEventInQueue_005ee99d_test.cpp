// Behaviour test for 005ee99d: src/005ee99d.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCInventoryAbilities::IsAbilityQuickAccessEventInQueue(EHeroAbility ability)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventoryAbilities::IsAbilityQuickAccessEventInQueue(EHeroAbility ability) -- true when one of the inventory's quick-access slots holding the ability's quick-access item has a press of type 2 queued for the main player
// Leaf: IsAbilityQuickAccessEventInQueue
// Retail 0x005EE99D, 254 bytes, `ret 4`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: GetAbilityPDef(EHeroAbility) const 0x005EDFD9 (counted CDefPointer by value, built in the dead `ability`
// argument slot), std::bitset::test 0x00410DE0 + the const CVectorMap::LowerBound 0x004365B0 (inline GetInterface for the
// inventory component 0x11), CTCBase::GetPlayerManager() const 0x00686D50, CPlayerManager::GetMainPlayer() const
// 0x00449960, CPlayer::GetPlayerNumber 0x00487510, the main game component's (0x013B86A0) game player interface
// getter 0x00416359, CGamePlayerInterface::IsAssignableSpecialMoveButtonInQueue(long, EInputButtonEventType, long)
// 0x004460C0, the def's slot 1 on release.
// Shape: the def's quick-access item id (+0x98) must be positive; the inventory's quick-access slot vector (+0x148,
// size re-read every pass) is scanned for it and each hit asks the input queue; both exits release the def through
// its inline destructor (the null test folds into the `if (def)`); the call object comes after the argument chain.
// `this` stays in edx across GetAbilityPDef because its real body (byte-exact to 0x005EDFD9, frame omitted) is compiled
// above the caller in the same TU; g_pMainGameComponent is read first through an inline accessor (ebx before the
// argument chain); the vector's start is loaded into a local each pass so the size test and the element read share it.
// Layout: retail CHeroAbilityDef quick-access item +0x98; CTCInventory quick-access slots vector<long> +0x148.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;      // 0x004365B0
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CLongVector {
    long* first;
    long* last;
    long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    long& operator[](unsigned long n) { return *(first + n); }
};
struct CTCInventory {                                // interface type 0x11
    unsigned char _pad_0x000[0x148];
    CLongVector QuickAccessItems;                    // +0x148
};
struct CHeroAbilityDef {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long RefCount;                                   // +0x04
    unsigned char _pad_0x08[0x98 - 0x08];
    long QuickAccessItem;                            // +0x98
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CPlayer {
    long GetPlayerNumber();                          // 0x00487510
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;                  // 0x00449960
};
struct CGamePlayerInterface {
    bool IsAssignableSpecialMoveButtonInQueue(long button, long buttonEventType, long playerNumber);   // 0x004460C0
};
struct CMainGameComponent {
    CGamePlayerInterface* GetGamePlayerInterface(); // 0x00416359
};
extern CMainGameComponent* g_pMainGameComponent;    // 0x013B86A0
__forceinline CMainGameComponent* GetMainGameComponent() { return g_pMainGameComponent; }
enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };

struct SAbilityEntry {                               // std::pair<long, CDefPointer<const CHeroAbilityDef> >
    long                         Level;
    CDefPointer<CHeroAbilityDef> PDef;
};
struct CAbilityVector {                              // STLport vector<SAbilityEntry>
    SAbilityEntry* _M_start;
    SAbilityEntry* _M_finish;
    SAbilityEntry* _M_end_of_storage;
    const SAbilityEntry& operator[](unsigned long n) const { return *(_M_start + n); }
};
struct CTCInventoryAbilities {
    void* __vftable;
    CThing* ParentThing;                             // +0x04
    unsigned char _pad_0x08[0x160 - 0x08];
    CAbilityVector Abilities;                        // +0x160 (PDB +0x180)
    CDefPointer<CHeroAbilityDef> GetAbilityPDef(EHeroAbility ability) const;   // 0x005EDFD9
    const CPlayerManager* GetPlayerManager() const;  // 0x00686D50 (CTCBase)
    bool IsAbilityQuickAccessEventInQueue(EHeroAbility ability);
};

// Same-TU body of GetAbilityPDef @ 0x005EDFD9 (retail's TU, above the caller): it leaves edx alone, so VC7.1 keeps
// `this` there across the call, as retail does.
#pragma optimize("y", on)
__declspec(noinline) CDefPointer<CHeroAbilityDef> CTCInventoryAbilities::GetAbilityPDef(EHeroAbility ability) const
{
    return Abilities[ability].PDef;
}
#pragma optimize("y", off)

bool CTCInventoryAbilities::IsAbilityQuickAccessEventInQueue(EHeroAbility ability)
{
    CDefPointer<CHeroAbilityDef> def = GetAbilityPDef(ability);
    if (def.Object) {
        long item = def->QuickAccessItem;
        if (item > 0) {
            CTCInventory* inventory;
            if (ParentThing->GetInterface(&inventory, 0x11) && inventory) {
                for (unsigned long i = 0; ; ++i) {
                    const long* data = inventory->QuickAccessItems.first;
                    if (i >= (unsigned long)(inventory->QuickAccessItems.last - data))
                        break;
                    if (data[i] == item &&
                        GetMainGameComponent()->GetGamePlayerInterface()->IsAssignableSpecialMoveButtonInQueue(
                            i, 2, GetPlayerManager()->GetMainPlayer()->GetPlayerNumber()))
                        return true;
                }
            }
        }
    }
    return false;
}

// ---- stubs and driver ----
#include <cstdlib>
static bool g_hasInventory = true; static int g_queries = 0, g_destroys = 0; static long g_lastButton = -1, g_lastType = -1, g_lastPlayer = -1;
static bool g_queued[8];
bool CPresenceBits::test(long bit) { return bit == 0x11 && g_hasInventory; }
static CInterfaceMap::SEntry g_entries[2];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    for (SEntry* it = first; it != last; ++it) if (!(it->type < *type)) return it;
    return last;
}
long CPlayer::GetPlayerNumber() { return 3; }
static CPlayer g_player;
CPlayer* CPlayerManager::GetMainPlayer() const { return &g_player; }
static CPlayerManager g_players;
const CPlayerManager* CTCInventoryAbilities::GetPlayerManager() const { return &g_players; }
bool CGamePlayerInterface::IsAssignableSpecialMoveButtonInQueue(long button, long type, long player)
{ ++g_queries; g_lastButton = button; g_lastType = type; g_lastPlayer = player; return g_queued[button]; }
static CGamePlayerInterface g_gpi;
CGamePlayerInterface* CMainGameComponent::GetGamePlayerInterface() { return &g_gpi; }
static CMainGameComponent g_game;
CMainGameComponent* g_pMainGameComponent = &g_game;
void CHeroAbilityDef::v00() {}
void CHeroAbilityDef::Destroy() { ++g_destroys; }
int main()
{
    CHeroAbilityDef* def = new CHeroAbilityDef; def->RefCount = 1; def->QuickAccessItem = 42;
    SAbilityEntry* abilities = (SAbilityEntry*)calloc(4, sizeof(SAbilityEntry));
    abilities[2].PDef.Object = def;
    CTCInventory* inventory = (CTCInventory*)calloc(1, sizeof(CTCInventory));
    long slots[4] = { 7, 42, 9, 42 };
    inventory->QuickAccessItems.first = slots; inventory->QuickAccessItems.last = slots + 4; inventory->QuickAccessItems.end_of_storage = slots + 4;
    CThing* thing = (CThing*)calloc(1, sizeof(CThing));
    g_entries[0].type = 0x11; g_entries[0].component = inventory; g_entries[1].type = 0x20; g_entries[1].component = 0;
    thing->m_interfaces.first = g_entries; thing->m_interfaces.last = g_entries + 2;
    CTCInventoryAbilities* abil = (CTCInventoryAbilities*)calloc(1, sizeof(CTCInventoryAbilities));
    abil->ParentThing = thing; abil->Abilities._M_start = abilities; abil->Abilities._M_finish = abilities + 4;
    // no press queued on either matching slot: false, two queries (slots 1 and 3), the def reference released
    CHECK(!abil->IsAbilityQuickAccessEventInQueue((EHeroAbility)2));
    CHECK(g_queries == 2 && g_lastButton == 3 && g_lastType == 2 && g_lastPlayer == 3 && def->RefCount == 1 && g_destroys == 0);
    // queued on slot 3
    g_queued[3] = true;
    CHECK(abil->IsAbilityQuickAccessEventInQueue((EHeroAbility)2) && g_queries == 4 && def->RefCount == 1);
    // no def for the ability / non-positive item / no inventory: false without queries
    CHECK(!abil->IsAbilityQuickAccessEventInQueue((EHeroAbility)1) && g_queries == 4);
    def->QuickAccessItem = 0;
    CHECK(!abil->IsAbilityQuickAccessEventInQueue((EHeroAbility)2) && g_queries == 4 && def->RefCount == 1);
    def->QuickAccessItem = 42; g_hasInventory = false;
    CHECK(!abil->IsAbilityQuickAccessEventInQueue((EHeroAbility)2) && g_queries == 4);
    // the last reference dies with the call: Destroy through slot 1
    g_hasInventory = true; def->RefCount = 0;
    CHECK(abil->IsAbilityQuickAccessEventInQueue((EHeroAbility)2) && g_destroys == 1 && def->RefCount == 0);
    if (fails == 0) printf("005ee99d_TEST PASS\n");
    return fails;
}
