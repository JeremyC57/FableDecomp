// Behaviour test for 0x008826F0 NScreenEffect::CScreenEffectPriorityStack::CRadialBlurProcess scalar deleting destructor
#include <cstdio>
#include <new>
static int g_freed = 0;
static int g_baseDtors = 0;
static void* g_lastFreed = 0;
struct CScreenEffectPriorityStack { int dummy; };
struct ICustomProcessInterface {
    virtual ~ICustomProcessInterface() { ++g_baseDtors; }
    virtual void ResetToDefault() {}
};
struct CRadialBlurProcess : ICustomProcessInterface {
    CScreenEffectPriorityStack* TheStack;
    explicit CRadialBlurProcess(CScreenEffectPriorityStack* stack) : TheStack(stack) {}
    virtual ~CRadialBlurProcess() { TheStack = 0; }
    void* operator new(size_t size) { return ::operator new(size); }
    void operator delete(void* p) { ++g_freed; g_lastFreed = p; ::operator delete(p); }
};
int main()
{
    CScreenEffectPriorityStack stack;
    int ok = 1;
    {
        unsigned char storage[sizeof(CRadialBlurProcess)];
        CRadialBlurProcess* inPlace = ::new (storage) CRadialBlurProcess(&stack);
        ok = inPlace->TheStack == &stack;
        inPlace->~CRadialBlurProcess();                           // the flags==0 path: destroy without freeing
        ok = ok && g_baseDtors == 1 && g_freed == 0;
    }
    CRadialBlurProcess* p = new CRadialBlurProcess(&stack);
    ICustomProcessInterface* base = p;
    delete base;                                   // virtual: the deleting destructor with bit 0 set
    ok = ok && g_baseDtors == 2 && g_freed == 1 && g_lastFreed == (void*)p;
    if (!ok) { std::printf("008826f0_TEST FAIL\n"); return 1; }
    std::printf("008826f0_TEST PASS\n");
    return 0;
}