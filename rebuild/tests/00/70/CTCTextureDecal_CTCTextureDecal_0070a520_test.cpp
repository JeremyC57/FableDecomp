#include <cstdio>
#include <cstdlib>
class CThing { public: int id; };
class CEngineTextureDecal;
static int g_baseCalls = 0;
struct CTCBase_Calls {
    virtual void v00() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { ++g_baseCalls; }
};
struct SDecalNode { SDecalNode* Next; SDecalNode* Prev; unsigned char Entry[0x18]; };
struct CRetailList_DecalEntry {
    SDecalNode* Node;
    CRetailList_DecalEntry() : Node(0)
    {
        SDecalNode* sentinel = static_cast<SDecalNode*>(std::malloc(sizeof(SDecalNode)));
        sentinel->Next = sentinel; sentinel->Prev = sentinel; Node = sentinel;
    }
};
struct CRetailVector_EngineDecal { CEngineTextureDecal* First; CEngineTextureDecal* Last; CEngineTextureDecal* End;
    CRetailVector_EngineDecal() : First(0), Last(0), End(0) { } };
class CTCTextureDecal : public CTCBase_Calls { public:
    CRetailList_DecalEntry Decals; CRetailVector_EngineDecal EngineDecals; long CurrentAllocEntryID; bool Dirty;
    virtual void v00() { }
    CTCTextureDecal(CThing* thing) : CTCBase_Calls(thing), Decals(), EngineDecals(), CurrentAllocEntryID(1), Dirty(false) { }
};
int main()
{
    CThing thing = { 4 };
    CTCTextureDecal* d = new CTCTextureDecal(&thing);
    SDecalNode* s = d->Decals.Node;
    if (g_baseCalls != 1 || d->ParentThing != &thing || s == 0 || s->Next != s || s->Prev != s || d->EngineDecals.First != 0
        || d->EngineDecals.Last != 0 || d->EngineDecals.End != 0 || d->CurrentAllocEntryID != 1 || d->Dirty)
    { std::printf("0070a520_TEST FAIL\n"); return 1; }
    std::free(s);
    delete d;
    std::printf("0070a520_TEST PASS\n");
    return 0;
}