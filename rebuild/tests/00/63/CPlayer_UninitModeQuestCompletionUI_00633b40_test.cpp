#include <cstdio>
struct CGamePlayerInterface { int m_id; };
struct CInputProcess {
    CGamePlayerInterface* m_pAttachedTo; int m_adds; int m_removes;
    CInputProcess() : m_pAttachedTo(0), m_adds(0), m_removes(0) { }
    virtual ~CInputProcess() { }
    virtual bool AddToInterface(CGamePlayerInterface* i) { m_pAttachedTo = i; ++m_adds; return true; }
    void RemoveFromInterface() { m_pAttachedTo = 0; ++m_removes; }
};
struct CPlayer {
    CGamePlayerInterface* PlayerInterface;
    CInputProcess* InputProcessMain_ptr;
    CInputProcess* InputProcessDebugControls_ptr;
    CInputProcess* InputProcessQuickAccessItems_ptr;
    CInputProcess* InputProcessControlCreatureRightStick_ptr;
    CInputProcess* InputProcessRightStickLookAround_ptr;
    CInputProcess* InputProcessCycleSpecialCameraModes_ptr;
    CInputProcess* InputProcessQuestCompletionUI_ptr;
    bool Local; bool m_isMain; long m_lastAdded; bool m_lastFlag; long m_lastRemoved;
    bool IsMainPlayer() const { return m_isMain; }
    void AddMode(long mode, bool flag) { m_lastAdded = mode; m_lastFlag = flag; }
    void RemoveMode(long mode) { m_lastRemoved = mode; }
    void InitModeChargeQuickAccess(long, void*)
    {
        InputProcessQuickAccessItems_ptr->AddToInterface(PlayerInterface);
        InputProcessControlCreatureRightStick_ptr->AddToInterface(PlayerInterface);
        InputProcessRightStickLookAround_ptr->AddToInterface(PlayerInterface);
        InputProcessCycleSpecialCameraModes_ptr->AddToInterface(PlayerInterface);
        InputProcessDebugControls_ptr->AddToInterface(PlayerInterface);
    }
    void UninitModeChargeQuickAccess(long, void*)
    {
        InputProcessQuickAccessItems_ptr->RemoveFromInterface();
        InputProcessControlCreatureRightStick_ptr->RemoveFromInterface();
        InputProcessRightStickLookAround_ptr->RemoveFromInterface();
        InputProcessCycleSpecialCameraModes_ptr->RemoveFromInterface();
        InputProcessDebugControls_ptr->RemoveFromInterface();
        RemoveMode(0x1c);
    }
    void UninitModeQuestCompletionUI(long, void*)
    {
        InputProcessMain_ptr->AddToInterface(PlayerInterface);
        InputProcessQuestCompletionUI_ptr->RemoveFromInterface();
    }
    void InitModes()
    {
        if (Local) {
            if (IsMainPlayer()) InputProcessMain_ptr->AddToInterface(PlayerInterface);
            if (Local) AddMode(0, true);
        }
    }
};
static CInputProcess g_procs[8];
static void Setup(CPlayer& p, CGamePlayerInterface* gi)
{
    p.PlayerInterface = gi;
    p.InputProcessMain_ptr = &g_procs[0]; p.InputProcessDebugControls_ptr = &g_procs[1]; p.InputProcessQuickAccessItems_ptr = &g_procs[2];
    p.InputProcessControlCreatureRightStick_ptr = &g_procs[3]; p.InputProcessRightStickLookAround_ptr = &g_procs[4];
    p.InputProcessCycleSpecialCameraModes_ptr = &g_procs[5]; p.InputProcessQuestCompletionUI_ptr = &g_procs[6];
    p.Local = true; p.m_isMain = true; p.m_lastAdded = -1; p.m_lastFlag = false; p.m_lastRemoved = -1;
}
int main()
{
    CGamePlayerInterface gi; gi.m_id = 1;
    CPlayer p; Setup(p, &gi);
    g_procs[6].AddToInterface(&gi);
    p.UninitModeQuestCompletionUI(0, 0);
    if (g_procs[0].m_pAttachedTo != &gi || g_procs[6].m_pAttachedTo != 0 || g_procs[6].m_removes != 1) { std::printf("00633b40_TEST FAIL\n"); return 1; }
    std::printf("00633b40_TEST PASS\n");
    return 0;
}