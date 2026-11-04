// Behaviour test for CSoundEvent::GetPlaybackKind (0x00C206E0): states 2..4 -> 0, 5 -> 1, 6 -> 2, others -> -1.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static long Kind(unsigned short state) { switch (state) { case 2: case 3: case 4: return 0; case 5: return 1; case 6: return 2; } return -1; }
int main()
{
    bool ok = Kind(0) == -1 && Kind(1) == -1 && Kind(2) == 0 && Kind(4) == 0 && Kind(5) == 1 && Kind(6) == 2 && Kind(7) == -1;
    printf(ok ? "00c206e0_TEST PASS\n" : "00c206e0_TEST FAIL\n");
    return ok ? 0 : 1;
}