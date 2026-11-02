// Behaviour test for 0x00B4ABB0 CEnginePrimitiveRenderer2D::AddPrimitiveManager:
// push_back of a counted pointer (in-place copy bumps the count; a full vector grows), then register the raw manager.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
struct Manager { int Id; };
struct PointerInfo { long RefCount; };
struct CountedPointer {
    Manager* Data; PointerInfo* Info;
    CountedPointer(Manager* d, PointerInfo* i) : Data(d), Info(i) { if (Info) ++Info->RefCount; }
    CountedPointer(const CountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
};
static Manager* g_registered[8];
static int g_registeredCount = 0;
struct TypeInfo { void RegisterPrimitiveManager(Manager* m) { g_registered[g_registeredCount++] = m; } };
static TypeInfo g_typeInfo;
static TypeInfo* g_pTypeInfo = &g_typeInfo;
struct ManagerVector {
    CountedPointer* Start; CountedPointer* Finish; CountedPointer* EndOfStorage;
    void InsertOverflow(CountedPointer* pos, const CountedPointer& x)
    {
        unsigned int old = (unsigned int)(Finish - Start), len = old ? 2 * old : 1;
        CountedPointer* mem = (CountedPointer*)std::malloc(len * sizeof(CountedPointer));
        if (old) std::memcpy(mem, Start, old * sizeof(CountedPointer));
        new (mem + (pos - Start)) CountedPointer(x);
        std::free(Start);
        Start = mem; Finish = mem + old + 1; EndOfStorage = mem + len;
    }
    void push_back(const CountedPointer& x)
    {
        if (Finish != EndOfStorage) { if (Finish) new (Finish) CountedPointer(x); ++Finish; }
        else InsertOverflow(Finish, x);
    }
};
struct Renderer2D {
    ManagerVector PrimitiveManagers;
    void AddPrimitiveManager(const CountedPointer& manager)
    {
        PrimitiveManagers.push_back(manager);
        g_pTypeInfo->RegisterPrimitiveManager(manager.Data);
    }
};
int main()
{
    Manager m1 = { 1 }, m2 = { 2 }, m3 = { 3 };
    PointerInfo i1 = { 0 }, i2 = { 0 }, i3 = { 0 };
    Renderer2D r; r.PrimitiveManagers.Start = r.PrimitiveManagers.Finish = r.PrimitiveManagers.EndOfStorage = 0;
    CountedPointer p1(&m1, &i1), p2(&m2, &i2), p3(&m3, &i3);
    r.AddPrimitiveManager(p1);
    r.AddPrimitiveManager(p2);
    r.AddPrimitiveManager(p3);
    if (r.PrimitiveManagers.Finish - r.PrimitiveManagers.Start != 3) { std::printf("00b4abb0_TEST FAIL\n"); return 1; }
    if (i1.RefCount != 2 || i2.RefCount != 2 || i3.RefCount != 2) { std::printf("00b4abb0_TEST FAIL\n"); return 1; }
    if (r.PrimitiveManagers.Start[2].Data != &m3 || g_registeredCount != 3 || g_registered[0] != &m1 || g_registered[2] != &m3) { std::printf("00b4abb0_TEST FAIL\n"); return 1; }
    std::free(r.PrimitiveManagers.Start);
    std::printf("00b4abb0_TEST PASS\n");
    return 0;
}