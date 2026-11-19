// Behaviour test for CTCInGameMenu::PCSetQuitToFrontEnd (0x0047D296): the row body verbatim over models of its callees (a component is a
// CUIInterface + CObserver, so the observer pointer is the component + 4 and a null stays null). Kills: callbacks swapped or missing,
// the wrong dialog state, X / Y swapped or the wrong half-extent / scale axis, the globals not set (or set from the wrong member),
// any observer call dropped, reordered or sent the wrong object, a missing null test on either button, the wrong button names.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
static void check(bool v, int line) { if (!v) { printf("FAIL line %d\n", line); exit(1); } }
#define CHECK(c) check((c), __LINE__)
static bool same(float a, float b) { return memcmp(&a, &b, 4) == 0; }
static std::string trace;
static void T(const char* s) { trace += s; trace += ';'; }
struct C2DVector { float X, Y; C2DVector() {} C2DVector(float x, float y) : X(x), Y(y) {} };
struct CCharString { const char* s; CCharString(const char* t) : s(t) {} };
static float bb_w, bb_h, scale_x, scale_y; static bool has_yes, has_no;
static C2DVector changed_pos; static float changed_time; static bool changed_imm; static unsigned long state;
static void (*yes_cb)(); static void (*no_cb)();
namespace NUISystem {
enum EEvent { UI_EVENT_MOUSE_MOVEMENT = 25 };
struct CUIInterface { int pad; virtual ~CUIInterface() {} };
struct CObserver {
    const char* tag; EEvent observed; bool all;
    virtual ~CObserver() {}
    void ObserveEvent(EEvent e) { observed = e; std::string s = std::string("obs:") + tag; T(s.c_str()); }
};
struct CComponent : CUIInterface, CObserver {
    CComponent* yes; CComponent* no;
    CComponent* FindChild(CCharString n) {
        if (strcmp(n.s, "PC_YESNO_GAME_YESBUTTON") == 0) { T("find:yes"); return has_yes ? yes : 0; }
        if (strcmp(n.s, "PC_YESNO_GAME_NOBUTTON") == 0) { T("find:no"); return has_no ? no : 0; }
        CHECK(false); return 0;
    }
    void ChangeState(unsigned long s) { state = s; T("state"); }
    void ChangePosition(const C2DVector& p, float t, bool imm) { changed_pos = p; changed_time = t; changed_imm = imm; T("pos"); }
    void ObserveAllEvents() { all = true; std::string s = std::string("all:") + tag; T(s.c_str()); }
};
struct CYesNo : CComponent {
    void SetYesCallBack(void (*f)()) { yes_cb = f; T("yescb"); }
    void SetNoCallBack(void (*f)()) { no_cb = f; T("nocb"); }
};
struct CManager {
    static CManager* GetInstance() { static CManager m; return &m; }
    C2DVector GetUIScale() const { return C2DVector(scale_x, scale_y); }
    void RemoveObserver(CObserver* o) { std::string s = std::string("rem:") + (o ? o->tag : "null"); T(s.c_str()); }
    void AddConcurrentExclusiveObserver(CObserver* o) { std::string s = std::string("add:") + (o ? o->tag : "null"); T(s.c_str()); }
};
struct CUIScreenUnitConverter {
    static float GetBackBufferWidth() { return bb_w; }
    static float GetBackBufferHeight() { return bb_h; }
};
}
using namespace NUISystem;
static void PCQuitToFrontEnd() {}
static void PCCancelQuitToFrontEnd() {}
static CYesNo* g_pPCYesNoDialog; static CComponent* g_pPCMousePointer;
class CTCInGameMenu {
public:
    long SaveGameToLoad; CComponent* PMousePointer; CYesNo* PYesNo;
    void PCSetQuitToFrontEnd();
};
// ---- row body (verbatim) ----
void CTCInGameMenu::PCSetQuitToFrontEnd()
{
    PYesNo->SetYesCallBack(PCQuitToFrontEnd);
    PYesNo->ChangeState(1);
    C2DVector pos((CUIScreenUnitConverter::GetBackBufferWidth() * 0.5f) - CManager::GetInstance()->GetUIScale().X * 104.0f,
                  (CUIScreenUnitConverter::GetBackBufferHeight() * 0.5f) - CManager::GetInstance()->GetUIScale().Y * 52.0f);
    PYesNo->ChangePosition(pos, 0.0f, false);
    PYesNo->SetNoCallBack(PCCancelQuitToFrontEnd);
    g_pPCYesNoDialog = PYesNo;
    g_pPCMousePointer = PMousePointer;
    CManager::GetInstance()->RemoveObserver(g_pPCYesNoDialog);
    CManager::GetInstance()->AddConcurrentExclusiveObserver(g_pPCYesNoDialog);
    CManager::GetInstance()->RemoveObserver(g_pPCMousePointer);
    CManager::GetInstance()->AddConcurrentExclusiveObserver(g_pPCMousePointer);
    g_pPCMousePointer->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    g_pPCYesNoDialog->ObserveAllEvents();
    CComponent* pclickable = PYesNo->FindChild("PC_YESNO_GAME_YESBUTTON");
    if (pclickable) {
        CManager::GetInstance()->AddConcurrentExclusiveObserver(pclickable);
        pclickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    pclickable = PYesNo->FindChild("PC_YESNO_GAME_NOBUTTON");
    if (pclickable) {
        CManager::GetInstance()->AddConcurrentExclusiveObserver(pclickable);
        pclickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
}

// ---- end row body ----
int main()
{
    CYesNo dlg; CComponent mouse, yesb, nob, other;
    dlg.tag = "dlg"; mouse.tag = "mouse"; yesb.tag = "yes"; nob.tag = "no"; other.tag = "other";
    dlg.yes = &yesb; dlg.no = &nob;
    const float ws[] = { 640.0f, 1024.0f, 1280.0f, 801.0f }, hs[] = { 480.0f, 768.0f, 1024.0f, 599.0f }, ss[] = { 1.0f, 0.5f, 2.0f, 1.25f };
    long cases = 0;
    for (int y = 0; y != 2; ++y) for (int n = 0; n != 2; ++n) for (int i = 0; i != 4; ++i) for (int j = 0; j != 4; ++j) {
        has_yes = y != 0; has_no = n != 0; bb_w = ws[i]; bb_h = hs[(i + j) & 3]; scale_x = ss[j]; scale_y = ss[(j + 1) & 3];
        trace = ""; yes_cb = no_cb = 0; state = 0; g_pPCYesNoDialog = 0; g_pPCMousePointer = &other;
        dlg.all = false; mouse.observed = yesb.observed = nob.observed = (EEvent)0;
        CTCInGameMenu m; m.SaveGameToLoad = -1; m.PMousePointer = &mouse; m.PYesNo = &dlg;
        m.PCSetQuitToFrontEnd();
        CHECK(m.SaveGameToLoad == -1);
        CHECK(yes_cb == PCQuitToFrontEnd && no_cb == PCCancelQuitToFrontEnd && state == 1);
        CHECK(g_pPCYesNoDialog == &dlg && g_pPCMousePointer == &mouse);
        float ex = bb_w * 0.5f - scale_x * 104.0f, ey = bb_h * 0.5f - scale_y * 52.0f;
        CHECK(same(changed_pos.X, ex) && same(changed_pos.Y, ey) && same(changed_time, 0.0f) && !changed_imm);
        std::string want = "yescb;state;pos;nocb;rem:dlg;add:dlg;rem:mouse;add:mouse;obs:mouse;all:dlg;find:yes;";
        if (y) want += "add:yes;obs:yes;";
        want += "find:no;";
        if (n) want += "add:no;obs:no;";
        CHECK(trace == want);
        CHECK(mouse.observed == UI_EVENT_MOUSE_MOVEMENT && dlg.all);
        CHECK(yesb.observed == (y ? UI_EVENT_MOUSE_MOVEMENT : (EEvent)0) && nob.observed == (n ? UI_EVENT_MOUSE_MOVEMENT : (EEvent)0));
        ++cases;
    }
    CHECK(cases == 64);
    puts("0047d296_TEST PASS");
    return 0;
}