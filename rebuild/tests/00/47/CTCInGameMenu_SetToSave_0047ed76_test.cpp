// Behaviour test for CTCInGameMenu::SetToSave (0x0047ED76): the row body verbatim over models of its callees. Kills: the quest-screen
// test removed / inverted, the null-child test removed, the wrong component (not the last pushed) used, X / Y swapped, the wrong
// half-extent or scale axis, the time not stamped, the save index / wait count not stored.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
static void check(bool v, int line) { if (!v) { printf("FAIL line %d\n", line); exit(1); } }
#define CHECK(c) check((c), __LINE__)
static bool same(float a, float b) { return memcmp(&a, &b, 4) == 0; }
struct C2DVector { float X, Y; C2DVector() {} C2DVector(float x, float y) : X(x), Y(y) {} };
struct CCharString { const char* s; CCharString(const char* t, long) : s(t) {} };
static bool between, child_found; static float bb_w, bb_h, scale_x, scale_y; static double now;
static int set_value_calls, init_calls, change_calls, find_calls; static C2DVector changed_pos; static float changed_time; static bool changed_imm;
namespace NUISystem {
struct CComponent {
    int id; CComponent* child;
    virtual CComponent* FindChild(CCharString name) { ++find_calls; CHECK(strcmp(name.s, "PC_SAVE_IN_PROGRESS_TEXT") == 0); return child_found ? child : 0; }
    virtual void Initialise() { ++init_calls; last_init = this; }
    virtual void ChangePosition(const C2DVector& p, float t, bool imm) { ++change_calls; changed_pos = p; changed_time = t; changed_imm = imm; last_change = this; }
    static CComponent* last_init; static CComponent* last_change;
};
CComponent* CComponent::last_init; CComponent* CComponent::last_change;
struct CText : CComponent {
    const char* value; bool a, b;
    void SetValue(const char* v, bool x, bool y) { ++set_value_calls; value = v; a = x; b = y; }
};
static CComponent created; static CText created_text;
struct CManager {
    static CManager* GetInstance() { static CManager m; return &m; }
    CComponent* CreateComponent(const char* name, bool f) { CHECK(strcmp(name, "PC_SAVE_IN_PROGRESS_DIALOG") == 0 && !f); return &created; }
    C2DVector GetUIScale() const { return C2DVector(scale_x, scale_y); }
};
struct CUIScreenUnitConverter {
    static float GetBackBufferWidth() { return bb_w; }
    static float GetBackBufferHeight() { return bb_h; }
};
}
struct CQuestManager { static CQuestManager& Get() { static CQuestManager q; return q; } bool IsBetweenQuestStartAndFinishScreens() { return between; } };
static double GFGetTime() { return now; }
using namespace NUISystem;
class CTCInGameMenu {
public:
    long SaveGameIndex, FramesToWaitToSave; std::vector<CComponent*> Components; CComponent* PSaveInProgress; double SaveInProgressTime;
    void SetToSave(long save_index);
};
// ---- row body (verbatim) ----
void CTCInGameMenu::SetToSave(long save_index)
{
    SaveGameIndex = save_index;
    FramesToWaitToSave = 2;
    Components.push_back(CManager::GetInstance()->CreateComponent("PC_SAVE_IN_PROGRESS_DIALOG", false));
    if (CQuestManager::Get().IsBetweenQuestStartAndFinishScreens()) {
        CComponent* ptext = Components[Components.size() - 1]->FindChild(CCharString("PC_SAVE_IN_PROGRESS_TEXT", -1));
        if (ptext) {
            CText* ptext_real = static_cast<CText*>(ptext);
            ptext_real->SetValue("TEXT_GUI_SAVE_HERO_IN_PROGRESS_PC", true, true);
        }
    }
    Components[Components.size() - 1]->Initialise();
    PSaveInProgress = Components[Components.size() - 1];
    C2DVector pos((CUIScreenUnitConverter::GetBackBufferWidth() * 0.5f) - CManager::GetInstance()->GetUIScale().X * 204.0f,
                  (CUIScreenUnitConverter::GetBackBufferHeight() * 0.5f) - CManager::GetInstance()->GetUIScale().Y * 52.0f);
    PSaveInProgress->ChangePosition(pos, 0.0f, false);
    SaveInProgressTime = GFGetTime();
}
// ---- end row body ----
int main()
{
    CComponent other; other.child = 0; created.child = &created_text;
    const float ws[] = { 640.0f, 1024.0f, 1280.0f, 801.0f }, hs[] = { 480.0f, 768.0f, 1024.0f, 599.0f }, ss[] = { 1.0f, 0.5f, 2.0f, 1.25f };
    long cases = 0;
    for (int q = 0; q != 2; ++q) for (int c = 0; c != 2; ++c) for (int i = 0; i != 4; ++i) for (int j = 0; j != 4; ++j) {
        between = q != 0; child_found = c != 0; bb_w = ws[i]; bb_h = hs[(i + j) & 3]; scale_x = ss[j]; scale_y = ss[(j + 1) & 3];
        now = 12.5 + i * 3 + j;
        set_value_calls = init_calls = change_calls = find_calls = 0; created_text.value = 0;
        CTCInGameMenu m; m.SaveGameIndex = -1; m.FramesToWaitToSave = 0; m.PSaveInProgress = 0; m.SaveInProgressTime = 0;
        m.Components.push_back(&other);
        m.SetToSave(7 + i);
        CHECK(m.SaveGameIndex == 7 + i && m.FramesToWaitToSave == 2);
        CHECK(m.Components.size() == 2 && m.Components[1] == &created && m.Components[0] == &other);
        CHECK(find_calls == (q ? 1 : 0));
        CHECK(set_value_calls == (q && c ? 1 : 0));
        if (q && c) CHECK(strcmp(created_text.value, "TEXT_GUI_SAVE_HERO_IN_PROGRESS_PC") == 0 && created_text.a && created_text.b);
        CHECK(init_calls == 1 && CComponent::last_init == &created);
        CHECK(m.PSaveInProgress == &created && change_calls == 1 && CComponent::last_change == &created);
        float ex = bb_w * 0.5f - scale_x * 204.0f, ey = bb_h * 0.5f - scale_y * 52.0f;
        CHECK(same(changed_pos.X, ex) && same(changed_pos.Y, ey) && same(changed_time, 0.0f) && !changed_imm);
        CHECK(m.SaveInProgressTime == now);
        ++cases;
    }
    CHECK(cases == 64);
    puts("0047ed76_TEST PASS");
    return 0;
}