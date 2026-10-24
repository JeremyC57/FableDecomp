#include <cstdio>

struct SElem { unsigned char _body[24]; };
struct STag { STag() { } };

static SElem* Copy(SElem* first, SElem* last, SElem* result, STag, long*)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

static void Fill(SElem& e, int v) { for (int i = 0; i < 24; ++i) e._body[i] = (unsigned char)(v * 16 + i); }
static bool Is(const SElem& e, int v) { for (int i = 0; i < 24; ++i) if (e._body[i] != (unsigned char)(v * 16 + i)) return false; return true; }

int main()
{
    STag tag;
    // forward: shift [1,4) down by one inside the same array (overlap-safe direction)
    SElem buf[5];
    for (int i = 0; i < 5; ++i) Fill(buf[i], i + 1);
    SElem* r = Copy(buf + 1, buf + 4, buf, tag, 0);
    bool ok = r == buf + 3;
    if (!Is(buf[0], 2) || !Is(buf[1], 3) || !Is(buf[2], 4) || !Is(buf[3], 4) || !Is(buf[4], 5)) ok = false;
    if (Copy(buf, buf, buf + 2, tag, 0) != buf + 2) ok = false;
    if (!ok) { std::printf("00819390_TEST FAIL\n"); return 1; }
    std::printf("00819390_TEST PASS\n");
    return 0;
}