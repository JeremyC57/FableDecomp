// Test: CPreallocTexturePool scalar deleting destructor -- releases every texture pointer (the pointee is destroyed when
// its count reaches zero), frees the vector storage, runs the base destructor, frees the object only when bit 0 is set.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_destroyed = 0, g_frees = 0, g_baseDtors = 0, g_deletes = 0;
class CManagedTextureData { public: long RefCount; void Destroy() { ++g_destroyed; } void DecRef() { if (--RefCount == 0) Destroy(); } };
struct CTexPtr { CManagedTextureData* Object; ~CTexPtr() { if (Object) { Object->DecRef(); Object = 0; } } };
class CResourceList { public: virtual ~CResourceList() { ++g_baseDtors; } };
struct CPreallocTexturePool : CResourceList {
    unsigned long Count, MaxSize; long PoolIndex; CTexPtr* Start; CTexPtr* Finish; CTexPtr* End;
    ~CPreallocTexturePool() { for (CTexPtr* p = Start; p != Finish; ++p) p->~CTexPtr(); if (Start) { ++g_frees; std::free(Start); } }
};
static CPreallocTexturePool* ScalarDeletingDestructor(CPreallocTexturePool* self, unsigned int flags)
{
    self->~CPreallocTexturePool();
    if (flags & 1) { ++g_deletes; std::free(self); }
    return self;
}
int main()
{
    CManagedTextureData a = { 1 }, b = { 2 };
    CPreallocTexturePool* p = (CPreallocTexturePool*)std::malloc(sizeof(CPreallocTexturePool)); new (p) CPreallocTexturePool;
    p->Start = (CTexPtr*)std::malloc(3 * sizeof(CTexPtr)); p->Finish = p->Start + 3; p->End = p->Finish;
    p->Start[0].Object = &a; p->Start[1].Object = &b; p->Start[2].Object = 0;
    ScalarDeletingDestructor(p, 1);
    bool ok = g_destroyed == 1 && a.RefCount == 0 && b.RefCount == 1 && g_frees == 1 && g_baseDtors == 1 && g_deletes == 1;
    CPreallocTexturePool* q = (CPreallocTexturePool*)std::malloc(sizeof(CPreallocTexturePool)); new (q) CPreallocTexturePool;
    q->Start = q->Finish = q->End = 0;
    ScalarDeletingDestructor(q, 0);
    ok = ok && g_frees == 1 && g_baseDtors == 2 && g_deletes == 1;
    std::free(q);
    std::puts(ok ? "00a6bfc0_TEST PASS" : "00a6bfc0_TEST FAIL");
    return ok ? 0 : 1;
}