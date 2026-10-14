#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SE { unsigned char b[16]; }; struct CLeaf { unsigned char _pad[0x18]; char nn; unsigned char _p[7]; SE* first; SE* m24; SE* last; unsigned long Usage(long& nodes, long& a, long& leaves, long& b) const { ++nodes; ++leaves; return ((long)nn + (last - first) * 4 + 12) * 4; } };

int main() {
    CLeaf* n = new CLeaf; SE e[3]; n->nn = 5; n->first = e; n->last = e + 3; long a = 1, b = 2, c = 3, d = 4; if (n->Usage(a, b, c, d) != (5 + 12 + 12) * 4 || a != 2 || b != 2 || c != 4 || d != 4) { std::printf("00a76550_TEST FAIL\n"); return 1; } delete n;
    std::printf("00a76550_TEST PASS\n"); return 0;
}