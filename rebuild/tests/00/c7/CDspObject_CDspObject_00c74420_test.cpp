#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
typedef long long __int64;
#endif
bool CreateSharedOps();                    // 0x00C74360

struct CDspOps;

struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74480 (deleting) / 0x00C74450
    CDspObject();
    static CDspOps* s_pOps;                // 0x0143CCDC
    static long s_refCount;                // 0x0143CCE0
};

CDspObject::CDspObject()
{
    if (!s_pOps)
        CreateSharedOps();
    else
        ++s_refCount;
}
static int fail() { std::printf("00c74420_TEST FAIL\n"); return 1; }
static bool nearly(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-3f; }
CDspOps* CDspObject::s_pOps = 0;
long CDspObject::s_refCount = 0;
static int g_created = 0;
static int g_fakeOps;
bool CreateSharedOps()
{
    ++g_created;
    CDspObject::s_pOps = reinterpret_cast<CDspOps*>(&g_fakeOps);
    CDspObject::s_refCount = 1;
    return true;
}
CDspObject::~CDspObject() { }
int main()
{
    CDspObject a;
    if (g_created != 1 || CDspObject::s_refCount != 1 || CDspObject::s_pOps == 0) return fail();
    CDspObject b;
    CDspObject c;
    if (g_created != 1 || CDspObject::s_refCount != 3) return fail();
    std::printf("00c74420_TEST PASS\n");
    return 0;
}