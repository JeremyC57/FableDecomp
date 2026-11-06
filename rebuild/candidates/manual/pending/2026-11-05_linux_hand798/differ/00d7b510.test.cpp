// Behaviour test for CQ_HeroSoulsThunderScript::CHeroThunder::Main (0x00D7B510), talk-line numbering: the talk text is the
// Whisper/Lady Grey base plus the line number; a number with no text entry restarts at 10; each talk advances it by 10.
#include <cstdio>
#include <cstring>

static bool TextEntryExists(long n) { return n <= 30; }

struct Thunder {
    long line_number; char text[96];
    void Talk(bool killed, bool married)
    {
        const char* base = killed ? (married ? "KILLED_MARRIED_" : "KILLED_NOTMARRIED_") : (married ? "NOTKILLED_MARRIED_" : "NOTKILLED_NOTMARRIED_");
        if (!TextEntryExists(line_number)) line_number = 10;
        std::sprintf(text, "%s%ld", base, line_number);
        line_number += 10;
    }
};

int main()
{
    Thunder t; t.line_number = 10;
    t.Talk(true, false); bool ok = std::strcmp(t.text, "KILLED_NOTMARRIED_10") == 0;
    t.Talk(false, true); ok = ok && std::strcmp(t.text, "NOTKILLED_MARRIED_20") == 0;
    t.Talk(false, false); ok = ok && std::strcmp(t.text, "NOTKILLED_NOTMARRIED_30") == 0;
    t.Talk(true, true); ok = ok && std::strcmp(t.text, "KILLED_MARRIED_10") == 0 && t.line_number == 20;
    if (!ok) { std::printf("%s 00d7b510_TEST FAIL\n", t.text); return 1; }
    std::printf("00d7b510_TEST PASS\n");
    return 0;
}
