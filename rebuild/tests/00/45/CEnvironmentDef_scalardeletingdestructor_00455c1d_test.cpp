#include <cstdio>
#include <cstdlib>

static int g_dtor = 0;
static void* g_deleted = 0;

struct CSubObj { ~CSubObj(); };
class CEnvironmentDef { public:
    char pad[0x3c];
    CSubObj sub;
    ~CEnvironmentDef();
    void * scalar_deleting_destructor(unsigned int flags);
};
CSubObj::~CSubObj() {}
CEnvironmentDef::~CEnvironmentDef() { g_dtor++; }

void * CEnvironmentDef::scalar_deleting_destructor(unsigned int flags)
{
    this->sub.~CSubObj();
    this->~CEnvironmentDef();
    if (flags & 1)
        ::operator delete(this);
    return this;
}

int main()
{
    char buf1[sizeof(CEnvironmentDef)];
    CEnvironmentDef* p = (CEnvironmentDef*)buf1;
    void* r = p->scalar_deleting_destructor(0);
    if (r != p) { std::printf("FAIL ret\n"); return 1; }
    if (g_dtor != 1) { std::printf("FAIL dtor\n"); return 1; }
    CEnvironmentDef* q = (CEnvironmentDef*)::operator new(sizeof(CEnvironmentDef));
    void* qsave = q;
    void* r2 = q->scalar_deleting_destructor(1);
    if (r2 != qsave) { std::printf("FAIL ret2\n"); return 1; }
    if (g_dtor != 2) { std::printf("FAIL dtor2\n"); return 1; }
    std::printf("CEngineSubPrimitiveText_00455c1d_TEST PASS\n");
    return 0;
}