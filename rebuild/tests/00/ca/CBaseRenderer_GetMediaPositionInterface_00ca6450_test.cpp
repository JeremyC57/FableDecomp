// Behaviour test for 0x00CA6450 CBaseRenderer::GetMediaPositionInterface (renbase.cpp): under the creation lock, forward to
// an existing pass-through; else create it on pin 0, E_OUTOFMEMORY when new fails, E_NOINTERFACE (and delete) when its
// constructor fails, otherwise retry through the virtual (which now forwards).
#include <cstdio>

typedef int HRESULT;
static const HRESULT E_OUTOFMEMORY_ = (HRESULT)0x8007000E;
static const HRESULT E_NOINTERFACE_ = (HRESULT)0x80004002;

static int g_locks = 0, g_unlocks = 0, g_deleted = 0, g_created = 0;
static bool g_newFails = false;
static HRESULT g_ctorResult = 0;

struct Lock { Lock() { ++g_locks; } ~Lock() { ++g_unlocks; } };
struct Pin { int id; };
struct PosPassThru {
    Pin* pin;
    PosPassThru(HRESULT* phr, Pin* p) : pin(p) { ++g_created; if (!p) *phr = (HRESULT)0x80004003; else if (g_ctorResult) *phr = g_ctorResult; }
    virtual ~PosPassThru() { ++g_deleted; }
    virtual HRESULT NonDelegatingQueryInterface(int riid, void** ppv) { *ppv = this; return riid; }
};

struct Renderer {
    PosPassThru* m_pPosition;
    Pin m_pin;
    Renderer() : m_pPosition(0) { m_pin.id = 1; }
    virtual Pin* GetPin(int n) { return n == 0 ? &m_pin : 0; }
    virtual HRESULT GetMediaPositionInterface(int riid, void** ppv)
    {
        Lock cObjectCreationLock;
        if (m_pPosition)
            return m_pPosition->NonDelegatingQueryInterface(riid, ppv);
        HRESULT hr = 0;
        m_pPosition = g_newFails ? 0 : new PosPassThru(&hr, GetPin(0));
        if (m_pPosition == 0)
            return E_OUTOFMEMORY_;
        if (hr < 0) {
            delete m_pPosition;
            m_pPosition = 0;
            return E_NOINTERFACE_;
        }
        return GetMediaPositionInterface(riid, ppv);
    }
};

int main()
{
    Renderer r; void* pv = 0;
    g_newFails = true;
    if (r.GetMediaPositionInterface(7, &pv) != E_OUTOFMEMORY_ || g_locks != 1 || g_unlocks != 1) { std::printf("00ca6450_TEST FAIL\n"); return 1; }
    g_newFails = false; g_ctorResult = (HRESULT)0x80004005;
    if (r.GetMediaPositionInterface(7, &pv) != E_NOINTERFACE_ || r.m_pPosition != 0 || g_deleted != 1) { std::printf("00ca6450_TEST FAIL\n"); return 1; }
    g_ctorResult = 0;
    if (r.GetMediaPositionInterface(7, &pv) != 7 || r.m_pPosition == 0 || pv != r.m_pPosition || g_created != 2) { std::printf("00ca6450_TEST FAIL\n"); return 1; }
    if (r.GetMediaPositionInterface(9, &pv) != 9 || g_created != 2 || g_locks != g_unlocks) { std::printf("00ca6450_TEST FAIL\n"); return 1; }
    delete r.m_pPosition;
    std::printf("00ca6450_TEST PASS\n");
    return 0;
}