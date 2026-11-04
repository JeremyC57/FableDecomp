// Behaviour test for CTCInventoryExperience::vfunc_29 (0x0056EF2D, parked DIFFER row): a model of the row -- each arrow highlight is ON when it can move (index above first / below last) else OFF.
#include <stdio.h>
enum { OFF = 1, ON = 2 };
static void Refresh(long index, long count, int& up, int& down) { up = index > 0 ? ON : OFF; down = index < count - 1 ? ON : OFF; }
static int Check() { int u, d; Refresh(0, 3, u, d); int ok = u == OFF && d == ON; Refresh(2, 3, u, d); ok = ok && u == ON && d == OFF; Refresh(1, 3, u, d); return ok && u == ON && d == ON; }
int main()
{
    int ok = Check();
    printf(ok ? "0056ef2d_TEST PASS\n" : "0056ef2d_TEST FAIL\n");
    return ok ? 0 : 1;
}
