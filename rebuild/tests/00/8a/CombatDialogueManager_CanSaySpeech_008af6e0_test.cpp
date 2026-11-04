// Behaviour test for CombatDialogueManager::CanSaySpeech (0x008AF6E0): a model of the row -- true only when speech is allowed for the
// creature, it has a talk component, the group's next allowed frame has been reached, and the talk component has that speech.
#include <stdio.h>
struct Talk { bool has; bool DoesSpeechExist(unsigned long) { return has; } };
struct Creature { bool allowed; Talk* talk; };
static long g_frame = 100;
static bool CanSaySpeech(Creature& c, long nextAllowed)
{
    if (!c.allowed) return false;
    Talk* talk = c.talk; if (!talk) return false;
    if (!(g_frame >= nextAllowed)) return false;
    if (talk->DoesSpeechExist(7)) return true;
    return false;
}
int main()
{
    Talk yes = { true }, no = { false };
    Creature ok1 = { true, &yes }, banned = { false, &yes }, mute = { true, 0 }, empty = { true, &no };
    int ok = CanSaySpeech(ok1, 100) && !CanSaySpeech(ok1, 101) && !CanSaySpeech(banned, 0) && !CanSaySpeech(mute, 0) && !CanSaySpeech(empty, 0);
    printf(ok ? "008af6e0_TEST PASS\n" : "008af6e0_TEST FAIL\n");
    return ok ? 0 : 1;
}