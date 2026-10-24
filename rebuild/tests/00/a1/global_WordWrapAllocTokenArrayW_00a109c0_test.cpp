// Behaviour test for 0x00A109C0 WordWrap_AllocTokenArrayW: allocates a null-terminated pointer array only when the slot is empty.
#include <cstdio>
#if !defined(_MSC_VER)
#define __fastcall
#endif

void __fastcall WordWrap_AllocTokenArrayW(unsigned short*** tokens, long count)
{
    if (*tokens == 0) {
        *tokens = new unsigned short*[count + 1];
        (*tokens)[count] = 0;
    }
}

int main()
{
    unsigned short** tokens = 0;
    WordWrap_AllocTokenArrayW(&tokens, 3);
    if (tokens == 0 || tokens[3] != 0) { std::printf("00a109c0_TEST FAIL\n"); return 1; }
    unsigned short** keep = tokens;
    WordWrap_AllocTokenArrayW(&tokens, 7);          // already allocated: untouched
    if (tokens != keep) { std::printf("00a109c0_TEST FAIL\n"); return 1; }
    delete[] tokens;
    unsigned short** empty = 0;
    WordWrap_AllocTokenArrayW(&empty, 0);
    if (empty == 0 || empty[0] != 0) { std::printf("00a109c0_TEST FAIL\n"); return 1; }
    delete[] empty;
    std::printf("00a109c0_TEST PASS\n");
    return 0;
}