#include <cstdio>
#include <cstring>
// Behaviour model of CTCPhysicalShieldSpell::CancelMove (0x0079E300)
static long g_soundId; static bool g_soundPlayer; static char g_effBone[8], g_effType[8]; static long g_removed;
class CThing { public: unsigned char Type; void RemoveTC(long t) { g_removed = t; } };
struct CDef { long CancelSound; };
class CTCPhysicalShieldSpell { public:
    CThing* ParentThing; const CDef* PDef; bool Pausable;
    void PlaySound(long id, bool player) { g_soundId = id; g_soundPlayer = player; }
    void PlayEffect(long, const char* bone, const char* type, CThing*, long) { std::strcpy(g_effBone, bone); std::strcpy(g_effType, type); }
    void CancelMove() {
        CThing* owner = ParentThing;
        PlaySound(PDef->CancelSound, owner->Type == 2);
        PlayEffect(0, "END", "", owner, 8);
        Pausable = false;
        owner->RemoveTC(0x93);
    } };
int main()
{
    CThing t = { 2 }; CDef d = { 55 };
    CTCPhysicalShieldSpell s = { &t, &d, true };
    s.CancelMove();
    if (g_soundId != 55 || !g_soundPlayer || std::strcmp(g_effBone, "END") || g_effType[0] || s.Pausable || g_removed != 0x93)
    { std::printf("0079e300_TEST FAIL\n"); return 1; }
    std::printf("0079e300_TEST PASS\n");
    return 0;
}