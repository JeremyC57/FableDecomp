// Behaviour test for 0x00BBDCB0 (see src/00bbdcb0.cpp): list::resize(n) forwarding to resize(n, T()) with a default handle.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("00bbdcb0_TEST FAIL\n"); return 1; } } while (0)

static int g_lastReleased = 0, g_destroyed = 0, g_baseCtor = 0, g_baseDtor = 0;
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase {
public:
    int RefCount;
    CEnginePersistentPrimitiveBase() : RefCount(0) { }
    virtual ~CEnginePersistentPrimitiveBase() { }
    virtual void Destroy() { ++g_destroyed; }
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle*) { ++g_lastReleased; }
    void DecRef() { --RefCount; }
    int GetRefCount() const { return RefCount; }
    void Release() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    CIVCountedPointer() : Object(0) { }
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    ~CIVCountedPointer() { if (Object) Object->Release(); Object = 0; }
};
class CBaseClass { public: CBaseClass() { ++g_baseCtor; } virtual ~CBaseClass() { ++g_baseDtor; } };
class CEnginePrimitiveHandle : public CBaseClass {
public:
    CIVCountedPointer Data;
    CEnginePrimitiveHandle() { }
    ~CEnginePrimitiveHandle() { if (Data.IsUnique()) Data->OnLastHandleReleased(this); }
};

struct CEnginePrimitiveHandleList {
    unsigned int size; unsigned int lastRequest; bool valueWasEmpty;
    void resize(unsigned int newSize, const CEnginePrimitiveHandle& value) { lastRequest = newSize; valueWasEmpty = value.Data.Object == 0; size = newSize; }
    void resize(unsigned int newSize) { resize(newSize, CEnginePrimitiveHandle()); }
};
int main()
{
    CEnginePrimitiveHandleList l; l.size = 5; l.lastRequest = 99; l.valueWasEmpty = false;
    l.resize(0);
    CHECK(l.size == 0 && l.lastRequest == 0 && l.valueWasEmpty && g_baseCtor == 1 && g_baseDtor == 1 && g_lastReleased == 0);
    l.resize(3);
    CHECK(l.size == 3 && g_baseCtor == 2 && g_baseDtor == 2);
    std::printf("00bbdcb0_TEST PASS\n");
    return 0;
}
