// Behaviour test for std::vector<C2DLineF>::vector(const vector&) (0x00514EB0): the copy holds the same lines in fresh storage.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Line { float x1, y1, x2, y2; };
struct Vec { Line* first; Line* last; Line* end;
    Vec(const Vec& o) { unsigned n = (unsigned)(o.last - o.first); first = (Line*)malloc(n * sizeof(Line)); end = first + n;
        Line* d = first; for (const Line* s = o.first; s != o.last; ++s, ++d) *d = *s; last = d; }
    Vec() : first(0), last(0), end(0) {} };
int main()
{
    Line l[2] = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 } };
    Vec a; a.first = l; a.last = l + 2; a.end = l + 2;
    Vec b(a);
    bool ok = b.last - b.first == 2 && b.first != a.first && b.first[1].y2 == 8.0f && b.end == b.last;
    free(b.first);
    printf(ok ? "00514eb0_TEST PASS\n" : "00514eb0_TEST FAIL\n");
    return ok ? 0 : 1;
}