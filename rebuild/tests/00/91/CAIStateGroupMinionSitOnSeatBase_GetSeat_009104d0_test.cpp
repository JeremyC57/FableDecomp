// Behaviour test for CAIStateGroup_MinionSitOnSeatBase::GetSeat (0x009104D0): a model of the row -- the seats passing the owner filter
// are gathered; when there are any, the world seed steps (x * 0x24A1 + 0x24DF, rotated right 13) and seat[seed % count] becomes the seat.
#include <stdio.h>
static unsigned long Ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long g_seed = 1;
static unsigned long GFRandom(unsigned long range, unsigned long* seed) { *seed = *seed * 0x24A1 + 0x24DF; *seed = Ror13(*seed); return range ? *seed % range : 0; }
struct Seat { int owner; };
static Seat* GetSeat(Seat* all, int n, int owner)
{
    Seat* seats[8]; unsigned long count = 0;
    for (int i = 0; i < n; ++i) if (all[i].owner == owner) seats[count++] = &all[i];
    if (count != 0) return seats[GFRandom(count, &g_seed)];
    return 0;
}
int main()
{
    Seat all[4] = { { 1 }, { 2 }, { 1 }, { 3 } };
    unsigned long expect = Ror13(1 * 0x24A1 + 0x24DF);
    Seat* s = GetSeat(all, 4, 1);
    int ok = s == (expect % 2 ? &all[2] : &all[0]) && g_seed == expect;
    ok = ok && GetSeat(all, 4, 9) == 0 && g_seed == expect;
    printf(ok ? "009104d0_TEST PASS\n" : "009104d0_TEST FAIL\n");
    return ok ? 0 : 1;
}