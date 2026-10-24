// Behaviour test for 0x00AD4CA0 _Rb_tree::erase(iterator) of multimap<float, CCountedPointer<CVertexPairContraction>>.
#include <cstdio>
#include <cstdlib>

struct SCountInfo { long RefCount; void (*DeleteFunc)(void*); void* Data; };
struct SCounted {
    void* Data; SCountInfo* Info;
    ~SCounted()
    {
        if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
        Data = 0; Info = 0;
    }
};
struct SNodeBase { bool Color; SNodeBase* Parent; SNodeBase* Left; SNodeBase* Right; };
struct SNode : SNodeBase { float Key; SCounted Value; };

static int g_deleted = 0; static void* g_lastObject = 0;
static void DeleteObject(void* p) { ++g_deleted; g_lastObject = p; }

static SNodeBase** g_args[3];
// stand-in for _Rebalance_for_erase: records the link addresses, unlinks nothing, returns z
static SNodeBase* RebalanceForErase(SNodeBase* z, SNodeBase*& root, SNodeBase*& leftmost, SNodeBase*& rightmost)
{
    g_args[0] = &root; g_args[1] = &leftmost; g_args[2] = &rightmost;
    return z;
}

struct STree {
    SNodeBase* Header; unsigned int Count;
    void erase(SNodeBase* position)
    {
        SNode* y = (SNode*)RebalanceForErase(position, Header->Parent, Header->Left, Header->Right);
        y->Value.~SCounted();
        if (y != 0) std::free(y);
        --Count;
    }
};

int main()
{
    STree t; t.Header = (SNodeBase*)std::malloc(sizeof(SNode)); t.Count = 2;
    int objA = 0, objB = 0;
    SNode* a = (SNode*)std::malloc(sizeof(SNode)); a->Key = 1.5f;
    a->Value.Data = &objA; a->Value.Info = new SCountInfo; a->Value.Info->RefCount = 1;
    a->Value.Info->DeleteFunc = DeleteObject; a->Value.Info->Data = &objA;
    SCountInfo* shared = new SCountInfo; shared->RefCount = 2; shared->DeleteFunc = DeleteObject; shared->Data = &objB;
    SNode* b = (SNode*)std::malloc(sizeof(SNode)); b->Key = 2.5f; b->Value.Data = &objB; b->Value.Info = shared;

    t.erase(a);       // last reference: destroy routine runs on the object
    if (g_deleted != 1 || g_lastObject != &objA || t.Count != 1) { std::printf("00ad4ca0_TEST FAIL\n"); return 1; }
    if (g_args[0] != &t.Header->Parent || g_args[1] != &t.Header->Left || g_args[2] != &t.Header->Right) { std::printf("00ad4ca0_TEST FAIL\n"); return 1; }
    t.erase(b);       // shared block: only the count drops
    if (g_deleted != 1 || shared->RefCount != 1 || t.Count != 0) { std::printf("00ad4ca0_TEST FAIL\n"); return 1; }
    delete shared; std::free(t.Header);
    std::printf("00ad4ca0_TEST PASS\n");
    return 0;
}