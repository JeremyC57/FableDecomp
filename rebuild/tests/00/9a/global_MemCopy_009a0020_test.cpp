// behaviour test for 0x009A0020 (raw byte copy)
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#endif
void __fastcall MemCopy(void* dest, const void* source, unsigned long size)
{
    memcpy(dest, source, size);
}
int main()
{
    char src[16] = "abcdefghijklmno";
    char dst[16];
    memset(dst, '.', sizeof(dst));
    MemCopy(dst, src, 7);           // one dword + three tail bytes
    MemCopy(dst + 8, src + 8, 0);   // nothing
    int ok = memcmp(dst, "abcdefg", 7) == 0 && dst[7] == '.' && dst[8] == '.';
    MemCopy(dst, src, 16);
    ok = ok && memcmp(dst, src, 16) == 0;
    if (!ok) { printf("009a0020_TEST FAIL\n"); return 1; }
    printf("009a0020_TEST PASS\n");
    return 0;
}