// 00ce1280 behaviour test: CTestGateSwitch::Main (match.cpp body incl. its goto shape, host stand-ins)
#include <stdio.h>
#include <string>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static std::string g_log;
static int g_calls = 0, g_termAt = 0, g_frames = 0;
static int g_usedPattern = 0;
struct CScriptThing {
    int Id;
    bool MsgIsUsedBy(const std::string& user) const { (void)user; return ((g_frames >> g_usedPattern) & 1) != 0 || g_frames == 0; }
};
struct CGameScriptInterface {
    std::string LastMsg; float LastSecs;
    void GetAllThingsWithScriptName(const std::string& n, std::vector<CScriptThing>& out) const
    { if (n == "TestGate") { for (int k = 0; k < 3; ++k) { CScriptThing t; t.Id = k; out.push_back(t); } } }
    void ShowOnScreenMessage(const std::string& m, float s) { LastMsg = m; LastSecs = s; }
    void OpenDoor(const CScriptThing& d) const { char b[8]; sprintf(b, "O%d", d.Id); g_log += b; }
    void CloseDoor(const CScriptThing& d) const { char b[8]; sprintf(b, "C%d", d.Id); g_log += b; }
    void NewScriptFrame() const { ++g_frames; g_log += "|"; }
};
struct CTestGateSwitch {
    CGameScriptInterface* Game; CScriptThing Me; bool GatesOpen;
    bool IsActiveThreadTerminating() const { return ++g_calls >= g_termAt; }
    void Main()
    {
        std::vector<CScriptThing> gates;
        Game->GetAllThingsWithScriptName("TestGate", gates);
        Game->ShowOnScreenMessage("YOUR GATE MUST HAVE A SCRIPT NAME OF 'TestGate' - (case sensitive without the quotes)", 10.0f);
        unsigned int i;
        for (i = 0; i < gates.size(); ++i) {
            if (IsActiveThreadTerminating())
                return;
            if (GatesOpen) {
                if (IsActiveThreadTerminating()) {
ool:
                    return;
                }
                Game->OpenDoor(gates[i]);
            } else {
                if (IsActiveThreadTerminating())
                    return;
                Game->CloseDoor(gates[i]);
            }
        }
        if (IsActiveThreadTerminating())
            return;
        for (;;) {
            if (IsActiveThreadTerminating())
                return;
            if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating())
                    return;
                GatesOpen = !GatesOpen;
                for (unsigned int i = 0; i < gates.size(); ++i) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (GatesOpen) {
                        if (IsActiveThreadTerminating())
                            goto ool;
                        Game->OpenDoor(gates[i]);
                    } else {
                        if (IsActiveThreadTerminating())
                            goto ool;
                        Game->CloseDoor(gates[i]);
                    }
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            Game->NewScriptFrame();
        }
    }
};
static std::string Run(bool open, int termAt, bool* finalOpen)
{
    g_log = ""; g_calls = 0; g_termAt = termAt; g_frames = 0; g_usedPattern = 0;
    CGameScriptInterface gi; CTestGateSwitch s; s.Game = &gi; s.Me.Id = 99; s.GatesOpen = open;
    s.Main();
    CHECK(gi.LastSecs == 10.0f);
    CHECK(gi.LastMsg.find("TestGate") != std::string::npos);
    *finalOpen = s.GatesOpen;
    return g_log;
}
int main()
{
    bool fo;
    // 3 gates x 2 tests = 6 calls in loop 1; 7th = post-loop test.
    CHECK(Run(true, 1, &fo) == "");
    CHECK(Run(true, 2, &fo) == "");
    CHECK(Run(true, 3, &fo) == "O0");
    CHECK(Run(false, 7, &fo) == "C0C1C2");
    // loop 2: frame 0 used -> toggle to closed, close all (2 tests/gate), then post test, frame
    CHECK(Run(true, 100, &fo).substr(0, 7) == "O0O1O2C");
    std::string s = Run(true, 16, &fo);   // 7 + top(8) + toggle(9) + 3 gates*2 (10..15) + post(16)=term
    CHECK(s == "O0O1O2C0C1C2"); CHECK(fo == false);
    s = Run(true, 13, &fo);               // terminates at the 2nd test of gate 1 inside loop 2 (goto ool)
    CHECK(s == "O0O1O2C0"); CHECK(fo == false);
    s = Run(true, 12, &fo);               // terminates at the first test of gate 1
    CHECK(s == "O0O1O2C0");
    s = Run(false, 30, &fo);
    CHECK(s.substr(0, 13) == "C0C1C2O0O1O2|");
    if (g_fail) return 1;
    printf("00ce1280_TEST PASS\n");
    return 0;
}