#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags) { (void)flags; }
    virtual void Delete() { }
    unsigned long IVRefCount;
    CIVCountedPointeeBase() : IVRefCount(1) { }
    void AddRef() { ++IVRefCount; }
};
struct C3DAnimationStateBase : CIVCountedPointeeBase { };
template <class T> struct CIVCountedPointer {
    T* m_pObject;
    CIVCountedPointer(T* p) : m_pObject(p) { }
    CIVCountedPointer(const CIVCountedPointer& o) : m_pObject(o.m_pObject) { if (m_pObject) m_pObject->AddRef(); }
};
struct SCountBlock { long m_references; void* m_pfnDestroy; void* m_pObject; };
template <class T> struct CCountedPointer {
    T* m_pObject; SCountBlock* m_pCount;
    CCountedPointer(T* p, SCountBlock* b) : m_pObject(p), m_pCount(b) { }
    CCountedPointer(const CCountedPointer& o) : m_pObject(o.m_pObject), m_pCount(o.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
};
struct C3DSkeletalMorph { int m_unused; };
static int g_arrayCopies = 0;
template <class T> struct CArray {
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    CArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    CArray(const CArray& o) : m_pFirst(o.m_pFirst), m_pLast(o.m_pLast), m_pEnd(o.m_pEnd) { ++g_arrayCopies; }
};
struct SAlpha { float m_alpha; };
struct CSharedData : CIVCountedPointeeBase {
    CIVCountedPointer<C3DAnimationStateBase> BaseAnimation;
    CArray<SAlpha>                          CombinationBlends;
    CCountedPointer<const C3DSkeletalMorph> SkeletalMorph;
    CSharedData(C3DAnimationStateBase* base, const C3DSkeletalMorph* morph, SCountBlock* block) : BaseAnimation(base), SkeletalMorph(morph, block) { }
    CSharedData(const CSharedData& other)
        : CIVCountedPointeeBase(other), BaseAnimation(other.BaseAnimation), CombinationBlends(other.CombinationBlends), SkeletalMorph(other.SkeletalMorph) { }
};
int main()
{
    C3DAnimationStateBase anim; C3DSkeletalMorph morph; SCountBlock block; block.m_references = 1;
    CSharedData a(&anim, &morph, &block); a.IVRefCount = 5;
    SAlpha alphas[2]; a.CombinationBlends.m_pFirst = alphas; a.CombinationBlends.m_pLast = alphas + 2;
    CSharedData b(a);
    if (b.IVRefCount != 5 || b.BaseAnimation.m_pObject != &anim || anim.IVRefCount != 2 || g_arrayCopies != 1 || b.CombinationBlends.m_pLast != alphas + 2
        || b.SkeletalMorph.m_pObject != &morph || b.SkeletalMorph.m_pCount != &block || block.m_references != 2) { std::printf("00a3d330_TEST FAIL\n"); return 1; }
    CSharedData n(0, 0, 0); CSharedData m(n);
    if (m.BaseAnimation.m_pObject != 0 || m.SkeletalMorph.m_pCount != 0 || g_arrayCopies != 2) { std::printf("00a3d330_TEST FAIL\n"); return 1; }
    std::printf("00a3d330_TEST PASS\n");
    return 0;
}
