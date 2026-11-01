// Behaviour test for CTCInventoryQuests::ProcessButtonAReleased (0x0061B590): test-quest mode activates the selected test quest and
// closes the inventory; otherwise the inventory closes only outside the inventory screen, in selection state 1, when the
// quest-activate action succeeds; the base handler always runs last.
#include <cstdio>
static int g_activations, g_closes, g_baseCalls, g_questActions;
class CTCInventoryBase {
public:
    virtual ~CTCInventoryBase() { }
    virtual void AddCloseInventoryEvent() { ++g_closes; }
    virtual bool ProcessQuestActivateAction() { return false; }
    virtual void ProcessButtonAReleased() { ++g_baseCalls; }
    int InventorySelectionState;
};
class CBase {
public:
    virtual bool IsInventoryScreen() const { return true; }
};
class CTCInventoryQuests : public CTCInventoryBase, public CBase {
public:
    virtual bool IsInventoryScreen() const { return ScreenFlag; }
    virtual bool ProcessQuestActivateAction() { ++g_questActions; return ActionResult; }
    void ActivateSelectedTestQuest() { ++g_activations; }
    virtual void ProcessButtonAReleased()
    {
        if (ActiveTestQuestMode) {
            ActivateSelectedTestQuest();
            AddCloseInventoryEvent();
        } else if (!IsInventoryScreen() && InventorySelectionState == 1 && ProcessQuestActivateAction()) {
            AddCloseInventoryEvent();
        }
        CTCInventoryBase::ProcessButtonAReleased();
    }
    bool ScreenFlag, ActionResult, ActiveTestQuestMode;
};
static int run(bool testMode, bool screen, int state, bool action, int expectCloses, int expectActivations, int expectActions)
{
    g_activations = g_closes = g_baseCalls = g_questActions = 0;
    CTCInventoryQuests q;
    q.ActiveTestQuestMode = testMode; q.ScreenFlag = screen; q.InventorySelectionState = state; q.ActionResult = action;
    q.ProcessButtonAReleased();
    return g_closes == expectCloses && g_activations == expectActivations && g_questActions == expectActions && g_baseCalls == 1;
}
int main()
{
    int ok = run(true, false, 1, true, 1, 1, 0)
        && run(false, true, 1, true, 0, 0, 0)
        && run(false, false, 2, true, 0, 0, 0)
        && run(false, false, 1, false, 0, 0, 1)
        && run(false, false, 1, true, 1, 0, 1);
    if (!ok) { std::printf("0061b590_TEST FAIL\n"); return 1; }
    std::printf("0061b590_TEST PASS\n");
    return 0;
}