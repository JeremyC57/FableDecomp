// 00599e3f behaviour test: CFrontEndManager::Update (final.cpp body over mock front-end objects)
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <string.h>
#include <ctype.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static int icmp(const std::string& a, const std::string& b)
{
    if (a.size() != b.size()) return 1;
    for (size_t i = 0; i < a.size(); ++i) if (tolower(a[i]) != tolower(b[i])) return 1;
    return 0;
}
static double g_time; static bool g_installerActive = true; static int g_unpauses;
struct CComponentBase {
    bool Finished, Active; int Updates; int State; int Releases;
    bool IsFinished() { return Finished; }
    void Update(float) { ++Updates; }
};
struct CFrontEndGame { int Resets; bool Dirty; std::string Loaded; void Reset() { ++Resets; } void LoadGame(std::string* n) { Loaded = *n; } };
struct CVirtualKeyboard { bool Cancelled, Finished; std::string Name; std::string GetProfileName() { return Name; } };
static std::string g_text; static int g_removed;
struct CFrontEndManager {
    CFrontEndGame* Game; std::vector<CComponentBase*> ScreenStack; std::map<long, CComponentBase*> Screens;
    CVirtualKeyboard* VirtualKeyboard; bool CanLoad; CComponentBase* FadeOwner; CComponentBase* Fade; bool KeyboardActive;
    CComponentBase* MessageBox; CComponentBase* LoadBox; std::string LoadName; double InstallerPauseTime;
    bool InstallerPaused, InstallerResumed; std::string ProfileName; std::vector<std::string> Profiles;
    int Launches; std::vector<std::string> Log;
    void LaunchVirtualKeyboard() { ++Launches; }
    void EnableInput(bool e) { Log.push_back(e ? "in1" : "in0"); }
    void GetProfileNames(std::vector<std::string>* names) { *names = Profiles; }
    void LoadProfile(std::string* n) { Log.push_back("load:" + *n); }
    bool CreateNewProfile(std::string* n) { Log.push_back("create:" + *n); return *n != "bad"; }
    void GotoNextScreen(CComponentBase* s, bool back) { Log.push_back(s == Screens[0x13] ? "goto13" : s == Screens[0] ? "goto0" : "goto1b"); (void)back; }
    void Update(float dt);
};
void CFrontEndManager::Update(float dt)
{
    double now = g_time;
    if (now - InstallerPauseTime >= 2.0 && InstallerPaused && g_installerActive) {
        ++g_unpauses;
        InstallerPaused = false;
    }
    if (KeyboardActive && !ScreenStack.empty() && ScreenStack.back()) {
        if (ScreenStack.back()->IsFinished() || !ScreenStack.back()->Active) {
            KeyboardActive = false;
            LaunchVirtualKeyboard();
            if (ScreenStack.back() == Screens[0x13])
                ScreenStack.pop_back();
        }
    }
    if (VirtualKeyboard && VirtualKeyboard->Finished) {
        if (!VirtualKeyboard->Cancelled) {
            std::string current(ProfileName);
            std::string name = VirtualKeyboard->GetProfileName();
            std::vector<std::string> names;
            GetProfileNames(&names);
            size_t it = 0;
            for (; it != names.size(); ++it)
                if (icmp(names[it], name) == 0)
                    break;
            if (it != names.size()) {
                EnableInput(false);
                LoadProfile(&current);
                GotoNextScreen(Screens[0x13], false);
                g_text = "profile";
            }
            else {
                bool created = CreateNewProfile(&name);
                EnableInput(false);
                if (created)
                    GotoNextScreen(Screens[0], true);
                else {
                    LoadProfile(&current);
                    GotoNextScreen(Screens[0x1b], false);
                }
            }
        }
        else {
            g_text = "newprofile";
            EnableInput(true);
        }
    }
    for (std::map<long, CComponentBase*>::iterator node = Screens.begin(); node != Screens.end(); ++node)
        if (node->second)
            node->second->Update(dt);
    if (FadeOwner && (FadeOwner->IsFinished() || !FadeOwner->Active)) {
        Fade->State = 5; ++g_removed; ++Fade->Releases;
        FadeOwner = 0;
        Fade = 0;
    }
    if (MessageBox && (MessageBox->IsFinished() || !MessageBox->Active) && CanLoad) {
        MessageBox = 0;
        if (InstallerPaused && g_installerActive) {
            ++g_unpauses;
            InstallerPaused = false;
            InstallerResumed = true;
        }
        CFrontEndGame* game = Game;
        game->Reset();
        game->Dirty = true;
    }
    if (LoadBox && (LoadBox->IsFinished() || !LoadBox->Active)) {
        LoadBox = 0;
        if (InstallerPaused && g_installerActive) {
            ++g_unpauses;
            InstallerPaused = false;
            InstallerResumed = true;
        }
        Game->LoadGame(&LoadName);
    }
}
int main()
{
    CComponentBase s0 = { false, true, 0, 0, 0 }, s13 = { true, true, 0, 0, 0 }, s1b = { false, true, 0, 0, 0 };
    CComponentBase fadeOwner = { true, true, 0, 0, 0 }, fade = { false, true, 0, 0, 0 }, box = { false, false, 0, 0, 0 };
    CFrontEndGame game = { 0, false, "" };
    CVirtualKeyboard kb = { false, true, "ALICE" };
    CFrontEndManager m;
    m.Game = &game; m.Screens[0] = &s0; m.Screens[0x13] = &s13; m.Screens[0x1b] = &s1b; m.ScreenStack.push_back(&s13);
    m.VirtualKeyboard = &kb; m.CanLoad = true; m.FadeOwner = &fadeOwner; m.Fade = &fade; m.KeyboardActive = true;
    m.MessageBox = &box; m.LoadBox = 0; m.InstallerPauseTime = 0.0; m.InstallerPaused = true; m.InstallerResumed = false;
    m.ProfileName = "cur"; m.Profiles.push_back("bob"); m.Profiles.push_back("alice"); m.Launches = 0;
    g_time = 3.0;
    m.Update(0.5f);
    CHECK(g_unpauses == 1 && !m.InstallerPaused && !m.InstallerResumed);
    CHECK(m.Launches == 1 && m.ScreenStack.empty() && !m.KeyboardActive);
    CHECK(m.Log.size() == 3 && m.Log[0] == "in0" && m.Log[1] == "load:cur" && m.Log[2] == "goto13" && g_text == "profile");
    CHECK(s0.Updates == 1 && s13.Updates == 1 && s1b.Updates == 1);
    CHECK(m.FadeOwner == 0 && m.Fade == 0 && fade.State == 5 && fade.Releases == 1 && g_removed == 1);
    CHECK(m.MessageBox == 0 && game.Resets == 1 && game.Dirty);
    // new profile that fails, then cancelled keyboard, then a load box
    m.Log.clear(); kb.Name = "bad";
    m.Update(0.5f);
    CHECK(m.Log.size() == 4 && m.Log[0] == "create:bad" && m.Log[1] == "in0" && m.Log[2] == "load:cur" && m.Log[3] == "goto1b");
    m.Log.clear(); kb.Name = "carol";
    m.Update(0.5f);
    CHECK(m.Log.size() == 3 && m.Log[2] == "goto0");
    m.Log.clear(); kb.Cancelled = true; m.LoadBox = &box; m.LoadName = "slot1"; m.InstallerPaused = true; m.InstallerPauseTime = 2.5;
    m.Update(0.5f);
    CHECK(m.Log.size() == 1 && m.Log[0] == "in1" && g_text == "newprofile");
    CHECK(m.LoadBox == 0 && game.Loaded == "slot1" && m.InstallerResumed && g_unpauses == 2);
    if (!g_fail) printf("00599e3f_TEST PASS\n");
    return g_fail;
}