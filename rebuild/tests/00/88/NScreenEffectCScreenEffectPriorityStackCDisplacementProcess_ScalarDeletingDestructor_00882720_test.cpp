// Behaviour test for 0x00882720 NScreenEffect::CScreenEffectPriorityStack::CDisplacementProcess scalar deleting destructor
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
struct CDisplacementProcess : ICustomProcessInterface {
    CScreenEffectPriorityStack* TheStack;
    explicit CDisplacementProcess(CScreenEffectPriorityStack* stack) : TheStack(stack) {}
    virtual ~CDisplacementProcess() { TheStack = 0; }
    void* operator new(size_t size) { return ::operator new(size); }
    void operator delete(void* p) { ++g_freed; g_lastFreed = p; ::operator delete(p); }
};
int main()
{
    CScreenEffectPriorityStack stack;
    int ok = 1;
    {
        unsigned char storage[sizeof(CDisplacementProcess)];
        CDisplacementProcess* inPlace = ::new (storage) CDisplacementProcess(&stack);
        ok = inPlace->TheStack == &stack;
        inPlace->~CDisplacementProcess();                           // the flags==0 path: destroy without freeing
        ok = ok && g_baseDtors == 1 && g_freed == 0;
    }
    CDisplacementProcess* p = new CDisplacementProcess(&stack);
    ICustomProcessInterface* base = p;
    delete base;                                   // virtual: the deleting destructor with bit 0 set
    ok = ok && g_baseDtors == 2 && g_freed == 1 && g_lastFreed == (void*)p;
    if (!ok) { std::printf("00882720_TEST FAIL\n"); return 1; }
    std::printf("00882720_TEST PASS\n");
    return 0;
}