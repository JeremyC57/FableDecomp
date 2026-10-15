// Behaviour test for 0x00B8FAD0 (see src/00b8fad0.cpp): RegisterPrimitiveManager filling the per-type manager table.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#define CHECK(c) do { if (!(c)) { std::printf("00b8fad0_TEST FAIL\n"); return 1; } } while (0)

struct CPrimitiveTypeVector {
    int* m_pFirst; int* m_pLast; int* m_pEnd;
    CPrimitiveTypeVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    ~CPrimitiveTypeVector() { if (m_pFirst) std::free(m_pFirst); }
    unsigned int size() const { return (unsigned int)(m_pLast - m_pFirst); }
    int operator[](unsigned int n) const { return *(m_pFirst + n); }
};
struct CEnginePrimitiveManager {
    int types[3]; int typeCount;
    void GetSupportedTypes(CPrimitiveTypeVector& v)
    {
        if (!typeCount) return;
        v.m_pFirst = (int*)std::malloc(sizeof(int) * typeCount); v.m_pLast = v.m_pEnd = v.m_pFirst + typeCount;
        for (int i = 0; i < typeCount; ++i) v.m_pFirst[i] = types[i];
    }
};
struct CEnginePrimitiveTypeInfo {
    CEnginePrimitiveManager* PrimitiveManagers[47];
    void RegisterPrimitiveManager(CEnginePrimitiveManager* manager)
    {
        CPrimitiveTypeVector types;
        manager->GetSupportedTypes(types);
        for (unsigned int i = 0; i < types.size(); ++i) PrimitiveManagers[types[i]] = manager;
    }
};
int main()
{
    CEnginePrimitiveTypeInfo info; for (int i = 0; i < 47; ++i) info.PrimitiveManagers[i] = 0;
    CEnginePrimitiveManager text = { { 5, 0x27, 0 }, 2 }, none = { { 0, 0, 0 }, 0 };
    info.RegisterPrimitiveManager(&text);
    info.RegisterPrimitiveManager(&none);
    CHECK(info.PrimitiveManagers[5] == &text && info.PrimitiveManagers[0x27] == &text);
    for (int i = 0; i < 47; ++i) if (i != 5 && i != 0x27) CHECK(info.PrimitiveManagers[i] == 0);
    std::printf("00b8fad0_TEST PASS\n");
    return 0;
}