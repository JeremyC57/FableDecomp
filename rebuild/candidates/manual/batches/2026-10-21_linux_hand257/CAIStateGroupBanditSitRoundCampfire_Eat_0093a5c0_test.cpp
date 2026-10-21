// Behaviour test for CAIStateGroup_BanditSitRoundCampfire::Eat at 0x0093A5C0: the eating stage runs only after a completed cook.
#include <cstdio>
#include <cstring>
static char g_log[256];
static int g_results[2];
static int g_call;
static int Perform(const char* into) { std::strcat(g_log, into); std::strcat(g_log, ";"); return g_results[g_call++]; }
static bool Eat()
{
    std::strcat(g_log, "SHEATHE;");
    int result = Perform("ST_CHICKEN_COOKING_INTO");
    if (result == 2)
        result = Perform("ST_CHICKEN_EATING_INTO");
    std::strcat(g_log, "DESTROY;");
    return result == 2;
}
int main()
{
    int ok = 1;
    g_results[0] = 2; g_results[1] = 2; g_call = 0; g_log[0] = 0;
    ok &= Eat() && std::strcmp(g_log, "SHEATHE;ST_CHICKEN_COOKING_INTO;ST_CHICKEN_EATING_INTO;DESTROY;") == 0;
    g_results[0] = 1; g_call = 0; g_log[0] = 0;
    ok &= !Eat() && std::strcmp(g_log, "SHEATHE;ST_CHICKEN_COOKING_INTO;DESTROY;") == 0;
    g_results[0] = 2; g_results[1] = 3; g_call = 0; g_log[0] = 0;
    ok &= !Eat();
    std::printf(ok ? "0093a5c0_TEST PASS\n" : "0093a5c0_TEST FAIL\n");
    return ok ? 0 : 1;
}
