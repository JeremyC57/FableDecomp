// Behaviour test for vector<CTVertexLandscapeBackground>::erase(first, last) (0x00B6CD80): forward copy of the tail, new finish, returns first.
#include <stdio.h>
#include <string.h>
struct V { unsigned short X, Y; float Z, NX, NY, NZ; unsigned char a, b, c, d; };
struct Vec {
    V* _M_start; V* _M_finish; V* _M_end;
    V* erase(V* first, V* last)
    {
        V* r = first;
        for (int n = (int)(_M_finish - last); n > 0; --n) { *r = *last; ++r; ++last; }
        _M_finish = r;
        return first;
    }
};
int main()
{
    V buf[6]; int i;
    for (i = 0; i < 6; i++) { memset(&buf[i], 0, sizeof(V)); buf[i].X = (unsigned short)i; buf[i].Z = (float)i * 2.0f; buf[i].d = (unsigned char)(i + 10); }
    Vec v; v._M_start = buf; v._M_finish = buf + 6; v._M_end = buf + 6;
    V* r = v.erase(buf + 1, buf + 3);
    if (r != buf + 1 || v._M_finish != buf + 4 || buf[1].X != 3 || buf[2].X != 4 || buf[3].X != 5 || buf[3].Z != 10.0f || buf[3].d != 15 || buf[0].X != 0)
    { printf("00b6cd80_TEST FAIL\n"); return 1; }
    r = v.erase(buf + 2, buf + 4);
    if (r != buf + 2 || v._M_finish != buf + 2) { printf("00b6cd80_TEST FAIL\n"); return 1; }
    printf("00b6cd80_TEST PASS\n"); return 0;
}