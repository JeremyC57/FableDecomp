// Test for 0x00711520 CAnimSet::Clear: the source object must empty both vectors (finish = start, buffers kept) and
// zero CurrentAlpha / LastAlpha, leaving the other members alone.
#include <stdio.h>
#include <string.h>
class CAnimSet {
public:
    void Clear();
};
static short g_anims[4];
static float g_alphas[6];
int main()
{
    unsigned char s[0x54];
    memset(s, 0x11, sizeof(s));
    void* p;
    p = g_anims;      memcpy(s + 0x04, &p, 4);
    p = g_anims + 3;  memcpy(s + 0x08, &p, 4);
    p = g_anims + 4;  memcpy(s + 0x0c, &p, 4);
    p = g_alphas;     memcpy(s + 0x10, &p, 4);
    p = g_alphas + 4; memcpy(s + 0x14, &p, 4);
    p = g_alphas + 6; memcpy(s + 0x18, &p, 4);
    float one = 1.0f; memcpy(s + 0x2c, &one, 4); memcpy(s + 0x30, &one, 4);
    ((CAnimSet*)s)->Clear();
    void* q;
    bool ok = true;
    memcpy(&q, s + 0x08, 4); if (q != g_anims) ok = false;
    memcpy(&q, s + 0x0c, 4); if (q != g_anims + 4) ok = false;
    memcpy(&q, s + 0x14, 4); if (q != g_alphas) ok = false;
    memcpy(&q, s + 0x18, 4); if (q != g_alphas + 6) ok = false;
    float a, b; memcpy(&a, s + 0x2c, 4); memcpy(&b, s + 0x30, 4);
    if (a != 0.0f || b != 0.0f) ok = false;
    if (s[0x28] != 0x11 || s[0x34] != 0x11 || s[0x00] != 0x11) ok = false;
    if (!ok) { puts("00711520_TEST FAIL"); return 1; }
    puts("00711520_TEST PASS");
    return 0;
}
