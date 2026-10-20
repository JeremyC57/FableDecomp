// Behaviour test for CTCHeroOnlineScoreboard::OnOkClicked (0x0055EE20): without an inventory nothing happens; with
// one the mouse pointer and the OK button are released, the screen deactivated, the pointer re-added and set to
// observe event 0x19.
#include <cstdio>
#include <cstring>
// --- test-side copy of the CThing interface lookup (std::bitset presence + sorted {type, component} map) ---
struct CTCBase;
struct CPresenceBits {
    long bits[16];
    bool test(long bit) { return bit >= 0 && bit < 512 && ((bits[bit >> 5] >> (bit & 31)) & 1) != 0; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    CPresenceBits m_present;
    CInterfaceMap m_interfaces;
    CInterfaceMap::SEntry entries[4];
    void Init() { for (int i = 0; i < 16; ++i) m_present.bits[i] = 0; m_interfaces.first = m_interfaces.last = entries; }
    void Add(long type, void* comp) { m_present.bits[type >> 5] |= 1L << (type & 31); m_interfaces.last->type = type; m_interfaces.last->component = (CTCBase*)comp; ++m_interfaces.last; }
    template<class T> bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
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
    bool HasTC(long wanted) { return m_present.test(wanted); }
    template<class T> T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
// --- end lookup copy ---
struct CCharString { const char* Text; CCharString(const char* text, long) : Text(text) {} };
namespace NUISystem {
struct CObserver { long Observed; CObserver() : Observed(0) {} virtual void ObserveEvent(long e) { Observed = e; } };
struct CComponent;
struct CComponentBase {
    CComponent* Child; const char* ChildName; int Active;
    CComponentBase() : Child(0), ChildName(""), Active(1) {}
    virtual CComponent* FindChild(CCharString name) { return std::strcmp(name.Text, ChildName) == 0 ? Child : 0; }
    virtual void SetActive_slot48(bool active) { Active = active ? 1 : 0; }
};
struct CComponent : CComponentBase, CObserver {};
static CObserver* g_released[8]; static int g_nReleased = 0; static CObserver* g_added = 0;
struct CManager {
    virtual void AddObserver(CObserver* o) { g_added = o; }
    virtual void ReleaseObserver_0055ca00(CObserver* o) { g_released[g_nReleased++] = o; }
    static CManager* GetInstance() { static CManager m; return &m; }
};
}
struct CTCInventory { NUISystem::CComponent* PMouse_Pointer; };
struct CTCHeroOnlineScoreboard {
    CThing* ParentThing; NUISystem::CComponent* PScoreboardScreen;
    void OnOkClicked();
};
void CTCHeroOnlineScoreboard::OnOkClicked()
{
    CThing* thing = ParentThing;
    if (thing->HasTC(0x11)) {
        CTCInventory* inventory = thing->GetTC<CTCInventory>(0x11);
        NUISystem::CComponent* okButton = PScoreboardScreen->FindChild(CCharString("PC_OK_GAME_OKBUTTON_SCOREBOARD", -1));
        NUISystem::CManager::GetInstance()->ReleaseObserver_0055ca00(inventory->PMouse_Pointer);
        NUISystem::CManager::GetInstance()->ReleaseObserver_0055ca00(okButton);
        PScoreboardScreen->SetActive_slot48(false);
        NUISystem::CManager::GetInstance()->AddObserver(inventory->PMouse_Pointer);
        inventory->PMouse_Pointer->ObserveEvent(0x19);
    }
}
int main()
{
    using namespace NUISystem;
    CThing thing; thing.Init();
    CComponent screen, ok, mouse; screen.Child = &ok; screen.ChildName = "PC_OK_GAME_OKBUTTON_SCOREBOARD";
    CTCHeroOnlineScoreboard sb; sb.ParentThing = &thing; sb.PScoreboardScreen = &screen;
    sb.OnOkClicked();
    if (g_nReleased != 0 || screen.Active != 1) { std::printf("0055ee20_TEST FAIL\n"); return 1; }
    CTCInventory inv; inv.PMouse_Pointer = &mouse; thing.Add(0x11, &inv);
    sb.OnOkClicked();
    if (g_nReleased != 2 || g_released[0] != static_cast<CObserver*>(&mouse) || g_released[1] != static_cast<CObserver*>(&ok) ||
        screen.Active != 0 || g_added != static_cast<CObserver*>(&mouse) || mouse.Observed != 0x19) { std::printf("0055ee20_TEST FAIL\n"); return 1; }
    std::printf("0055ee20_TEST PASS\n");
    return 0;
}
