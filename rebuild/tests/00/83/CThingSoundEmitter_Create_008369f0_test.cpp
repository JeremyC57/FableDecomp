// Behaviour test for CThingSoundEmitter::Create (0x008369F0): allocate, construct, activate; null on any failure.
#include <cstdio>
#include <new>
#include <stddef.h>
struct C3DVector { float x, y, z; };

static bool g_allowAlloc = true, g_constructOk = true, g_activateOk = true;
static int g_constructed = 0;
static const C3DVector* g_pos = 0;
static const void* g_attached = 0;
static bool g_cont = false;
static double g_storage[64];

class CThing {
public:
    bool DoActivationSequence() { return g_activateOk; }
    static void* operator new(size_t size, bool) { return (g_allowAlloc && size <= sizeof(g_storage)) ? g_storage : 0; }
    static void operator delete(void*, bool) {}
    int m_dummy;
};

class CThingSoundEmitter : public CThing {
public:
    CThingSoundEmitter() { ++g_constructed; }
    bool ConstructFromParams(const C3DVector& pos, const CThing* attachedThing, bool continuePlaying)
    {
        g_pos = &pos; g_attached = attachedThing; g_cont = continuePlaying;
        return g_constructOk;
    }
    static CThingSoundEmitter* Create(const C3DVector& pos, const CThing* attachedThing, bool continuePlaying)
    {
        CThingSoundEmitter* result = 0;
        CThingSoundEmitter* p = (CThingSoundEmitter*)CThing::operator new(sizeof(CThingSoundEmitter), false);
        if (p) {
            p = ::new (p) CThingSoundEmitter;
            if (p && p->ConstructFromParams(pos, attachedThing, continuePlaying) && p->DoActivationSequence())
                result = p;
        }
        return result;
    }
};

int main()
{
    C3DVector v = { 1, 2, 3 };
    CThing other;
    CThingSoundEmitter* e = CThingSoundEmitter::Create(v, &other, true);
    if (e != (void*)g_storage || g_constructed != 1 || g_pos != &v || g_attached != &other || !g_cont) { std::printf("008369f0_TEST FAIL\n"); return 1; }
    g_activateOk = false;
    if (CThingSoundEmitter::Create(v, 0, false) != 0 || g_constructed != 2) { std::printf("008369f0_TEST FAIL\n"); return 1; }
    g_activateOk = true; g_constructOk = false;
    if (CThingSoundEmitter::Create(v, 0, false) != 0) { std::printf("008369f0_TEST FAIL\n"); return 1; }
    g_constructOk = true; g_allowAlloc = false;
    if (CThingSoundEmitter::Create(v, 0, false) != 0 || g_constructed != 3) { std::printf("008369f0_TEST FAIL\n"); return 1; }
    std::printf("008369f0_TEST PASS\n");
    return 0;
}