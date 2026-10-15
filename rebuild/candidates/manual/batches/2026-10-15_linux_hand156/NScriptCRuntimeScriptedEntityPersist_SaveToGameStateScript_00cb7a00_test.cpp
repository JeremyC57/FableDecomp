// Behaviour test for 0x00CB7A00: SaveToGameStateScript: each persisted entity script is appended as START line with the id, the script, END line
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %%d\n", __LINE__); } } while (0)
struct Entry { unsigned int key; unsigned int id; const char* script; };
static void Save(const Entry* begin, const Entry* end, char* out)
{
    for (const Entry* it = begin; it != end; ++it)
    {
        char num[32]; std::sprintf(num, "%u", it->id);
        std::strcat(out, "\r\nSTART_NEW_ENTITY_SCRIPT "); std::strcat(out, num); std::strcat(out, "\r\n");
        std::strcat(out, it->script);
        std::strcat(out, "END_NEW_ENTITY_SCRIPT\r\n");
    }
}
static void Run()
{
    Entry e[2] = { { 1, 42, "A=1;\r\n" }, { 2, 7, "B=2;\r\n" } };
    char out[512]; out[0] = 0;
    Save(e, e + 2, out);
    CHECK(std::strcmp(out, "\r\nSTART_NEW_ENTITY_SCRIPT 42\r\nA=1;\r\nEND_NEW_ENTITY_SCRIPT\r\n\r\nSTART_NEW_ENTITY_SCRIPT 7\r\nB=2;\r\nEND_NEW_ENTITY_SCRIPT\r\n") == 0);
    out[0] = 0; Save(e, e, out); CHECK(out[0] == 0);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00cb7a00_TEST FAIL\n"); return 1; }
    std::printf("00cb7a00_TEST PASS\n");
    return 0;
}
