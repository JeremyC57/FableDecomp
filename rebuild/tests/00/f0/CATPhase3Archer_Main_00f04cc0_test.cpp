// Behaviour test for 00f04cc0 CAT_Phase3Archer::Main: ally the spy with live guards, target selection, wait, exit walk.
#include <cstdio>
#include <cstring>
#include <string>
struct Sim {
    std::string log;
    bool spyAlive, exitAlive;
    bool guards[3];
    int phaseWait, walkWait;
    void Run()
    {
        for (int i = 0; i < 3; ++i)
            if (guards[i] && spyAlive) { char b[8]; std::sprintf(b, "A%d", i); log += b; }
        if (spyAlive) log += "E+T(spy)";
        else log += "T(hero)";
        log += "EH;M;";
        while (phaseWait-- > 0) log += "f";
        log += "S;";
        if (exitAlive) {
            log += "W";
            while (walkWait-- > 0) log += "f";
            log += "K";
        }
    }
};
int main()
{
    Sim a; a.spyAlive = true; a.exitAlive = true; a.guards[0] = true; a.guards[1] = false; a.guards[2] = true;
    a.phaseWait = 2; a.walkWait = 1; a.Run();
    Sim b; b.spyAlive = false; b.exitAlive = false; b.guards[0] = b.guards[1] = b.guards[2] = true;
    b.phaseWait = 0; b.walkWait = 0; b.Run();
    if (a.log != "A0A2E+T(spy)EH;M;ffS;WfK" || b.log != "T(hero)EH;M;S;") {
        std::printf("00f04cc0_TEST FAIL\n");
        return 1;
    }
    std::printf("00f04cc0_TEST PASS\n");
    return 0;
}