// Behaviour test for CTB_Bard::GetRandomSpeech (0x00ED99D0): pick a song not among the last three and rotate the history.
#include <cstdio>
#include <cstdlib>
#include <cstring>
typedef const char* Str;
static bool ne(Str a, Str b) { return std::strcmp(a, b) != 0; }
struct Bard {
    Str last1, last2, last3; Str songs[4]; long count; int frames, doneAfter;
    bool IsDone() { return frames >= doneAfter; }
    Str GetRandomSpeech()
    {
        Str empty = ""; Str noSpeech = empty; Str speech = "";
        bool found = false;
        while (!IsDone()) {
            speech = songs[std::rand() % count];
            if (ne(speech, last1) && ne(speech, last2) && ne(speech, last3)) { found = true; break; }
            ++frames;
        }
        if (!found) return noSpeech;
        if (IsDone()) return noSpeech;
        last3 = last2; last2 = last1; last1 = speech;
        if (IsDone()) return noSpeech;
        return speech;
    }
};
int main()
{
    Bard b = { "A", "B", "C", { "A", "B", "C", "D" }, 4, 0, 1000 };
    Str s = b.GetRandomSpeech();                          // only D is new
    if (ne(s, "D") || ne(b.last1, "D") || ne(b.last2, "A") || ne(b.last3, "B")) { std::printf("00ed99d0_TEST FAIL\n"); return 1; }
    Bard c = { "D", "A", "B", { "D", "D", "D", "D" }, 1, 0, 5 };   // nothing new: gives up when done
    Str t = c.GetRandomSpeech();
    if (ne(t, "") || c.frames != 5 || ne(c.last1, "D")) { std::printf("00ed99d0_TEST FAIL\n"); return 1; }
    std::printf("00ed99d0_TEST PASS\n");
    return 0;
}
