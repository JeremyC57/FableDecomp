// Behaviour test for 00f051d0 CAT_Phase3Guard::Main: ally the spy with live guards, enemies, wait, exit walk.
#include <cstdio>
#include <cstring>
#include <string>
struct Sim {
    std::string log;
    // wander setup (centre = own position, min 0, max from the def, state group 4) precedes the spy lookup
    bool spyAlive, exitAlive;
    bool guards[3];
    int phaseWait, walkWait;
    void Run()
    {
        for (int i = 0; i < 3; ++i)
            if (guards[i] && spyAlive) { char b[8]; std::sprintf(b, "A%d", i); log += b; }
        if (spyAlive) log += "E(spy)";
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
    if (a.log != "A0A2E(spy)EH;M;ffS;WfK" || b.log != "EH;M;S;") {
        std::printf("00f051d0_TEST FAIL\n");
        return 1;
    }
    std::printf("00f051d0_TEST PASS\n");
    return 0;
}