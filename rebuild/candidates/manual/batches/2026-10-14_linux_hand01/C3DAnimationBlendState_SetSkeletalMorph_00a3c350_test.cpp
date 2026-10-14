#include <cstdio>
#include <cstdlib>
static int g_destroyed = 0, g_copies = 0;
struct SCountBlock { long m_references; void (__fastcall* m_pfnDestroy)(void*); void* m_pObject; };
struct CCountedPointer { void* m_pObject; SCountBlock* m_pCount;
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        void* object = other.m_pObject; SCountBlock* block = other.m_pCount;
        if (m_pCount != block) {
            if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } }
            m_pObject = object; m_pCount = block; if (block) ++block->m_references;
        }
        return *this;
    } };
static void __fastcall Destroy(void* p) { ++g_destroyed; if (p != (void*)0x11) std::exit(1); }
struct CSharedData { virtual void v0() { } virtual void v1() { } unsigned long IVRefCount; void* BaseAnimation; unsigned char CombinationBlends[0xc]; CCountedPointer SkeletalMorph; };
struct C3DAnimationBlendState { virtual void v00() { } CSharedData* Data; void MakeOwnCopyOfData() { ++g_copies; }
    void SetSkeletalMorph(const CCountedPointer& m) { MakeOwnCopyOfData(); Data->SkeletalMorph = m; } };
int main()
{
    SCountBlock* oldBlock = new SCountBlock; oldBlock->m_references = 1; oldBlock->m_pfnDestroy = Destroy; oldBlock->m_pObject = (void*)0x11;
    SCountBlock newBlock; newBlock.m_references = 1; newBlock.m_pfnDestroy = Destroy; newBlock.m_pObject = (void*)0x22;
    CSharedData d; d.SkeletalMorph.m_pObject = (void*)0x11; d.SkeletalMorph.m_pCount = oldBlock;
    C3DAnimationBlendState s; s.Data = &d;
    CCountedPointer p; p.m_pObject = (void*)0x22; p.m_pCount = &newBlock;
    s.SetSkeletalMorph(p);
    if (g_copies != 1 || g_destroyed != 1 || d.SkeletalMorph.m_pObject != (void*)0x22 || d.SkeletalMorph.m_pCount != &newBlock || newBlock.m_references != 2) { std::printf("00a3c350_TEST FAIL\n"); return 1; }
    s.SetSkeletalMorph(p);   // same block: nothing changes
    if (g_copies != 2 || newBlock.m_references != 2) { std::printf("00a3c350_TEST FAIL\n"); return 1; }
    std::printf("00a3c350_TEST PASS\n"); return 0;
}
