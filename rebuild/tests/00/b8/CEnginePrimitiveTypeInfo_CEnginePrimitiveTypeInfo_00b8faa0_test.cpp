// Behaviour test for CEnginePrimitiveTypeInfo::CEnginePrimitiveTypeInfo (0x00B8FAA0): all 47 primitive-manager slots start null.
#include <new>
#include <cstdio>
#include <cstring>
struct TypeInfo { void* PrimitiveManagers[47]; TypeInfo() { for (int i = 0; i < 47; ++i) PrimitiveManagers[i] = 0; } };
int main()
{
    unsigned char storage[sizeof(TypeInfo)]; std::memset(storage, 0xAB, sizeof(storage));
    TypeInfo* t = new (storage) TypeInfo;
    for (int i = 0; i < 47; ++i) if (t->PrimitiveManagers[i] != 0) { std::printf("00b8faa0_TEST FAIL\n"); return 1; }
    std::printf("00b8faa0_TEST PASS\n");
    return 0;
}