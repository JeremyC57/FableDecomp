// Behaviour test for 0064e0da: body copied from src/0064e0da.cpp (NPlayerGui::CDrawGuildSeal::AddMessage), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1) : PStringData((void*)text) {}
    CCharString(const CCharString& other) : PStringData(other.PStringData) {}
    ~CCharString() {}
};
struct CTCBase;
struct CPresenceBits {
    unsigned long w[9];
    bool test(long bit) { return (w[bit >> 5] & (1UL << (bit & 31))) != 0; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    unsigned char _pad0[0x20];
    CPresenceBits m_present;
    CInterfaceMap m_interfaces;
    bool alive;
    bool HasInterface(long type) { return m_present.test(type); }
    template <class T> __forceinline T* GetInterfaceUnchecked(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing) { return thing != 0 && thing->alive; }
struct CThingCreatureBase : CThing { bool child; bool IsChild() { return child; } };
struct CPlayer { CThingCreatureBase* creature; CThingCreatureBase* GetControlledCreature() { return creature; } };
struct CPlayerManager { CPlayer player; CPlayer* GetMainPlayer() { return &player; } };
static long g_sealIndex = 0; static const char* g_nameSeen = 0; static int g_nameCalls = 0;
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name) { ++g_nameCalls; g_nameSeen = (const char*)name.PStringData; return g_sealIndex; }
};
struct CDefinitionOwner { unsigned char _pad[0x10]; CDefinitionManager* DefinitionManager; };
static long g_itemCount = 0; static long g_itemTypeSeen = -1; static int g_countCalls = 0;
struct CTCInventoryBase {
    long GetNumberOfItemsOfTypeInInventory(long type) { ++g_countCalls; g_itemTypeSeen = type; return g_itemCount; }
};
namespace NPlayerGui {
struct SGuildSealMessage {
    CCharString Text;
    long Time;
    __forceinline SGuildSealMessage(const CCharString& text, long time) : Text(text), Time(time) { }
};
static int g_pushCalls = 0; static const void* g_pushText = 0; static long g_pushTime = -1;
struct CMessageDeque {
    void* StartCur; void* StartFirst; void* StartLast; void* StartNode;
    void* FinishCur; void* FinishFirst; void* FinishLast; void* FinishNode;
    bool empty() const { return FinishCur == StartCur; }
    void push_back(const SGuildSealMessage& message) { ++g_pushCalls; g_pushText = message.Text.PStringData; g_pushTime = message.Time; FinishCur = (char*)FinishCur + 8; }
};
struct CDrawGuildSeal {
    unsigned char _pad0[8];
    CPlayerManager* PlayerManager;
    CDefinitionOwner* Definitions;
    unsigned char _pad1[0x58];
    CMessageDeque Messages;
    void AddMessage(const CCharString& text, long time, bool force);
};
void CDrawGuildSeal::AddMessage(const CCharString& text, long time, bool force)
{
    CThingCreatureBase* hero = PlayerManager->GetMainPlayer()->GetControlledCreature();
    if (!IS_THING_ALIVE<CThing*>(hero) || hero->IsChild())
        return;
    if (!hero->HasInterface(0x11))
        return;
    CTCInventoryBase* inventory = hero->GetInterfaceUnchecked<CTCInventoryBase>(0x11);
    long seal;
    {
        CCharString name("OBJECT_GUILD_SEAL_1");
        CDefinitionManager* definitions = Definitions->DefinitionManager;
        seal = definitions->GetDefGlobalIndexFromName(name);
    }
    if (seal > 0 && inventory->GetNumberOfItemsOfTypeInInventory(seal) > 0 && (force || Messages.empty()))
        Messages.push_back(SGuildSealMessage(text, time));
}
}
int main()
{
    using namespace NPlayerGui;
    CThingCreatureBase hero; memset(&hero, 0, sizeof(hero)); CTCInventoryBase inv;
    CInterfaceMap::SEntry entries[2] = { { 0x10, (CTCBase*)0x99 }, { 0x11, (CTCBase*)&inv } };
    hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 2;
    CPlayerManager pm; pm.player.creature = &hero; CDefinitionManager dm; CDefinitionOwner owner; owner.DefinitionManager = &dm;
    CDrawGuildSeal g; memset(&g, 0, sizeof(g)); g.PlayerManager = &pm; g.Definitions = &owner;
    CCharString text("hello");
    // 1. dead hero / child hero: nothing
    hero.alive = false; g.AddMessage(text, 5, false); CHECK(g_nameCalls == 0);
    hero.alive = true; hero.child = true; g.AddMessage(text, 5, false); CHECK(g_nameCalls == 0); hero.child = false;
    // 2. no inventory component: nothing
    g.AddMessage(text, 5, false); CHECK(g_nameCalls == 0);
    hero.m_present.w[0x11 >> 5] = 1UL << (0x11 & 31);
    // 3. seal def unknown (index 0): looked up, not counted
    g_sealIndex = 0; g.AddMessage(text, 5, false); CHECK(g_nameCalls == 1); CHECK(strcmp(g_nameSeen, "OBJECT_GUILD_SEAL_1") == 0); CHECK(g_countCalls == 0);
    // 4. seal known but none carried: no push
    g_sealIndex = 77; g_itemCount = 0; g.AddMessage(text, 5, false); CHECK(g_countCalls == 1); CHECK(g_itemTypeSeen == 77); CHECK(g_pushCalls == 0);
    // 5. seal carried, queue empty: pushed with text and time
    g_itemCount = 1; g.AddMessage(text, 9, false); CHECK(g_pushCalls == 1); CHECK(g_pushText == text.PStringData); CHECK(g_pushTime == 9);
    // 6. queue not empty: only when forced
    g.AddMessage(text, 10, false); CHECK(g_pushCalls == 1);
    g.AddMessage(text, 11, true); CHECK(g_pushCalls == 2); CHECK(g_pushTime == 11);
    if (fails == 0) printf("0064e0da_TEST PASS\n"); else printf("0064e0da_TEST FAIL\n");
    return fails;
}