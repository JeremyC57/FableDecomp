// Behaviour test for STLport __merge_adaptive<CSortElement*, int, CSortElement*, less<CSortElement> > (0x00B862C0):
// stable merge of two sorted runs with buffers of several sizes (buffered arms and the split / rotate / recurse arm).
#include <cstdio>

struct CSortElement { unsigned int SortValue; int Tag; };
struct Less { bool operator()(const CSortElement& a, const CSortElement& b) const { return a.SortValue < b.SortValue; } };

static CSortElement* copy_(CSortElement* f, CSortElement* l, CSortElement* r) { for (int n = (int)(l - f); n > 0; --n) *r++ = *f++; return r; }
static CSortElement* copy_backward_(CSortElement* f, CSortElement* l, CSortElement* r) { for (int n = (int)(l - f); n > 0; --n) *--r = *--l; return r; }
static CSortElement* merge_(CSortElement* f1, CSortElement* l1, CSortElement* f2, CSortElement* l2, CSortElement* r, Less c)
{
    while (f1 != l1 && f2 != l2) { if (c(*f2, *f1)) *r = *f2++; else *r = *f1++; ++r; }
    return copy_(f2, l2, copy_(f1, l1, r));
}
static CSortElement* merge_backward_(CSortElement* f1, CSortElement* l1, CSortElement* f2, CSortElement* l2, CSortElement* r, Less c)
{
    if (f1 == l1) return copy_backward_(f2, l2, r);
    if (f2 == l2) return copy_backward_(f1, l1, r);
    --l1; --l2;
    for (;;) {
        if (c(*l2, *l1)) { *--r = *l1; if (f1 == l1) return copy_backward_(f2, ++l2, r); --l1; }
        else { *--r = *l2; if (f2 == l2) return copy_backward_(f1, ++l1, r); --l2; }
    }
}
static CSortElement* lower_bound_(CSortElement* f, CSortElement* l, const CSortElement& v, Less c)
{ int len = (int)(l - f); while (len > 0) { int h = len >> 1; CSortElement* m = f + h; if (c(*m, v)) { f = m + 1; len = len - h - 1; } else len = h; } return f; }
static CSortElement* upper_bound_(CSortElement* f, CSortElement* l, const CSortElement& v, Less c)
{ int len = (int)(l - f); while (len > 0) { int h = len >> 1; CSortElement* m = f + h; if (c(v, *m)) len = h; else { f = m + 1; len = len - h - 1; } } return f; }
static CSortElement* rotate_(CSortElement* f, CSortElement* m, CSortElement* l)
{   // simple reversal rotate
    CSortElement* a; CSortElement* b; CSortElement t;
    for (a = f, b = m - 1; a < b; ++a, --b) { t = *a; *a = *b; *b = t; }
    for (a = m, b = l - 1; a < b; ++a, --b) { t = *a; *a = *b; *b = t; }
    for (a = f, b = l - 1; a < b; ++a, --b) { t = *a; *a = *b; *b = t; }
    return f + (l - m);
}
static CSortElement* rotate_adaptive_(CSortElement* f, CSortElement* m, CSortElement* l, int len1, int len2, CSortElement* buf, int bs)
{
    if (len1 > len2 && len2 <= bs) { CSortElement* e = copy_(m, l, buf); copy_backward_(f, m, l); return copy_(buf, e, f); }
    else if (len1 <= bs) { CSortElement* e = copy_(f, m, buf); copy_(m, l, f); return copy_backward_(buf, e, l); }
    return rotate_(f, m, l);
}

static void merge_adaptive(CSortElement* first, CSortElement* middle, CSortElement* last, int len1, int len2,
                           CSortElement* buffer, int buffer_size, Less comp)
{
    if (len1 <= len2 && len1 <= buffer_size) {
        CSortElement* buffer_end = copy_(first, middle, buffer);
        merge_(buffer, buffer_end, middle, last, first, comp);
    }
    else if (len2 <= buffer_size) {
        CSortElement* buffer_end = copy_(middle, last, buffer);
        merge_backward_(first, middle, buffer, buffer_end, last, comp);
    }
    else {
        CSortElement* first_cut = first;
        CSortElement* second_cut = middle;
        int len11 = 0, len22 = 0;
        if (len1 > len2) {
            len11 = len1 / 2;
            first_cut += len11;
            second_cut = lower_bound_(middle, last, *first_cut, comp);
            len22 += (int)(second_cut - middle);
        }
        else {
            len22 = len2 / 2;
            second_cut += len22;
            first_cut = upper_bound_(first, middle, *second_cut, comp);
            len11 += (int)(first_cut - first);
        }
        CSortElement* new_middle = rotate_adaptive_(first_cut, middle, second_cut, len1 - len11, len22, buffer, buffer_size);
        merge_adaptive(first, first_cut, new_middle, len11, len22, buffer, buffer_size, comp);
        merge_adaptive(new_middle, second_cut, last, len1 - len11, len2 - len22, buffer, buffer_size, comp);
    }
}

static int run(int len1, int len2, int bs, unsigned int seed)
{
    CSortElement a[64]; CSortElement buf[64];
    int i;
    unsigned int v = 0;
    for (i = 0; i < len1; ++i) { v += (seed = seed * 1103515245u + 12345u) >> 29; a[i].SortValue = v; a[i].Tag = i; }
    v = 0;
    for (i = 0; i < len2; ++i) { v += (seed = seed * 1103515245u + 12345u) >> 29; a[len1 + i].SortValue = v; a[len1 + i].Tag = 100 + i; }
    merge_adaptive(a, a + len1, a + len1 + len2, len1, len2, buf, bs, Less());
    for (i = 1; i < len1 + len2; ++i) {
        if (a[i - 1].SortValue > a[i].SortValue) return 0;
        if (a[i - 1].SortValue == a[i].SortValue && a[i - 1].Tag > a[i].Tag) return 0;   // stable: first run first
    }
    return 1;
}

int main()
{
    int bs, l1, l2;
    for (bs = 1; bs <= 8; ++bs)   // stable_sort always passes a buffer of at least one element
        for (l1 = 1; l1 <= 20; l1 += 3)
            for (l2 = 1; l2 <= 20; l2 += 4)
                if (!run(l1, l2, bs, (unsigned int)(bs * 977 + l1 * 31 + l2))) { std::printf("00b862c0_TEST FAIL\n"); return 1; }
    std::printf("00b862c0_TEST PASS\n");
    return 0;
}