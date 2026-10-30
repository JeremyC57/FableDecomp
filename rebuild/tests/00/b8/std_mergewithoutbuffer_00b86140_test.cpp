// Behaviour test for std::__merge_without_buffer over CSortElement (0x00B86140): merging two sorted runs in place gives
// a sorted, stable result, through a copy of the STLport algorithm with its own lower/upper_bound and rotate.
#include <cstdio>
struct Elem { unsigned long SortValue; int Tag; };
static bool Less(const Elem& a, const Elem& b) { return a.SortValue < b.SortValue; }
static Elem* LowerBound(Elem* f, Elem* l, const Elem& v)
{ int len = l - f; while (len > 0) { int h = len >> 1; Elem* m = f + h; if (Less(*m, v)) { f = m + 1; len = len - h - 1; } else len = h; } return f; }
static Elem* UpperBound(Elem* f, Elem* l, const Elem& v)
{ int len = l - f; while (len > 0) { int h = len >> 1; Elem* m = f + h; if (Less(v, *m)) len = h; else { f = m + 1; len = len - h - 1; } } return f; }
static void Reverse(Elem* f, Elem* l) { while (f < --l) { Elem t = *f; *f = *l; *l = t; ++f; } }
static Elem* Rotate(Elem* f, Elem* m, Elem* l) { Reverse(f, m); Reverse(m, l); Reverse(f, l); return f + (l - m); }
static void Merge(Elem* first, Elem* middle, Elem* last, int len1, int len2)
{
    if (len1 == 0 || len2 == 0) return;
    if (len1 + len2 == 2) { if (Less(*middle, *first)) { Elem t = *first; *first = *middle; *middle = t; } return; }
    Elem* first_cut = first; Elem* second_cut = middle; int len11 = 0, len22 = 0;
    if (len1 > len2) { len11 = len1 / 2; first_cut += len11; second_cut = LowerBound(middle, last, *first_cut); len22 += second_cut - middle; }
    else { len22 = len2 / 2; second_cut += len22; first_cut = UpperBound(first, middle, *second_cut); len11 += first_cut - first; }
    Elem* new_middle = Rotate(first_cut, middle, second_cut);
    Merge(first, first_cut, new_middle, len11, len22);
    Merge(new_middle, second_cut, last, len1 - len11, len2 - len22);
}
int main()
{
    unsigned long a[] = { 1, 3, 3, 5, 8, 9, 12 };            // run 1 (tags 0..6)
    unsigned long b[] = { 2, 3, 4, 9, 10 };                  // run 2 (tags 100..104)
    Elem e[12]; int i;
    for (i = 0; i < 7; i++) { e[i].SortValue = a[i]; e[i].Tag = i; }
    for (i = 0; i < 5; i++) { e[7 + i].SortValue = b[i]; e[7 + i].Tag = 100 + i; }
    Merge(e, e + 7, e + 12, 7, 5);
    bool ok = true;
    for (i = 1; i < 12; i++) {
        if (e[i - 1].SortValue > e[i].SortValue) ok = false;
        if (e[i - 1].SortValue == e[i].SortValue && e[i - 1].Tag > e[i].Tag) ok = false;   // stable: run 1 first
    }
    Elem p[2]; p[0].SortValue = 7; p[0].Tag = 0; p[1].SortValue = 4; p[1].Tag = 1;    // the two-element swap arm
    Merge(p, p + 1, p + 2, 1, 1);
    ok = ok && p[0].SortValue == 4 && p[1].Tag == 0;
    if (!ok) { std::printf("00b86140_TEST FAIL\n"); return 1; }
    std::printf("00b86140_TEST PASS\n");
    return 0;
}