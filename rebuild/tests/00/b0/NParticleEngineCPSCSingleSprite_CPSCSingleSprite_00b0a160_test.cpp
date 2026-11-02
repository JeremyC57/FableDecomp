// Behaviour test for 0x00B0A160 NParticleEngine::CPSCSingleSprite::~CPSCSingleSprite (compiler-generated):
// PrimIDList's block freed, then the trail list (every node and the sentinel freed), then ~CPSCSplineBase.
#include <cstdio>
#include <cstdlib>
static int g_frees = 0;
static int g_order[4];
static int g_n = 0;
static void Note(int e) { if (g_n < 4) g_order[g_n++] = e; }
static void FreeBlock(void* p) { if (p) { ++g_frees; std::free(p); } }
struct ListNodeBase { ListNodeBase* Next; ListNodeBase* Prev; };
struct TrailNode : ListNodeBase { float Position[3]; float Width; };
struct TrailList {                                   // STLport list: one sentinel pointer
    ListNodeBase* Node;
    TrailList() { Node = (ListNodeBase*)std::malloc(sizeof(TrailNode)); Node->Next = Node->Prev = Node; }
    void PushBack() { TrailNode* n = (TrailNode*)std::malloc(sizeof(TrailNode)); n->Next = Node; n->Prev = Node->Prev; Node->Prev->Next = n; Node->Prev = n; }
    ~TrailList()
    {
        Note(2);
        ListNodeBase* cur = Node->Next;
        while (cur != Node) { ListNodeBase* tmp = cur; cur = cur->Next; FreeBlock(tmp); }
        Node->Next = Node; Node->Prev = Node;
        FreeBlock(Node);
    }
};
struct LongArray {
    long* Start; long* Finish; long* EndOfStorage;
    LongArray() : Start(0), Finish(0), EndOfStorage(0) { }
    ~LongArray() { if (Start) Note(1); FreeBlock(Start); }
};
struct PSCSplineBase { virtual ~PSCSplineBase() { Note(3); } };
struct SpriteTrail { int Type, InterpolationSet, ThemeChannelDesc; TrailList Trail; float Rest[0x1d]; };
struct PSCSingleSprite : PSCSplineBase { long Members[0x1e]; SpriteTrail TrailUpdatePrimitive; LongArray PrimIDList; };
int main()
{
    PSCSingleSprite* sprite = new PSCSingleSprite;
    sprite->TrailUpdatePrimitive.Trail.PushBack();
    sprite->TrailUpdatePrimitive.Trail.PushBack();
    sprite->PrimIDList.Start = (long*)std::malloc(2 * sizeof(long));
    PSCSplineBase* base = sprite;
    delete base;
    if (g_frees != 4 || g_n != 3 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3) { std::printf("00b0a160_TEST FAIL\n"); return 1; }
    std::printf("00b0a160_TEST PASS\n");
    return 0;
}