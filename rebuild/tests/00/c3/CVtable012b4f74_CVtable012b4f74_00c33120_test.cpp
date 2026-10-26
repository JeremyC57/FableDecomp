#include <cstdio>
// CVtable_012b4f74::~CVtable_012b4f74(): Close() on the object, then the base destructor
static int g_close = 0, g_base = 0, g_order = 0;
static void* g_self = 0;
struct __declspec(novtable) CVtable_012b5860 {
    virtual ~CVtable_012b5860();
};
class __declspec(novtable) CVtable_012b4f74 : public CVtable_012b5860 {
public:
    virtual ~CVtable_012b4f74();
    virtual void Close();
};
CVtable_012b5860::~CVtable_012b5860() { ++g_base; if (g_close == 1) g_order = 1; }
void CVtable_012b4f74::Close() { ++g_close; g_self = this; }
int main()
{
    void* buf[16] = {0};
    CVtable_012b4f74* p = (CVtable_012b4f74*)buf;
    p->CVtable_012b4f74::~CVtable_012b4f74();
    if (g_close != 1 || g_base != 1 || !g_order || g_self != (void*)buf || buf[0] == 0) { std::printf("00c33120_TEST FAIL\n"); return 1; }
    std::printf("00c33120_TEST PASS\n");
    return 0;
}
