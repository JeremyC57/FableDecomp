// behaviour test for 0x0075CDF0 CTCCameraPointScripted validity check
#include <cstdio>
#include <cstring>
struct CCharString { const char* s; CCharString& operator=(const char* p) { s = p; return *this; } };
static float g_eps = 0.0001f;
struct SP {
    unsigned long long ThingUID; float TransitionTime; bool LookDirSet, StartPosSet, EndPosSet;
    bool IsValid(CCharString& error) const
    {
        if (!StartPosSet) { error = "start pos not set"; return false; }
        if (!LookDirSet && ThingUID == 0) { error = "look at direction or thing not set"; return false; }
        if (!EndPosSet && TransitionTime > g_eps) { error = "I'm transitioning but there's no end pos to go to"; return false; }
        return true;
    }
};
int main()
{
    CCharString e; e.s = "";
    SP p; p.ThingUID = 0; p.TransitionTime = 0; p.LookDirSet = false; p.StartPosSet = false; p.EndPosSet = false;
    bool ok = !p.IsValid(e) && !std::strcmp(e.s, "start pos not set");
    p.StartPosSet = true;
    ok = ok && !p.IsValid(e) && e.s[0] == 'l';
    p.ThingUID = 1ULL << 40;
    ok = ok && p.IsValid(e);
    p.TransitionTime = 1.0f;
    ok = ok && !p.IsValid(e) && e.s[0] == 'I';
    p.EndPosSet = true;
    ok = ok && p.IsValid(e);
    if (!ok) { std::printf("0075cdf0_TEST FAIL\n"); return 1; }
    std::printf("0075cdf0_TEST PASS\n");
    return 0;
}
