// Behaviour test for CClassFactory::AllocateClass (0x00A371C0): the type number comes from the name lookup; the type's
// allocation function is called when present, otherwise 0 is returned.
#include <cstdio>
#include <cstring>
struct Class { int type; };
static Class g_made;
static Class* MakeClass() { g_made.type = 7; return &g_made; }
struct ClassTypeInfo { void* TypeList; unsigned int NoAllocated; bool Used; bool Local; Class* (*AllocFunc)(); const char* Name; };
struct ClassFactory {
    ClassTypeInfo* start;
    long GetClassNumberFromName(const char* name) const { return std::strcmp(name, "Thing") == 0 ? 1 : 0; }
    Class* AllocateClass(const char* name)
    {
        long type = GetClassNumberFromName(name);
        if (start[type].AllocFunc)
            return start[type].AllocFunc();
        return 0;
    }
};
int main()
{
    ClassTypeInfo infos[2];
    std::memset(infos, 0, sizeof(infos));
    infos[1].AllocFunc = MakeClass;
    ClassFactory f; f.start = infos;
    Class* a = f.AllocateClass("Thing");
    Class* b = f.AllocateClass("Other");
    if (a != &g_made || a->type != 7 || b != 0) { std::printf("00a371c0_TEST FAIL\n"); return 1; }
    std::printf("00a371c0_TEST PASS\n");
    return 0;
}