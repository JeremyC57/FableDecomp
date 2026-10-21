// Behaviour test for CV_ChickenKickingScript::CChickenSign::Main (0x00E64DF0): the sign is made usable and labelled
// with the current sector's text, re-labelled on each sector change, until the sector reaches 7 (or termination).
#include <cstdio>
#include <string>
#include <vector>

struct CParent { long FinalSector; };
struct CSign {
    CParent* ParentClass; std::vector<std::string> TextKeys; bool Usable; std::vector<std::string> Shown;
    std::vector<long> Script; unsigned int Frame; int TerminateAt;
    bool IsActiveThreadTerminating() { return TerminateAt >= 0 && (int)Frame >= TerminateAt; }
    void NewScriptFrame() { if (Frame < Script.size()) ParentClass->FinalSector = Script[Frame]; ++Frame; }
    void Main()
    {
        Usable = true;
        Shown.push_back(TextKeys[ParentClass->FinalSector]);
        long sector = ParentClass->FinalSector;
        while (sector != 7) {
            NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (sector != ParentClass->FinalSector) {
                if (IsActiveThreadTerminating())
                    return;
                Shown.push_back(TextKeys[ParentClass->FinalSector]);
                sector = ParentClass->FinalSector;
            }
        }
        IsActiveThreadTerminating();
    }
};

int main()
{
    CParent p = { 0 };
    CSign s; s.ParentClass = &p; s.Usable = false; s.Frame = 0; s.TerminateAt = -1;
    for (int i = 0; i < 8; ++i) { char b[8]; std::sprintf(b, "T%d", i); s.TextKeys.push_back(b); }
    long seq[] = { 0, 2, 2, 5, 7 };
    s.Script.assign(seq, seq + 5);
    s.Main();
    bool ok = s.Usable && s.Shown.size() == 4 && s.Shown[0] == "T0" && s.Shown[1] == "T2" && s.Shown[2] == "T5" && s.Shown[3] == "T7";
    CSign t = s; p.FinalSector = 1; t.Shown.clear(); t.Frame = 0; t.TerminateAt = 3;
    long seq2[] = { 1, 3, 4, 7 }; t.Script.assign(seq2, seq2 + 4);
    t.Main();
    ok = ok && t.Shown.size() == 2 && t.Shown[1] == "T3";
    if (!ok) { std::printf("00e64df0_TEST FAIL\n"); return 1; }
    std::printf("00e64df0_TEST PASS\n");
    return 0;
}