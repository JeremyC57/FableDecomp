#include <cstdio>

static int g_dtor = 0;
static int g_free = 0;

// NF460: the row now calls NParticleEngine::CParticle::~CParticle (0x00AF7B60) and CParticle::operator delete (0x00AF8650); the test
//   defines both (counted).
namespace NParticleEngine {
class CParticle { public:
    int marker;
    virtual ~CParticle();
    static void __fastcall operator delete(void* object);
};
}
using NParticleEngine::CParticle;

CParticle::~CParticle() { g_dtor++; }
void __fastcall CParticle::operator delete(void*) { g_free++; }

void* __fastcall CParticle_scalar_deleting_destructor(
    CParticle* particle, int edx, unsigned int flags);

int main()
{
    CParticle a;
    a.marker = 0x1234;

    // flags bit0 clear: dtor only, no free
    g_dtor = g_free = 0;
    void* r0 = CParticle_scalar_deleting_destructor(&a, 0, 0);
    bool ok0 = (r0 == &a) && (g_dtor == 1) && (g_free == 0);

    // flags bit0 set: dtor + free
    g_dtor = g_free = 0;
    void* r1 = CParticle_scalar_deleting_destructor(&a, 0, 1);
    bool ok1 = (r1 == &a) && (g_dtor == 1) && (g_free == 1);

    // even flags (bit0 clear) => no free
    g_dtor = g_free = 0;
    void* r2 = CParticle_scalar_deleting_destructor(&a, 0, 2);
    bool ok2 = (r2 == &a) && (g_dtor == 1) && (g_free == 0);

    if (ok0 && ok1 && ok2) {
        printf("VDD_OK\n");
        return 0;
    }
    printf("VDD_FAIL d=%d f=%d\n", g_dtor, g_free);
    return 1;
}