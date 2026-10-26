// behaviour test for 0x004354A0
#include <stdio.h>
static int g_log[8]; static int g_n = 0; static const void* g_removed;
class CEngineInit;
class CIEngine { public:
    virtual ~CIEngine() {}
    virtual void Validate() const {}
    virtual bool Initialise(const CEngineInit&) { return true; }
    virtual void Release() { g_log[g_n++] = 1; }
};
class CEngineManager {
public:
    void RemoveEngine(CIEngine& engine) { g_log[g_n++] = 3; g_removed = &engine; }
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get() { static T inst; g_log[g_n++] = 2; return inst; }
};
class CDisplayEngine { public: unsigned char _pad[0x2c]; CIEngine* Engine3D; };
struct CDisplayEngine_Methods : CDisplayEngine {
    void UninitEngine();
};
void CDisplayEngine_Methods::UninitEngine()
{
    Engine3D->Release();
    CTBaseSingleton<CEngineManager>::Get().RemoveEngine(*Engine3D);
}
int main()
{
    CIEngine e; CDisplayEngine_Methods d; d.Engine3D = &e; d.UninitEngine();
    int ok = g_n == 3 && g_log[0] == 1 && g_log[1] == 2 && g_log[2] == 3 && g_removed == &e;
    printf(ok ? "004354a0_TEST PASS\n" : "004354a0_TEST FAIL\n");
    return ok ? 0 : 1;
}