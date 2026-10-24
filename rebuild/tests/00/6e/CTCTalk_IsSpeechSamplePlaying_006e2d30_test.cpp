// Behaviour test for 0x006E2D30 CTCTalk::IsSpeechSamplePlaying: false without a positive sample, an attached emitter
// or its sound source; otherwise the source's answer for (speech bank or default bank, sample).
#include <cstdio>

struct Bank { int id; };
static Bank g_default;
struct Source { long lastSample; const Bank* lastBank; bool answer;
    bool IsSamplePlaying(const Bank& b, long s) { lastBank = &b; lastSample = s; return answer; } };
struct Emitter { Source* PSoundSource; };
struct Player { Emitter* attached; Emitter* GetPItem() const { return attached; } };
struct Thing { Player* sound; bool GetInterface(Player** out, long) const { if (!sound) return false; *out = sound; return true; } };
struct Talk {
    const Thing* ParentThing; long SpeechSample; Bank* PSpeechBank;
    const Bank& GetSpeechBank() const { return PSpeechBank ? *PSpeechBank : g_default; }
    bool IsSpeechSamplePlaying() const
    {
        Player* player = 0;
        ParentThing->GetInterface(&player, 16);
        if (SpeechSample > 0 && player->GetPItem()) {
            Source* source = player->GetPItem()->PSoundSource;
            if (source) return source->IsSamplePlaying(GetSpeechBank(), SpeechSample);
        }
        return false;
    }
};

#define FAIL { std::printf("006e2d30_TEST FAIL\n"); return 1; }
int main()
{
    Source src; src.answer = true; src.lastSample = 0; src.lastBank = 0;
    Emitter em; em.PSoundSource = &src;
    Player pl; pl.attached = &em;
    Thing th; th.sound = &pl;
    Talk t; t.ParentThing = &th; t.SpeechSample = 0; t.PSpeechBank = 0;
    if (t.IsSpeechSamplePlaying()) FAIL;
    t.SpeechSample = 4;
    if (!t.IsSpeechSamplePlaying() || src.lastSample != 4 || src.lastBank != &g_default) FAIL;
    Bank mine; t.PSpeechBank = &mine; src.answer = false;
    if (t.IsSpeechSamplePlaying() || src.lastBank != &mine) FAIL;
    em.PSoundSource = 0; src.answer = true;
    if (t.IsSpeechSamplePlaying()) FAIL;
    pl.attached = 0;
    if (t.IsSpeechSamplePlaying()) FAIL;
    std::printf("006e2d30_TEST PASS\n");
    return 0;
}