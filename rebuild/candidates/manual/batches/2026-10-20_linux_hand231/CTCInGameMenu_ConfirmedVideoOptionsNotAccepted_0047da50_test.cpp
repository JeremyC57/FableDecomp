// Behaviour test for CTCInGameMenu::ConfirmedVideoOptionsNotAccepted (retail 0x0047DA50): model copy + stub callees.
#include <cstdio>
#include <cstring>

static char g_lastFind[64];
struct CCharString {
    const char* text;
    CCharString(const char* t, long) : text(t) {}
    CCharString(const CCharString& o) : text(o.text) {}
    ~CCharString() {}
};
namespace NUISystem {
struct CObserver { int events[4]; int eventCount; void ObserveEvent(long e) { events[eventCount++] = (int)e; } };
struct CComponent;
struct CComponentBase { CComponent* child; bool active; CComponent* FindChild(CCharString name) { std::strcpy(g_lastFind, name.text); return child; } void SetActive_slot48(bool a) { active = a; } };
struct CComponent : CComponentBase, CObserver { };
struct CYesNo : CComponent { };
struct CManager {
    CObserver* released[8]; int releasedCount; CObserver* added;
    void AddObserver(CObserver* o) { added = o; }
    void ReleaseObserver_0055ca00(CObserver* o) { released[releasedCount++] = o; }
    static CManager* GetInstance() { static CManager m; return &m; }
};
}
class CTCBase;
struct CPresenceBits { unsigned long bits; bool test(long kind) { return (bits >> kind) & 1; } };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing {
public:
    CPresenceBits present;
    CInterfaceMap m_interfaces;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCInventory { int reverts; void RevertVideoChanges() { ++reverts; } };
static int g_order[8]; static int g_orderCount = 0;
struct CUserProfileManager {
    bool DisplayDialogOnOptionsChanged;
    void Apply() { g_order[g_orderCount++] = DisplayDialogOnOptionsChanged ? 11 : 10; }
    void Save() { g_order[g_orderCount++] = DisplayDialogOnOptionsChanged ? 21 : 20; }
    static CUserProfileManager& Get() { static CUserProfileManager m = { true }; return m; }
};
struct CTCInGameMenu {
    CThing* ParentThing;
    NUISystem::CYesNo* PVideoOptionsNotAcceptedDialog;
    NUISystem::CComponent* PMousePointer;
    void ConfirmedVideoOptionsNotAccepted();
};
void CTCInGameMenu::ConfirmedVideoOptionsNotAccepted()
{
    NUISystem::CComponent* okButton = PVideoOptionsNotAcceptedDialog->FindChild(CCharString("PC_OK_GAME_OKBUTTON", -1));
    if (okButton)
        NUISystem::CManager::GetInstance()->ReleaseObserver_0055ca00(okButton);
    NUISystem::CManager::GetInstance()->ReleaseObserver_0055ca00(PVideoOptionsNotAcceptedDialog);
    NUISystem::CManager::GetInstance()->ReleaseObserver_0055ca00(PMousePointer);
    NUISystem::CManager::GetInstance()->AddObserver(PMousePointer);
    PMousePointer->ObserveEvent(0x19);
    PVideoOptionsNotAcceptedDialog->SetActive_slot48(false);
    CThing* parent = ParentThing;
    CTCInventory* inventory;
    if (parent->GetInterface(&inventory, 0x11) && inventory)
        inventory->RevertVideoChanges();
    CUserProfileManager::Get().DisplayDialogOnOptionsChanged = false;
    CUserProfileManager::Get().Apply();
    CUserProfileManager::Get().Save();
    CUserProfileManager::Get().DisplayDialogOnOptionsChanged = true;
}
int main()
{
    int fail = 0;
    NUISystem::CComponent ok; std::memset(&ok, 0, sizeof(ok));
    NUISystem::CYesNo dialog; std::memset(&dialog, 0, sizeof(dialog)); dialog.child = &ok; dialog.active = true;
    NUISystem::CComponent pointer; std::memset(&pointer, 0, sizeof(pointer));
    CTCInventory inv = { 0 };
    CInterfaceMap::SEntry entries[1] = { { 0x11, (CTCBase*)&inv } };
    CThing hero; hero.present.bits = 1UL << 0x11; hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 1;
    CTCInGameMenu menu = { &hero, &dialog, &pointer };
    menu.ConfirmedVideoOptionsNotAccepted();
    NUISystem::CManager* m = NUISystem::CManager::GetInstance();
    if (std::strcmp(g_lastFind, "PC_OK_GAME_OKBUTTON") || m->releasedCount != 3) fail = 1;
    if (m->released[0] != (NUISystem::CObserver*)&ok || m->released[1] != (NUISystem::CObserver*)&dialog || m->released[2] != (NUISystem::CObserver*)&pointer) fail = 2;
    if (m->added != (NUISystem::CObserver*)&pointer || pointer.eventCount != 1 || pointer.events[0] != 0x19 || dialog.active) fail = 3;
    if (inv.reverts != 1 || g_orderCount != 2 || g_order[0] != 10 || g_order[1] != 20 || !CUserProfileManager::Get().DisplayDialogOnOptionsChanged) fail = 4;
    // no OK button, no inventory: still releases the dialog and pointer, re-saves the profile
    dialog.child = 0; hero.present.bits = 0; m->releasedCount = 0;
    menu.ConfirmedVideoOptionsNotAccepted();
    if (m->releasedCount != 2 || inv.reverts != 1 || g_orderCount != 4) fail = 5;
    if (fail) { std::printf("0047da50_TEST FAIL\n"); return 1; }
    std::printf("0047da50_TEST PASS\n");
    return 0;
}
