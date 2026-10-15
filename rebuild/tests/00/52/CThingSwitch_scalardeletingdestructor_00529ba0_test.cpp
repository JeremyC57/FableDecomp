#include <cstdio>
#include <cstdlib>
static int g_strings = 0, g_bases = 0, g_frees = 0, g_order = 0, g_baseOrder = 0;
struct CCharString {
    void* m_pData;
    CCharString() : m_pData(0) {}
    ~CCharString() { ++g_strings; ++g_order; }
};
struct CThingGameObject {
    CThingGameObject() {}
    virtual ~CThingGameObject() { ++g_bases; g_baseOrder = ++g_order; }
    static void* operator new(size_t size) { return std::malloc(size); }
    static void operator delete(void* p) { ++g_frees; std::free(p); }
};
struct CThingSwitch : CThingGameObject {
    float         TriggerRadius;
    CCharString   EnvironmentDef;
    float         TimeToChangeEnvironmentDef;
    bool          Triggered;
    CCharString   TriggerableObjectScriptName;
    long          TimesTriggered;
};
CThingSwitch* NewThingSwitch()
{
    return new CThingSwitch;
}
int main()
{
    CThingGameObject* p = NewThingSwitch();
    delete p;
    if (g_strings != 2 || g_bases != 1 || g_frees != 1 || g_baseOrder != 3) { std::printf("00529ba0_TEST FAIL\n"); return 1; }
    std::printf("00529ba0_TEST PASS\n");
    return 0;
}