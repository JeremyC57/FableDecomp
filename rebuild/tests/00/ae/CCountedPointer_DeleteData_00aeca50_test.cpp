// behaviour test for 0x00AECA50 CCountedPointer<NParticleEngine::CParticleEmitterManager>::DeleteData(void*): `delete` the manager (null-safe)
#include <cstdio>
#include <cstdlib>
#include <new>
static int gLive = 0;
static int gDeleted = 0;
static void* Alloc(unsigned int n) { if (n == 0) return 0; ++gLive; return std::malloc(n); }
static void Free(void* p) { if (p) { --gLive; std::free(p); } }
struct CCountBlock { long RefCount; void (*DeleteData)(void* data); void* Data; };
struct SEmitter { int Id; };
static void DeleteEmitter(void* p) { ++gDeleted; delete (SEmitter*)p; }
struct CEmitterPtr {                                   // CCountedPointer<CParticleEmitter> model
    SEmitter* Data; CCountBlock* Block;
    CEmitterPtr(const CEmitterPtr& o) : Data(o.Data), Block(o.Block) { if (Block) ++Block->RefCount; }
    explicit CEmitterPtr(int id) : Data(new SEmitter), Block(new CCountBlock)
    { Data->Id = id; Block->RefCount = 1; Block->DeleteData = &DeleteEmitter; Block->Data = Data; }
    void ReleaseData()
    {
        if (Block) { --Block->RefCount; if (Block->RefCount == 0) { Block->DeleteData(Block->Data); delete Block; } }
        Block = 0; Data = 0;
    }
    ~CEmitterPtr() { ReleaseData(); }
};
struct SNodeBase { SNodeBase* Next; SNodeBase* Prev; };
struct SNode : SNodeBase { CEmitterPtr Value; };
struct SList {                                         // STLport list model: one sentinel pointer
    SNode* Node;
    SList() { Node = (SNode*)Alloc(sizeof(SNode)); Node->Next = Node; Node->Prev = Node; }
    void insert(SNodeBase* pos, const CEmitterPtr& x)
    {
        SNode* tmp = (SNode*)Alloc(sizeof(SNode));
        new (&tmp->Value) CEmitterPtr(x);
        SNodeBase* p = pos->Prev;
        tmp->Next = pos; tmp->Prev = p; p->Next = tmp; pos->Prev = tmp;
    }
    void push_front(const CEmitterPtr& x) { insert(Node->Next, x); }
    void clear()
    {
        SNodeBase* cur = Node->Next;
        while (cur != Node) { SNode* tmp = (SNode*)cur; cur = cur->Next; tmp->Value.~CEmitterPtr(); Free(tmp); }
        Node->Next = Node; Node->Prev = Node;
    }
    ~SList() { clear(); Free(Node); }
    int Size() const { int n = 0; for (SNodeBase* p = Node->Next; p != Node; p = p->Next) ++n; return n; }
};
struct SEmitterList { SList Active; long MaxActiveEmitters; SEmitterList(long n) : MaxActiveEmitters(n) { } };
struct SManager {                                    // CParticleEmitterManager: the list first, trivially destructible rest
    SEmitterList ActiveEmitters; unsigned long Perlin[4]; long MaxVisibleEmitters; void* PEngine;
    SManager() : ActiveEmitters(0x80), MaxVisibleEmitters(32), PEngine(0) { }
};
static int gFreedManagers = 0;
static void* ScalarDeletingDestructor(SManager* m, unsigned int flags)  // ~SManager(); if (flags & 1) operator delete
{
    m->~SManager();
    if (flags & 1) { ++gFreedManagers; ::operator delete(m); }
    return m;
}
static void DeleteData(void* data) { SManager* m = (SManager*)data; if (m) { m->~SManager(); ++gFreedManagers; ::operator delete(m); } }
int main()
{

    {
        SManager* m = new SManager;
        m->ActiveEmitters.Active.push_front(CEmitterPtr(7));
        DeleteData(m);
        if (gLive != 0 || gDeleted != 1 || gFreedManagers != 1) { std::printf("00aeca50_TEST FAIL\n"); return 1; }
        DeleteData(0);
        if (gFreedManagers != 1) { std::printf("00aeca50_TEST FAIL\n"); return 1; }
    }
    std::printf("00aeca50_TEST PASS\n");
    return 0;
}