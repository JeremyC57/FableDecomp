// Behaviour test for 0x00A10960 WordWrap_FreeTokenArrayW: deletes every token of a null-terminated array, then the array itself.
#include <cstdio>
#include <cstdlib>
#if !defined(_MSC_VER)
#define __fastcall
#endif

static int g_deletes = 0;
void operator delete(void* p) throw() { if (p) ++g_deletes; std::free(p); }
void operator delete[](void* p) throw() { if (p) ++g_deletes; std::free(p); }

void __fastcall WordWrap_FreeTokenArrayW(unsigned short** tokens)
{
    if (tokens) {
        unsigned short** array = tokens;
        for (; *tokens; ++tokens)
            delete[] *tokens;
        delete[] array;
    }
}

int main()
{
    WordWrap_FreeTokenArrayW(0);                     // null array: nothing
    if (g_deletes != 0) { std::printf("00a10960_TEST FAIL\n"); return 1; }
    unsigned short** tokens = new unsigned short*[4];
    tokens[0] = new unsigned short[3];
    tokens[1] = new unsigned short[5];
    tokens[2] = new unsigned short[1];
    tokens[3] = 0;
    WordWrap_FreeTokenArrayW(tokens);
    if (g_deletes != 4) { std::printf("00a10960_TEST FAIL\n"); return 1; }
    unsigned short** none = new unsigned short*[1];
    none[0] = 0;
    WordWrap_FreeTokenArrayW(none);
    if (g_deletes != 5) { std::printf("00a10960_TEST FAIL\n"); return 1; }
    std::printf("00a10960_TEST PASS\n");
    return 0;
}