// Behaviour test for CGameTimeManager::ConsoleSetDaySpeed (0x006BB940): time step = 1/speed when speed > 0.0001, else 0.
#include <cstdio>
class CGenericVar { public: int Type; float ValFloat; };
struct Args { CGenericVar* Start; CGenericVar* Finish; const CGenericVar& operator[](unsigned int n) const { return Start[n]; } };
class CGameTimeManager { public:
    float TimeStep;
    static void ConsoleSetDaySpeed(CGameTimeManager& m, const Args& args)
    {
        if (args[0].ValFloat > 0.0001f) m.TimeStep = 1.0f / args[0].ValFloat;
        else m.TimeStep = 0.0f;
    }
};
int main()
{
    CGenericVar v; Args a; a.Start = &v; a.Finish = &v + 1;
    CGameTimeManager m; m.TimeStep = 5.0f;
    v.ValFloat = 4.0f; CGameTimeManager::ConsoleSetDaySpeed(m, a);
    if (m.TimeStep != 0.25f) { std::printf("006bb940_TEST FAIL\n"); return 1; }
    v.ValFloat = 0.0001f; CGameTimeManager::ConsoleSetDaySpeed(m, a);
    if (m.TimeStep != 0.0f) { std::printf("006bb940_TEST FAIL\n"); return 1; }
    v.ValFloat = -3.0f; m.TimeStep = 1.0f; CGameTimeManager::ConsoleSetDaySpeed(m, a);
    if (m.TimeStep != 0.0f) { std::printf("006bb940_TEST FAIL\n"); return 1; }
    std::printf("006bb940_TEST PASS\n");
    return 0;
}