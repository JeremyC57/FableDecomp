// Behaviour test for 0x00AD76E0 __uninitialized_fill_n of COptimisedSubMesh (two inline STLport vector copy constructors per copy).
#include <cstdio>
#include <cstdlib>
#include <new>

class CCPPointerInfo { public: unsigned int RefCount; void (*DeleteFunc)(void*); void* Data; };
struct SCounted { void* Data; CCPPointerInfo* Info; };
struct SBone { const char* BoneName; long Parent; bool Remapped; long NewIndex; };

template <class T> struct SVector {
    T* Start; T* Finish; T* End;
    unsigned int size() const { return (unsigned int)(Finish - Start); }
};

static int g_boneCopies = 0;
// stand-in for the out-of-line bone copy 0x00AD1B20
static SBone* CopyBones(const SBone* first, const SBone* last, SBone* result)
{
    ++g_boneCopies;
    for (; first != last; ++first, ++result) new (result) SBone(*first);
    return result;
}

struct SSubMesh { SVector<SCounted> Primitives; SVector<SBone> Bones; };

static void CopySubMesh(SSubMesh* cur, const SSubMesh& x)
{
    unsigned int n = x.Primitives.size();
    cur->Primitives.Start = cur->Primitives.Finish = cur->Primitives.End = 0;
    SCounted* p = n ? (SCounted*)std::malloc(n * sizeof(SCounted)) : 0;
    cur->Primitives.Start = p; cur->Primitives.Finish = p; cur->Primitives.End = p + n;
    for (const SCounted* s = x.Primitives.Start; s != x.Primitives.Finish; ++s, ++p)
        if (p) { p->Data = s->Data; p->Info = s->Info; if (p->Info) ++p->Info->RefCount; }
    cur->Primitives.Finish = p;
    unsigned int m = x.Bones.size();
    cur->Bones.Start = cur->Bones.Finish = cur->Bones.End = 0;
    SBone* b = m ? (SBone*)std::malloc(m * sizeof(SBone)) : 0;
    cur->Bones.Start = b; cur->Bones.Finish = b; cur->Bones.End = b + m;
    cur->Bones.Finish = CopyBones(x.Bones.Start, x.Bones.Finish, cur->Bones.Start);
}
// reconstructed body
static SSubMesh* UninitializedFillN(SSubMesh* first, unsigned int n, const SSubMesh& x)
{
    SSubMesh* cur = first;
    for (; n > 0; --n, ++cur)
        if (cur != 0) CopySubMesh(cur, x);
    return cur;
}

int main()
{
    CCPPointerInfo info = { 1, 0, 0 };
    SCounted prims[2] = { { (void*)0x10, &info }, { (void*)0x20, 0 } };
    SBone bones[3] = { { "root", -1, false, 0 }, { "spine", 0, true, 4 }, { "head", 1, false, 2 } };
    SSubMesh src;
    src.Primitives.Start = prims; src.Primitives.Finish = prims + 2; src.Primitives.End = prims + 2;
    src.Bones.Start = bones; src.Bones.Finish = bones + 3; src.Bones.End = bones + 3;
    SSubMesh dst[3];
    SSubMesh* end = UninitializedFillN(dst, 3, src);
    if (end != dst + 3 || info.RefCount != 4 || g_boneCopies != 3) { std::printf("00ad76e0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) {
        if (dst[i].Primitives.size() != 2 || dst[i].Primitives.Start[0].Data != (void*)0x10 || dst[i].Bones.size() != 3 || dst[i].Bones.Start[2].Parent != 1) { std::printf("00ad76e0_TEST FAIL\n"); return 1; }
        std::free(dst[i].Primitives.Start); std::free(dst[i].Bones.Start);
    }
    if (UninitializedFillN(dst, 0, src) != dst || g_boneCopies != 3) { std::printf("00ad76e0_TEST FAIL\n"); return 1; }
    std::printf("00ad76e0_TEST PASS\n");
    return 0;
}