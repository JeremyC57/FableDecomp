#include <cstdio>
#include <new>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
typedef unsigned long long u64;
#else
typedef unsigned __int64 u64;
#endif

static int g_fail = 0;
static void Check(bool ok) { if (!ok) g_fail = 1; }

class C3DVector { public: float X, Y, Z; };
class C3DVertex2 { public: C3DVector Position; C3DVector Normal; float U; float V; };   // 0x20 bytes
struct SVertexVector { C3DVertex2* start; C3DVertex2* finish; C3DVertex2* eos; };
// the row: vector<C3DVertex2>::erase(first, last) -- copy the tail down (trivial destructors), return first
static C3DVertex2* Erase(SVertexVector& v, C3DVertex2* first, C3DVertex2* last)
{
    C3DVertex2* i = first;
    for (int n = (int)(v.finish - last); n > 0; --n, ++last, ++i)
        *i = *last;
    v.finish = i;
    return first;
}
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    Check(sizeof(C3DVertex2) == (unsigned int)0x20);
    C3DVertex2 buf[5];
    for (int k = 0; k < 5; ++k) {
        buf[k].Position.X = (float)k; buf[k].Position.Y = 0.0f; buf[k].Position.Z = 1.0f;
        buf[k].Normal.X = 0.0f; buf[k].Normal.Y = 1.0f; buf[k].Normal.Z = 0.0f;
        buf[k].U = 0.25f * (float)k; buf[k].V = 1.0f - 0.25f * (float)k;
    }
    SVertexVector v = { buf, buf + 5, buf + 5 };
    C3DVertex2* r = Erase(v, buf + 1, buf + 3);
    Check(r == buf + 1 && v.finish == buf + 3);
    Check(Near(buf[0].Position.X, 0.0f) && Near(buf[1].Position.X, 3.0f) && Near(buf[2].Position.X, 4.0f));
    Check(Near(buf[1].U, 0.75f) && Near(buf[2].V, 0.0f));
    r = Erase(v, v.finish, v.finish);
    Check(r == buf + 3 && v.finish == buf + 3);
    r = Erase(v, buf, v.finish);
    Check(r == buf && v.finish == buf);

    if (g_fail) {
        std::printf("00a96a90_TEST FAIL\n");
        return 1;
    }
    std::printf("00a96a90_TEST PASS\n");
    return 0;
}