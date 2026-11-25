// Behaviour test for 0x00C3E290 (placeholder T::Get) (NF415: linked): links the row object (T::Get is declared here, not defined) and
// checks the value it returns is the address of the string "Win32" (retail 0x012B5B78).
#include <stdio.h>
#include <string.h>
struct T { int Get(); };
int main(void)
{
    T t;
    const char* s = (const char*)t.Get();
    if (s == 0 || strcmp(s, "Win32") != 0) { printf("FAIL_00c3e290\n"); return 1; }
    printf("PASS_00c3e290\n");
    return 0;
}
