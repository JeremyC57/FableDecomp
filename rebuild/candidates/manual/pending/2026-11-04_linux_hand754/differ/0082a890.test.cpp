// Behaviour test for CMiniMapDisplay::AddMarker (0x0082A890): a model of the row.
#include <stdio.h>

struct Marker { const void* thing; long graphic; };
struct Display { Marker m[8]; int n; };
static void AddMarker(Display& d, const void* thing, long graphic)
{
    if (graphic <= 0) return;
    for (int i = 0; i < d.n; ++i) if (d.m[i].thing == thing && d.m[i].graphic == graphic) return;
    d.m[d.n].thing = thing; d.m[d.n++].graphic = graphic;
}
static int run()
{
    int a, b; Display d; d.n = 0;
    AddMarker(d, &a, 1); AddMarker(d, &a, 1); AddMarker(d, &a, 2); AddMarker(d, &b, 1); AddMarker(d, &b, 0);
    return d.n == 3;
}
int main()
{
    int ok = run();
    printf(ok ? "0082a890_TEST PASS\n" : "0082a890_TEST FAIL\n");
    return ok ? 0 : 1;
}
