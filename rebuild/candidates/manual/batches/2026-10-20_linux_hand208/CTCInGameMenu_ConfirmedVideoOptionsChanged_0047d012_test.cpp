// Behaviour test for CTCInGameMenu::ConfirmedVideoOptionsChanged (0x0047D012): the OK button (if found), the
// dialog and the mouse pointer are released from the manager, the pointer re-added and set to observe event 0x19,
// the dialog deactivated and the revert timer set to -1.
#include <cstdio>
#include <cstring>

struct CCharString { const char* Text; CCharString(const char* text, long) : Text(text) {} };
namespace NUISystem {
struct CObserver {
    long Observed;
    CObserver() : Observed(0) {}
    virtual void ObserveEvent(long eventType) { Observed = eventType; }
};
struct CComponent;
struct CComponentBase {
    CComponent* Child; const char* ChildName; int Active;
    CComponentBase() : Child(0), ChildName(""), Active(1) {}
    virtual CComponent* FindChild(CCharString name) { return std::strcmp(name.Text, ChildName) == 0 ? Child : 0; }
    virtual void SetActive_slot48(bool active) { Active = active ? 1 : 0; }
};
struct CComponent : CComponentBase, CObserver {};
struct CYesNo : CComponent {};
static CObserver* g_released[8]; static int g_nReleased = 0; static CObserver* g_added = 0;
struct CManager {
    virtual void AddObserver(CObserver* observer) { g_added = observer; }
    virtual void ReleaseObserver_0055ca00(CObserver* observer) { g_released[g_nReleased++] = observer; }
    static CManager* GetInstance() { static CManager m; return &m; }
};
}
struct CTCInGameMenu {
    NUISystem::CYesNo* PVideoOptionsChangedDialog;
    double VideoChangesTimer;
    NUISystem::CComponent* PMousePointer;
    void ConfirmedVideoOptionsChanged();
};
void CTCInGameMenu::ConfirmedVideoOptionsChanged()
{
    NUISystem::CComponent* okButton = PVideoOptionsChangedDialog->FindChild(CCharString("PC_OK_GAME_OKBUTTON", -1));
    if (okButton)
        NUISystem::CManager::GetInstance()->ReleaseObserver_0055ca00(okButton);
    NUISystem::CManager::GetInstance()->ReleaseObserver_0055ca00(PVideoOptionsChangedDialog);
    NUISystem::CManager::GetInstance()->ReleaseObserver_0055ca00(PMousePointer);
    NUISystem::CManager::GetInstance()->AddObserver(PMousePointer);
    PMousePointer->ObserveEvent(0x19);
    PVideoOptionsChangedDialog->SetActive_slot48(false);
    VideoChangesTimer = -1.0;
}
int main()
{
    using namespace NUISystem;
    CYesNo dialog; CComponent ok; CComponent mouse;
    dialog.Child = &ok; dialog.ChildName = "PC_OK_GAME_OKBUTTON";
    CTCInGameMenu menu; menu.PVideoOptionsChangedDialog = &dialog; menu.PMousePointer = &mouse; menu.VideoChangesTimer = 5.0;
    menu.ConfirmedVideoOptionsChanged();
    if (g_nReleased != 3 || g_released[0] != static_cast<CObserver*>(&ok) || g_released[1] != static_cast<CObserver*>(&dialog) ||
        g_released[2] != static_cast<CObserver*>(&mouse) || g_added != static_cast<CObserver*>(&mouse) || mouse.Observed != 0x19 ||
        dialog.Active != 0 || menu.VideoChangesTimer != -1.0) { std::printf("0047d012_TEST FAIL\n"); return 1; }
    dialog.ChildName = "NOPE"; g_nReleased = 0;
    menu.ConfirmedVideoOptionsChanged();
    if (g_nReleased != 2 || g_released[0] != static_cast<CObserver*>(&dialog)) { std::printf("0047d012_TEST FAIL\n"); return 1; }
    std::printf("0047d012_TEST PASS\n");
    return 0;
}
