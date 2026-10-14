#include <cstdio>
static int g_copies = 0;
struct Base { virtual ~Base() {} };
struct Mid : Base { unsigned char body[0xb0]; Mid() {} Mid(const Mid& o) { ++g_copies; body[3] = o.body[3]; } };
struct Act : Mid { Act* Clone() { return new Act(*this); } };
int main()
{
    Act a; a.body[3] = 5; Act* p = a.Clone(); Act* q = p;
    bool ok = q && q != &a && g_copies == 1 && q->body[3] == 5; delete p;
    if (!ok) { std::printf("009680d0_TEST FAIL\n"); return 1; }
    std::printf("009680d0_TEST PASS\n");
    return 0;
}
