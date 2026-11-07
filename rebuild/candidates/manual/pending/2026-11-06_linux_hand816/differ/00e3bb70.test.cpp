// Behaviour test for NScript::CV_BordelloScript::CMadame::Main (0x00E3BB70): the intro picks the lady line when the hero is dressed as
// a lady; the face-the-hero timer only re-arms (to 4) when it has run out and the hero is within 8 units; chit-chat cycles through
// "TEXT_QST_B13_MADAME_CHIT_CHAT_0" + the counter (1..5, wrapping back to 1). A model of those three choices.
#include <cstdio>
#include <cstring>
static const char* IntroArg(bool heroIsLady) { return heroIsLady ? "HEROLADY_10" : "INTRO_10"; }
static bool FaceWhenNear(long& timer, float distance)
{
    if (timer == 0 && distance < 8.0f) { timer = 4; return true; }
    return false;
}
static void ChitChat(char* out, long& chat) { if (chat > 5) chat = 1; std::sprintf(out, "TEXT_QST_B13_MADAME_CHIT_CHAT_0%ld", chat); chat++; }
int main()
{
    long timer = 0;
    bool ok = !std::strcmp(IntroArg(true), "HEROLADY_10") && !std::strcmp(IntroArg(false), "INTRO_10");
    ok = ok && !FaceWhenNear(timer, 9.0f) && timer == 0 && FaceWhenNear(timer, 3.0f) && timer == 4 && !FaceWhenNear(timer, 3.0f);
    char buf[64];
    long chat = 1;
    ChitChat(buf, chat); ok = ok && !std::strcmp(buf, "TEXT_QST_B13_MADAME_CHIT_CHAT_01") && chat == 2;
    chat = 5; ChitChat(buf, chat); ok = ok && !std::strcmp(buf, "TEXT_QST_B13_MADAME_CHIT_CHAT_05");
    ChitChat(buf, chat); ok = ok && !std::strcmp(buf, "TEXT_QST_B13_MADAME_CHIT_CHAT_01") && chat == 2;
    std::printf(ok ? "00e3bb70_TEST PASS\n" : "00e3bb70_TEST FAIL\n");
    return ok ? 0 : 1;
}
