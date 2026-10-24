// behaviour test for 0x00AEC740 _STL::list<CCountedPointer<CParticleEmitter> >::push_front: link a new node holding a counted copy at the front
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
int main()
{

    {
        SList l;
        CEmitterPtr a(1), b(2);
        l.push_front(a);
        l.push_front(b);
        if (l.Size() != 2 || ((SNode*)l.Node->Next)->Value.Data->Id != 2 || ((SNode*)l.Node->Prev)->Value.Data->Id != 1) { std::printf("00aec740_TEST FAIL\n"); return 1; }
        if (a.Block->RefCount != 2 || b.Block->RefCount != 2) { std::printf("00aec740_TEST FAIL\n"); return 1; }
        if (l.Node->Next->Prev != l.Node || l.Node->Prev->Next != l.Node || l.Node->Next->Next != l.Node->Prev) { std::printf("00aec740_TEST FAIL\n"); return 1; }
    }
    if (gLive != 0 || gDeleted != 2) { std::printf("00aec740_TEST FAIL\n"); return 1; }
    std::printf("00aec740_TEST PASS\n");
    return 0;
}