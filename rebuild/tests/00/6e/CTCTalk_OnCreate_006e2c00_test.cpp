// Behaviour test for 0x006E2C00 CTCTalk::OnCreate(): the row's logic over a flat model (std::string voice, a vector of
// string-table ids, a fixed random seed); checks the kept voice, the missing creature def, the empty voice list and the pick.
#include <cstdio>
#include <string>
#include <vector>
static unsigned long g_seed = 0;
static unsigned long GFRandom(unsigned long range, unsigned long* seed) { *seed = *seed * 1664525u + 1013904223u; return (*seed >> 8) % range; }
static std::string GetString(long id) { char b[16]; std::sprintf(b, "voice%ld", id); return b; }
struct CCreatureDef { std::vector<long> DialogueVoices; };
struct CTCTalk {
    std::string DialogueVoice;
    CCreatureDef* SubDef;            // the creature sub-def of the parent's def (0 when it has none)
    unsigned long* GetPWorldSeed() const { return &g_seed; }
    void OnCreate();
};
void CTCTalk::OnCreate()
{
    if (DialogueVoice.empty()) {
        CCreatureDef* creatureDef = SubDef;
        if (creatureDef && creatureDef->DialogueVoices.size() > 0) {
            unsigned long index = GFRandom(creatureDef->DialogueVoices.size(), GetPWorldSeed());
            DialogueVoice = GetString(creatureDef->DialogueVoices[index]);
        }
    }
}
int main()
{
    bool ok = true;
    CCreatureDef one; one.DialogueVoices.push_back(7);
    CCreatureDef none;
    CCreatureDef three; three.DialogueVoices.push_back(1); three.DialogueVoices.push_back(2); three.DialogueVoices.push_back(3);
    CTCTalk t;
    t.DialogueVoice = "kept"; t.SubDef = &one; t.OnCreate(); ok = ok && t.DialogueVoice == "kept";
    t.DialogueVoice = ""; t.SubDef = 0; t.OnCreate(); ok = ok && t.DialogueVoice.empty();
    t.SubDef = &none; t.OnCreate(); ok = ok && t.DialogueVoice.empty();
    t.SubDef = &one; t.OnCreate(); ok = ok && t.DialogueVoice == "voice7";
    t.DialogueVoice = ""; t.SubDef = &three; t.OnCreate();
    ok = ok && (t.DialogueVoice == "voice1" || t.DialogueVoice == "voice2" || t.DialogueVoice == "voice3");
    std::printf(ok ? "006e2c00_TEST PASS\n" : "006e2c00_TEST FAIL\n");
    return ok ? 0 : 1;
}