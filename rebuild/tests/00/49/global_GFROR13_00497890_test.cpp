// Behaviour test for 0x00497890 GFROR13 (rotate right by 13).
#include <cstdio>
#ifdef _MSC_VER
static unsigned long __fastcall GFROR13(unsigned long value) 
{
    __asm ror value, 13
    return value;
}
#else
static unsigned long GFROR13(unsigned long value) { unsigned int v = (unsigned int)value; __asm__("rorl $13, %0" : "+r"(v)); return v; }
#endif
int main()
{
    if (GFROR13(0x00002000UL) != 1UL || GFROR13(1UL) != 0x00080000UL || GFROR13(0x12345678UL) != 0xB3C091A2UL
        || GFROR13(0) != 0 || GFROR13(0xFFFFFFFFUL) != 0xFFFFFFFFUL) { std::printf("00497890_TEST FAIL\n"); return 1; }
    std::printf("00497890_TEST PASS\n");
    return 0;
}
