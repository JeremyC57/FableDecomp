// Behaviour test for CDefString::CDefString() (0x007E37C0): a new def string has no table entry (TablePos == -1).
#include <cstdio>

struct CDefStringModel
{
    long TablePos;
    CDefStringModel() : TablePos(-1) {}
};

int main()
{
    CDefStringModel a;
    if (a.TablePos != -1) { std::printf("007e37c0_TEST FAIL\n"); return 1; }
    CDefStringModel* p = new CDefStringModel;
    bool ok = (p->TablePos == -1);
    delete p;
    if (!ok) { std::printf("007e37c0_TEST FAIL\n"); return 1; }
    std::printf("007e37c0_TEST PASS\n");
    return 0;
}
