// Behaviour test for 0x00882750 NScreenEffect::CScreenEffectPriorityStack::CScreenFilterProcess scalar deleting destructor
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
struct CScreenFilterProcess : ICustomProcessInterface {
    CScreenEffectPriorityStack* TheStack;
    explicit CScreenFilterProcess(CScreenEffectPriorityStack* stack) : TheStack(stack) {}
    virtual ~CScreenFilterProcess() { TheStack = 0; }
    void* operator new(size_t size) { return ::operator new(size); }
    void operator delete(void* p) { ++g_freed; g_lastFreed = p; ::operator delete(p); }
};
int main()
{
    CScreenEffectPriorityStack stack;
    int ok = 1;
    {
        unsigned char storage[sizeof(CScreenFilterProcess)];
        CScreenFilterProcess* inPlace = ::new (storage) CScreenFilterProcess(&stack);
        ok = inPlace->TheStack == &stack;
        inPlace->~CScreenFilterProcess();                           // the flags==0 path: destroy without freeing
        ok = ok && g_baseDtors == 1 && g_freed == 0;
    }
    CScreenFilterProcess* p = new CScreenFilterProcess(&stack);
    ICustomProcessInterface* base = p;
    delete base;                                   // virtual: the deleting destructor with bit 0 set
    ok = ok && g_baseDtors == 2 && g_freed == 1 && g_lastFreed == (void*)p;
    if (!ok) { std::printf("00882750_TEST FAIL\n"); return 1; }
    std::printf("00882750_TEST PASS\n");
    return 0;
}