// Behaviour test for CTCTalk::OnCreate (0x006E2C00): a model of the row -- a talker without a dialogue voice picks one at random
// from its creature def's voices; a talker that has one keeps it.
#include <stdio.h>
#include <string>
#include <vector>
static unsigned long g_seed = 1;
static unsigned long Random(unsigned long range) { g_seed = g_seed * 0x24A1 + 0x24DF; return range ? g_seed % range : 0; }
static void OnCreate(std::string& voice, const std::vector<std::string>& voices)
{
    if (voice.empty() && !voices.empty()) voice = voices[Random(voices.size())];
}
int main()
{
    std::vector<std::string> v; v.push_back("A"); v.push_back("B"); v.push_back("C");
    std::string none, kept = "Z";
    OnCreate(none, v); OnCreate(kept, v);
    int ok = (none == "A" || none == "B" || none == "C") && kept == "Z";
    printf(ok ? "006e2c00_TEST PASS\n" : "006e2c00_TEST FAIL\n");
    return ok ? 0 : 1;
}
