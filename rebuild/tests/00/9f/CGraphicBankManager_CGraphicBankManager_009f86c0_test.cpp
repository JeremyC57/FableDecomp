#include <cstdio>
// Host model of the CGraphicBankManager destructor body: reset every entry's counted bank pointer (destroying banks
// whose last reference goes away).
struct Bank { bool destroyed; };
struct Block { long refs; Bank* obj; };
struct CP {
    Bank* Data; Block* Info;
    void Reset() { if (Info) { --Info->refs; if (Info->refs == 0) Info->obj->destroyed = true; } Info = 0; Data = 0; }
};
struct Entry { CP bank; };
int main()
{
    int fail = 0;
    Bank a = { false }, b = { false };
    Block ba = { 1, &a }, bb = { 3, &b };
    Entry e[3];
    e[0].bank.Data = &a; e[0].bank.Info = &ba;
    e[1].bank.Data = &b; e[1].bank.Info = &bb;
    e[2].bank.Data = 0; e[2].bank.Info = 0;
    for (int i = 0; i < 3; ++i) e[i].bank.Reset();
    if (!a.destroyed || b.destroyed || bb.refs != 2) fail = 1;
    for (int i = 0; i < 3; ++i) if (e[i].bank.Data || e[i].bank.Info) fail = 1;
    if (fail) { std::printf("009f86c0_TEST FAIL\n"); return 1; }
    std::printf("009f86c0_TEST PASS\n");
    return 0;
}