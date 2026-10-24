#include <cstdio>

struct SElem { unsigned char _body[24]; };
struct STag { STag() { } };

static SElem* CopyBackward(SElem* first, SElem* last, SElem* result, STag, long*)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

static void Fill(SElem& e, int v) { for (int i = 0; i < 24; ++i) e._body[i] = (unsigned char)(v * 16 + i); }
static bool Is(const SElem& e, int v) { for (int i = 0; i < 24; ++i) if (e._body[i] != (unsigned char)(v * 16 + i)) return false; return true; }

int main()
{
    STag tag;
    // backward: shift [0,3) up by one inside the same array (overlap-safe direction)
    SElem buf[5];
    for (int i = 0; i < 5; ++i) Fill(buf[i], i + 1);
    SElem* r = CopyBackward(buf, buf + 3, buf + 4, tag, 0);
    bool ok = r == buf + 1;
    if (!Is(buf[0], 1) || !Is(buf[1], 1) || !Is(buf[2], 2) || !Is(buf[3], 3) || !Is(buf[4], 5)) ok = false;
    if (CopyBackward(buf, buf, buf + 2, tag, 0) != buf + 2) ok = false;
    if (!ok) { std::printf("00819670_TEST FAIL\n"); return 1; }
    std::printf("00819670_TEST PASS\n");
    return 0;
}