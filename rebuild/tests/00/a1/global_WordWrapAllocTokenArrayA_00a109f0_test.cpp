// Behaviour test for 0x00A109F0 WordWrap_AllocTokenArrayA: allocates a null-terminated pointer array only when the slot is empty.
#include <cstdio>
#if !defined(_MSC_VER)
#define __fastcall
#endif

void __fastcall WordWrap_AllocTokenArrayA(char*** tokens, long count)
{
    if (*tokens == 0) {
        *tokens = new char*[count + 1];
        (*tokens)[count] = 0;
    }
}

int main()
{
    char** tokens = 0;
    WordWrap_AllocTokenArrayA(&tokens, 3);
    if (tokens == 0 || tokens[3] != 0) { std::printf("00a109f0_TEST FAIL\n"); return 1; }
    char** keep = tokens;
    WordWrap_AllocTokenArrayA(&tokens, 7);          // already allocated: untouched
    if (tokens != keep) { std::printf("00a109f0_TEST FAIL\n"); return 1; }
    delete[] tokens;
    char** empty = 0;
    WordWrap_AllocTokenArrayA(&empty, 0);
    if (empty == 0 || empty[0] != 0) { std::printf("00a109f0_TEST FAIL\n"); return 1; }
    delete[] empty;
    std::printf("00a109f0_TEST PASS\n");
    return 0;
}