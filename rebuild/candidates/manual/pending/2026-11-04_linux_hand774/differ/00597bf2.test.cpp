// Behaviour test for 0x00597BF2: NUISystem::CFrontEndManager::GotoPreviousScreen -- pop back one screen with the root/27/23 special cases
#include <cstdio>
#include <map>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct CComponent { int State; CComponent() : State(0) { } };
static int g_sounds = 0, g_loads = 0, g_keyboards = 0;
struct CFrontEndManager {
    std::vector<CComponent*> Stack; std::map<int, CComponent*> Screens;
    CComponent* PScreenToDeactivate; CComponent* PScreenToActivate; int NextScreen; bool NextFlag; CComponent* Specific;
    CFrontEndManager() : PScreenToDeactivate(0), PScreenToActivate(0), NextScreen(-1), NextFlag(false), Specific(0) { }
    bool IsScreenOnStack(CComponent* s) const { for (unsigned i = 0; i < Stack.size(); ++i) if (Stack[i] == s) return true; return false; }
    void GotoNextScreen(CComponent* s, bool f) { for (std::map<int, CComponent*>::iterator i = Screens.begin(); i != Screens.end(); ++i) if (i->second == s) NextScreen = i->first; NextFlag = f; }
    void GotoSpecificPreviousScreen(CComponent* s) { Specific = s; }
    void GotoPreviousScreen(bool force) {
        if (!(Screens[20] != Stack.back())) {
            if (!force) return;
            GotoNextScreen(Screens[26], false);
        } else {
            ++g_loads;
            if (Stack.size() > 1) {
                if (Stack.back() == Screens[27]) {
                    if (IsScreenOnStack(Screens[7])) GotoSpecificPreviousScreen(Screens[7]);
                    else GotoNextScreen(Screens[20], true);
                } else {
                    CComponent* leaving = Stack.back(); leaving->State = 6; Stack.pop_back();
                    PScreenToActivate = Stack.back(); PScreenToDeactivate = leaving;
                    if (Stack.back() == Screens[23]) ++g_keyboards;
                }
            } else if (force)
                GotoNextScreen(Screens[26], false);
        }
        ++g_sounds;
    }
};
int main()
{
    CComponent s[30]; CFrontEndManager m; for (int i = 0; i < 30; ++i) m.Screens[i] = &s[i];
    m.Stack.push_back(&s[20]); m.GotoPreviousScreen(false); CHECK(g_sounds == 0 && g_loads == 0);
    m.GotoPreviousScreen(true); CHECK(g_sounds == 1 && m.NextScreen == 26 && !m.NextFlag && g_loads == 0);
    m.Stack.clear(); m.Stack.push_back(&s[23]); m.Stack.push_back(&s[5]); m.GotoPreviousScreen(false);
    CHECK(m.PScreenToActivate == &s[23] && m.PScreenToDeactivate == &s[5] && s[5].State == 6 && g_keyboards == 1 && m.Stack.size() == 1);
    m.Stack.clear(); m.Stack.push_back(&s[7]); m.Stack.push_back(&s[27]); m.GotoPreviousScreen(false); CHECK(m.Specific == &s[7]);
    m.Stack.clear(); m.Stack.push_back(&s[1]); m.Stack.push_back(&s[27]); m.GotoPreviousScreen(false); CHECK(m.NextScreen == 20 && m.NextFlag);
    m.Stack.clear(); m.Stack.push_back(&s[3]); m.NextScreen = -1; m.GotoPreviousScreen(false); CHECK(m.NextScreen == -1);
    m.GotoPreviousScreen(true); CHECK(m.NextScreen == 26 && g_sounds == 6);
    if (g_fail) { std::printf("00597bf2_TEST FAIL\n"); return 1; }
    std::printf("00597bf2_TEST PASS\n");
    return 0;
}
