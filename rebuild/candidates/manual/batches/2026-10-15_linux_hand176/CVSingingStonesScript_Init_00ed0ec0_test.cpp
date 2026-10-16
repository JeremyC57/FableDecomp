// Behaviour test for 0x00ed0ec0: CV_SingingStonesScript::Init: play lists, stone names A..D, door-man flags
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

struct S { long idx; long play[4]; long rude[4]; long personal[4]; const char* text[4]; bool door[4];
  void Init() { idx = 0; play[0] = 3; play[1] = 1; play[2] = 0; play[3] = 2; rude[0] = 2; rude[1] = 3; rude[2] = 1; rude[3] = 0; text[0] = "A"; text[1] = "B"; text[2] = "C"; text[3] = "D"; door[0] = door[1] = door[2] = door[3] = false; } };
static void run() { S s; std::memset(&s, 1, sizeof(s)); s.personal[2] = 9; s.Init(); CHECK(s.idx == 0 && s.play[0] == 3 && s.play[3] == 2 && s.rude[1] == 3 && s.rude[3] == 0 && s.personal[2] == 9); CHECK(!std::strcmp(s.text[0], "A") && !std::strcmp(s.text[3], "D") && !s.door[0] && !s.door[3]); }

int main()
{
    run();
    if (g_fail) { std::printf("00ed0ec0_TEST FAIL\n"); return 1; }
    std::printf("00ed0ec0_TEST PASS\n");
    return 0;
}
