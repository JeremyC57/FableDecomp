#include <cstdio>
// Behaviour test for CTextureManager::AllocateTexture (0x00A69B70): a copy of the logic over a model manager.
static int g_deleted = 0;
struct Pointee { virtual ~Pointee() {} virtual void Delete() const { ++g_deleted; } void AddRef() const { ++RefCount; }
                 long Release() const { return --RefCount; } mutable unsigned long RefCount; };
template <class T> struct CountedPointer {
    Pointee* Object;
    CountedPointer() : Object(0) {}
    void Release() { Pointee* o = Object; if (o) { if (o->Release() == 0) o->Delete(); Object = 0; } }
    void Reset(T* object) { Release(); Object = object; }                 // adopting Reset
    T* operator->() const { return static_cast<T*>(Object); }
};
struct TexData : Pointee { void* Owner; bool Create; void SetOwner(void* o) { Owner = o; }
                        bool CreateTexture(unsigned long, unsigned long, long, int) { return Create; } };
struct Texture { CountedPointer<TexData> Data; };
struct Pool { TexData* Free; TexData* Allocate() { TexData* d = Free; Free = 0; return d; } };
struct Manager {
    Pool PreallocPools[2]; long FailedAllocations[2];
    static unsigned long CalculateTextureSize(unsigned long w, unsigned long h, bool, int) { return w * h; }
    long GetPoolIndex(unsigned long size) { return size <= 16 ? 0 : size <= 64 ? 1 : -1; }
    bool AllocateTexture(Texture& texture, unsigned long width, unsigned long height, long mip_levels, int format, void* owner)
    {
        unsigned long size = CalculateTextureSize(width, height, true, format);
        long pool_index = GetPoolIndex(size);
        if (pool_index == -1)
            return false;
        TexData* texture_data = PreallocPools[pool_index].Allocate();
        if (texture_data && texture_data->CreateTexture(width, height, mip_levels, format)) {
            texture_data->SetOwner(owner);
            texture.Data.Reset(texture_data);
            texture.Data->AddRef();
            return true;
        }
        ++FailedAllocations[pool_index];
        return false;
    }
};
int main()
{
    TexData a, b, c; a.RefCount = 0; b.RefCount = 0; c.RefCount = 0; a.Create = true; b.Create = true; c.Create = false;
    Manager m; m.FailedAllocations[0] = m.FailedAllocations[1] = 0; m.PreallocPools[0].Free = &a; m.PreallocPools[1].Free = &b;
    Texture t; int owner = 0;
    bool ok = true;
    ok = ok && !m.AllocateTexture(t, 16, 16, 1, 0, &owner) && m.FailedAllocations[0] == 0 && m.FailedAllocations[1] == 0;   // too big
    ok = ok && m.AllocateTexture(t, 4, 4, 1, 0, &owner) && t.Data.Object == &a && a.RefCount == 1 && a.Owner == &owner;
    ok = ok && !m.AllocateTexture(t, 2, 2, 1, 0, &owner) && m.FailedAllocations[0] == 1;                               // pool 0 empty
    ok = ok && m.AllocateTexture(t, 8, 8, 1, 0, 0) && t.Data.Object == &b && b.RefCount == 1 && a.RefCount == 0 && g_deleted == 1;
    m.PreallocPools[1].Free = &c;
    ok = ok && !m.AllocateTexture(t, 8, 8, 1, 0, 0) && m.FailedAllocations[1] == 1 && t.Data.Object == &b;              // create fails
    if (!ok) { std::printf("00a69b70_TEST FAIL\n"); return 1; }
    std::printf("00a69b70_TEST PASS\n");
    return 0;
}