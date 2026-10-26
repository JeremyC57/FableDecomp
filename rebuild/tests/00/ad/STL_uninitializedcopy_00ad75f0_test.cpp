// Behaviour test for 0x00AD75F0 __uninitialized_copy of COptimisedSubMesh (two inline STLport vector copy constructors).
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

// reconstructed body
static SSubMesh* UninitializedCopy(const SSubMesh* first, const SSubMesh* last, SSubMesh* result)
{
    SSubMesh* cur = result;
    for (; first != last; ++first, ++cur) {
        if (cur == 0) continue;
        unsigned int n = first->Primitives.size();
        cur->Primitives.Start = cur->Primitives.Finish = cur->Primitives.End = 0;
        SCounted* p = n ? (SCounted*)std::malloc(n * sizeof(SCounted)) : 0;
        cur->Primitives.Start = p; cur->Primitives.Finish = p; cur->Primitives.End = p + n;
        for (const SCounted* s = first->Primitives.Start; s != first->Primitives.Finish; ++s, ++p) {
            if (p) { p->Data = s->Data; p->Info = s->Info; if (p->Info) ++p->Info->RefCount; }
        }
        cur->Primitives.Finish = p;
        unsigned int m = first->Bones.size();
        cur->Bones.Start = cur->Bones.Finish = cur->Bones.End = 0;
        SBone* b = m ? (SBone*)std::malloc(m * sizeof(SBone)) : 0;
        cur->Bones.Start = b; cur->Bones.Finish = b; cur->Bones.End = b + m;
        cur->Bones.Finish = CopyBones(first->Bones.Start, first->Bones.Finish, cur->Bones.Start);
    }
    return cur;
}

int main()
{
    CCPPointerInfo info = { 1, 0, 0 };
    SCounted prims[2] = { { (void*)0x10, &info }, { (void*)0x20, 0 } };
    SBone bones[3] = { { "root", -1, false, 0 }, { "spine", 0, true, 4 }, { "head", 1, false, 2 } };
    SSubMesh src[2];
    src[0].Primitives.Start = prims; src[0].Primitives.Finish = prims + 2; src[0].Primitives.End = prims + 2;
    src[0].Bones.Start = bones; src[0].Bones.Finish = bones + 3; src[0].Bones.End = bones + 3;
    src[1].Primitives.Start = src[1].Primitives.Finish = src[1].Primitives.End = 0;
    src[1].Bones.Start = src[1].Bones.Finish = src[1].Bones.End = 0;
    SSubMesh dst[2];
    SSubMesh* end = UninitializedCopy(src, src + 2, dst);
    if (end != dst + 2 || info.RefCount != 2 || g_boneCopies != 2) { std::printf("00ad75f0_TEST FAIL\n"); return 1; }
    if (dst[0].Primitives.size() != 2 || dst[0].Primitives.Start[0].Data != (void*)0x10 || dst[0].Primitives.Start[1].Info != 0) { std::printf("00ad75f0_TEST FAIL\n"); return 1; }
    if (dst[0].Bones.size() != 3 || dst[0].Bones.Start[1].NewIndex != 4 || dst[0].Bones.End != dst[0].Bones.Start + 3) { std::printf("00ad75f0_TEST FAIL\n"); return 1; }
    if (dst[1].Primitives.Start != 0 || dst[1].Bones.Start != 0 || dst[1].Bones.Finish != 0) { std::printf("00ad75f0_TEST FAIL\n"); return 1; }
    std::free(dst[0].Primitives.Start); std::free(dst[0].Bones.Start);
    std::printf("00ad75f0_TEST PASS\n");
    return 0;
}