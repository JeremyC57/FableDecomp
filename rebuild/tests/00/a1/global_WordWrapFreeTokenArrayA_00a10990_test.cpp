// Behaviour test for 0x00A10990 WordWrap_FreeTokenArrayA: deletes every token of a null-terminated array, then the array itself.
#include <cstdio>
#include <cstdlib>
#if !defined(_MSC_VER)
#define __fastcall
#endif

static int g_deletes = 0;
void operator delete(void* p) throw() { if (p) ++g_deletes; std::free(p); }
void operator delete[](void* p) throw() { if (p) ++g_deletes; std::free(p); }

void __fastcall WordWrap_FreeTokenArrayA(char** tokens)
{
    if (tokens) {
        char** array = tokens;
        for (; *tokens; ++tokens)
            delete[] *tokens;
        delete[] array;
    }
}

int main()
{
    WordWrap_FreeTokenArrayA(0);                     // null array: nothing
    if (g_deletes != 0) { std::printf("00a10990_TEST FAIL\n"); return 1; }
    char** tokens = new char*[4];
    tokens[0] = new char[3];
    tokens[1] = new char[5];
    tokens[2] = new char[1];
    tokens[3] = 0;
    WordWrap_FreeTokenArrayA(tokens);
    if (g_deletes != 4) { std::printf("00a10990_TEST FAIL\n"); return 1; }
    char** none = new char*[1];
    none[0] = 0;
    WordWrap_FreeTokenArrayA(none);
    if (g_deletes != 5) { std::printf("00a10990_TEST FAIL\n"); return 1; }
    std::printf("00a10990_TEST PASS\n");
    return 0;
}