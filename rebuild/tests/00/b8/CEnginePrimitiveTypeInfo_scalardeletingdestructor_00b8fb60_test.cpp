// Behaviour test for CEnginePrimitiveTypeInfo's scalar deleting destructor (0x00B8FB60): the non-copyable base (+0x0C) destroyed before the engine component, freed only on flag bit 0.
#include <cstdio>
#include <new>
static int g_log[4]; static int g_n; static int g_frees;
struct Component { virtual ~Component() { g_log[g_n++] = 2; } };
struct NonCopyable { virtual ~NonCopyable() { g_log[g_n++] = 1; } };
struct TypeInfo : Component, NonCopyable { void* PrimitiveManagers[47];
    void* ScalarDelete(unsigned int flags) { this->~TypeInfo(); if (flags & 1) ++g_frees; return this; } };
int main()
{
    unsigned char storage[sizeof(TypeInfo)]; TypeInfo* t = new (storage) TypeInfo;
    g_n = 0; t->ScalarDelete(1);
    if (g_n != 2 || g_log[0] != 1 || g_log[1] != 2 || g_frees != 1) { std::printf("00b8fb60_TEST FAIL\n"); return 1; }
    t = new (storage) TypeInfo; g_n = 0; t->ScalarDelete(0);
    if (g_n != 2 || g_frees != 1) { std::printf("00b8fb60_TEST FAIL\n"); return 1; }
    std::printf("00b8fb60_TEST PASS\n");
    return 0;
}