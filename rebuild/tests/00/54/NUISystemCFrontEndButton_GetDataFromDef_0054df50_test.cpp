// behaviour test for NUISystem::CFrontEndButton::GetDataFromDef 0x0054DF50: a nonzero def action becomes one action of that
// type (a movie action 0x41 carries the movie file name as a string parameter); the input delay is always copied
#include <cstdio>
#include <string>
#include <vector>
struct Action { long Type; std::vector<std::wstring> Params; };
struct Def { long Action; float InputDelay; std::wstring Movie; };
static void Get(const Def& d, std::vector<Action>& actions, float& delay)
{
    if (d.Action) {
        Action a; a.Type = d.Action;
        if (a.Type == 0x41) a.Params.push_back(d.Movie);
        actions.push_back(a);
    }
    delay = d.InputDelay;
}
int main()
{
    std::vector<Action> acts; float delay = 0;
    Def m = { 0x41, 0.25f, L"intro.bik" }, n = { 0, 0.5f, L"" }, o = { 3, 1.0f, L"x" };
    Get(m, acts, delay); Get(n, acts, delay); Get(o, acts, delay);
    int ok = acts.size() == 2 && acts[0].Params.size() == 1 && acts[0].Params[0] == L"intro.bik" && acts[1].Params.empty() && delay == 1.0f;
    std::printf(ok ? "0054df50_TEST PASS\n" : "0054df50_TEST FAIL\n");
    return !ok;
}