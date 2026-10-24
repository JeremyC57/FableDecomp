// behaviour test for 0x00AEBA90 NParticleEngine::CParticleEmitterManager::CParticleEmitterManager(): emitter list of 0x80, Perlin noise seeded 0, 32 visible emitters, none counted, not cleared, decals enabled, no engine
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
static unsigned long gPerlinSeed = 99;
struct SPerlin { unsigned long Seed; SPerlin(unsigned long s) : Seed(s) { gPerlinSeed = s; } };
struct SEmitterList { SList Active; long MaxActiveEmitters; SEmitterList(long n) : MaxActiveEmitters(n) { } };
struct SManager {
    SEmitterList ActiveEmitters; SPerlin PerlinNoise; long MaxVisibleEmitters; long VisibleEmitterCount;
    bool Cleared : 1; bool EnableDecals : 1; void* PEngine;
    SManager() : ActiveEmitters(0x80), PerlinNoise(0), MaxVisibleEmitters(32), VisibleEmitterCount(0), PEngine(0) { Cleared = false; EnableDecals = true; }
};
int main()
{

    {
        SManager* m = new SManager;
        if (m->ActiveEmitters.MaxActiveEmitters != 0x80 || m->ActiveEmitters.Active.Size() != 0) { std::printf("00aeba90_TEST FAIL\n"); return 1; }
        if (gPerlinSeed != 0 || m->PerlinNoise.Seed != 0) { std::printf("00aeba90_TEST FAIL\n"); return 1; }
        if (m->MaxVisibleEmitters != 32 || m->VisibleEmitterCount != 0 || m->Cleared || !m->EnableDecals || m->PEngine != 0) { std::printf("00aeba90_TEST FAIL\n"); return 1; }
        delete m;
    }
    if (gLive != 0) { std::printf("00aeba90_TEST FAIL\n"); return 1; }
    std::printf("00aeba90_TEST PASS\n");
    return 0;
}