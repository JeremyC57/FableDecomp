// Behaviour test for 0x00AAC4F0 CSoundEmitterLH::~CSoundEmitterLH (user destructor):
// Shutdown() first, then the active-sound list is cleared (every node freed) and its sentinel freed, then the base destructor.
#include <cstdio>
#include <cstdlib>
static int g_frees = 0;
static int g_order[8];
static int g_n = 0;
static void Note(int e) { if (g_n < 8) g_order[g_n++] = e; }
static void FreeBlock(void* p) { if (p != 0) { ++g_frees; std::free(p); } }
struct NodeBase { NodeBase* Next; NodeBase* Prev; };
struct SoundNode : NodeBase { void* Data; void* Info; };
struct SoundList {
    NodeBase* Node;
    SoundList() { Node = (NodeBase*)std::malloc(sizeof(SoundNode)); Node->Next = Node->Prev = Node; }
    void PushBack() { SoundNode* n = (SoundNode*)std::malloc(sizeof(SoundNode)); n->Next = Node; n->Prev = Node->Prev; Node->Prev->Next = n; Node->Prev = n; }
    void Clear()
    {
        Note(2);
        NodeBase* cur = Node->Next;
        while (cur != Node) { NodeBase* tmp = cur; cur = cur->Next; FreeBlock(tmp); }
        Node->Next = Node; Node->Prev = Node;
    }
    ~SoundList() { Clear(); Note(3); FreeBlock(Node); }
};
struct BaseClass { virtual ~BaseClass() { Note(4); } };
struct ASoundEmitter : BaseClass { };
struct SoundEmitterLH : ASoundEmitter {
    void* Manager; float Position[3]; SoundList Sounds; bool Active, Paused, Finished; long Number;
    int ShutdownCount;
    SoundEmitterLH() : ShutdownCount(0) { }
    void Shutdown() { ++ShutdownCount; Note(1); }
    virtual ~SoundEmitterLH() { Shutdown(); }
};
int main()
{
    SoundEmitterLH* e = new SoundEmitterLH;
    e->Sounds.PushBack();
    e->Sounds.PushBack();
    BaseClass* base = e;
    delete base;
    if (g_frees != 3 || g_n != 4 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3 || g_order[3] != 4) { std::printf("00aac4f0_TEST FAIL\n"); return 1; }
    std::printf("00aac4f0_TEST PASS\n");
    return 0;
}