// Behaviour test for 0x00B85CD0 std::__rotate_adaptive<CSortElement*, CSortElement*, int> (STLport 4.6 _algo.c): the
// result of rotating [first, middle, last) is the same through either buffer path and the in-place fallback.
#include <cstdio>

struct CSortElement { unsigned int SortValue; int Tag; };
static int g_rotateCalls = 0;

static CSortElement* Copy(CSortElement* f, CSortElement* l, CSortElement* r) { for (int n = (int)(l - f); n > 0; --n) *r++ = *f++; return r; }
static CSortElement* CopyBackward(CSortElement* f, CSortElement* l, CSortElement* r) { for (int n = (int)(l - f); n > 0; --n) *--r = *--l; return r; }
static CSortElement* Rotate(CSortElement* f, CSortElement* m, CSortElement* l)
{
    ++g_rotateCalls;
    CSortElement tmp[32];
    CSortElement* e = Copy(m, l, tmp);
    e = Copy(f, m, e);
    Copy(tmp, e, f);
    return f + (l - m);
}
static CSortElement* RotateAdaptive(CSortElement* first, CSortElement* middle, CSortElement* last, int len1, int len2,
                                    CSortElement* buffer, int buffer_size)
{
    CSortElement* buffer_end;
    if (len1 > len2 && len2 <= buffer_size) {
        buffer_end = Copy(middle, last, buffer);
        CopyBackward(first, middle, last);
        return Copy(buffer, buffer_end, first);
    } else if (len1 <= buffer_size) {
        buffer_end = Copy(first, middle, buffer);
        Copy(middle, last, first);
        return CopyBackward(buffer, buffer_end, last);
    } else
        return Rotate(first, middle, last);
}

static int Check(int len1, int len2, int bufSize, int wantRotate)
{
    CSortElement a[16], buf[16];
    for (int i = 0; i < len1 + len2; ++i) { a[i].SortValue = i; a[i].Tag = 100 + i; }
    g_rotateCalls = 0;
    CSortElement* r = RotateAdaptive(a, a + len1, a + len1 + len2, len1, len2, buf, bufSize);
    if (r != a + len2 || g_rotateCalls != wantRotate) return 1;
    for (int i = 0; i < len1 + len2; ++i) if (a[i].Tag != 100 + (i + len1) % (len1 + len2)) return 1;
    return 0;
}

int main()
{
    int fail = 0;
    fail |= Check(5, 2, 3, 0);    // len1 > len2, len2 fits: buffer holds the right run
    fail |= Check(2, 5, 3, 0);    // len1 fits: buffer holds the left run
    fail |= Check(6, 5, 3, 1);    // neither fits: in-place rotate
    fail |= Check(4, 4, 4, 0);
    if (fail) { std::printf("00b85cd0_TEST FAIL\n"); return 1; }
    std::printf("00b85cd0_TEST PASS\n");
    return 0;
}