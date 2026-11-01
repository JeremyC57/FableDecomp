// Test for 0x00A69E20 CManagedTexture::~CManagedTexture: Release() from the manager when the data's reference count is 2,
// then the counted reference is dropped (Delete at zero) and the pointer cleared.
#include <cstdio>
static int g_deletes = 0, g_releases = 0;
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() const { ++g_deletes; }
    long Release() const { return --IVRefCount; }
    long GetRefCount() const { return IVRefCount; }
    mutable unsigned long IVRefCount;
};
template <class T> class CIVCountedPointer { public:
    ~CIVCountedPointer() { Release(); }
    unsigned long GetRefCount() const { return Object ? Object->GetRefCount() : 0; }
    void Release() { CIVCountedPointeeBase* object = Object; if (object) { if (object->Release() == 0) object->Delete(); Object = 0; } }
    CIVCountedPointeeBase* Object;
};
class CManagedTextureData;
class CManagedTexture { public:
    virtual ~CManagedTexture() { if (Data.GetRefCount() == 2) Release(); }
    bool Release() { ++g_releases; Data.Object->Release(); return true; }   // the manager drops its own reference
    CIVCountedPointer<CManagedTextureData> Data;
};
int main()
{
    CIVCountedPointeeBase d; d.IVRefCount = 2;
    { CManagedTexture t; t.Data.Object = &d; }
    if (g_releases != 1 || g_deletes != 1 || d.IVRefCount != 0) { std::printf("00a69e20_TEST FAIL\n"); return 1; }
    d.IVRefCount = 3;
    { CManagedTexture t; t.Data.Object = &d; }
    if (g_releases != 1 || g_deletes != 1 || d.IVRefCount != 2) { std::printf("00a69e20_TEST FAIL\n"); return 1; }
    { CManagedTexture t; t.Data.Object = 0; }
    if (g_releases != 1 || g_deletes != 1) { std::printf("00a69e20_TEST FAIL\n"); return 1; }
    std::printf("00a69e20_TEST PASS\n");
    return 0;
}