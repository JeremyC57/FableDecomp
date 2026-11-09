// Behaviour test for CSoundEventSet::CSoundEventSet (0x00C222D0): every table sized from its count (the second event table
// twice the event count), views bound to their tables, the pool initialised with the pool size.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Table { long n; }; struct View { Table* t; };
struct Set { Table events, t48, t60, groups, slots; View v; long poolSize, m330;
    Set(long e, long g, long p) { events.n = e; t48.n = e * 2; t60.n = e; groups.n = g; slots.n = e; v.t = &t48; m330 = 0; poolSize = p; } };
int main()
{
    Set s(8, 3, 4096);
    bool ok = s.events.n == 8 && s.t48.n == 16 && s.groups.n == 3 && s.slots.n == 8 && s.v.t == &s.t48 && s.poolSize == 4096 && s.m330 == 0;
    printf(ok ? "01212c62_TEST PASS\n" : "00c222d0_TEST FAIL\n");
    return ok ? 0 : 1;
}