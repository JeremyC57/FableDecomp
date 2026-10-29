// Behaviour test for CThing::~CThing @ 0x004C8310: the destructor drops the thing count, removes only this thing's entries
// from the static pending-removal list, deletes every component and clears its presence bit (component adding is barred
// meanwhile and allowed again after), empties the component map and unregisters from the thing manager.
#include <stdio.h>
#include <stdlib.h>
class CThing;
struct SNode { SNode* Next; SNode* Prev; CThing* Thing; int Tag; };
struct SList {                                           // circular list with a header node (STLport shape)
    SNode* Head;
    void Init() { Head = (SNode*)malloc(sizeof(SNode)); Head->Next = Head; Head->Prev = Head; }
    void PushBack(CThing* t, int tag)
    {
        SNode* n = (SNode*)malloc(sizeof(SNode)); n->Thing = t; n->Tag = tag;
        n->Prev = Head->Prev; n->Next = Head; Head->Prev->Next = n; Head->Prev = n;
    }
    SNode* Erase(SNode* n) { SNode* next = n->Next; n->Prev->Next = next; next->Prev = n->Prev; free(n); return next; }
    int Count() const { int c = 0; for (SNode* n = Head->Next; n != Head; n = n->Next) c++; return c; }
};
static int g_componentsDeleted = 0, g_unregistered = 0;
static bool g_addingSeenDuringDelete = true;
class CTCBase { public:
    int Type; CThing* Owner;
    CTCBase(int type, CThing* owner) : Type(type), Owner(owner) { }
    virtual ~CTCBase();
    virtual int GetInterfaceType() const { return Type; }
};
struct SEntry { int first; CTCBase* second; };
int g_thingCount = 0;
class CThing { public:
    unsigned int UsingComponents[9];
    SEntry* CompFirst; SEntry* CompLast;
    bool AddingTCsPermitted;
    static SList TCsToBeRemoved;
    CThing() : CompFirst(0), CompLast(0), AddingTCsPermitted(true)
    { for (int i = 0; i < 9; ++i) UsingComponents[i] = 0; ++g_thingCount; }
    void Add(int type)
    {
        CompLast->first = type; CompLast->second = new CTCBase(type, this); ++CompLast;
        UsingComponents[type >> 5] |= 1u << (type & 31);
    }
    void Unregister() { g_unregistered++; }
    virtual ~CThing();
};
SList CThing::TCsToBeRemoved;
CTCBase::~CTCBase() { g_componentsDeleted++; if (Owner->AddingTCsPermitted) g_addingSeenDuringDelete = true; }
static unsigned int g_bitsAfter[9];
CThing::~CThing()
{
    --g_thingCount;
    AddingTCsPermitted = false;
    SNode* it = TCsToBeRemoved.Head->Next;
    while (it != TCsToBeRemoved.Head) {
        if (it->Thing == this)
            it = TCsToBeRemoved.Erase(it);
        else
            it = it->Next;
    }
    for (unsigned int i = 0; i < (unsigned int)(CompLast - CompFirst); ++i) {
        unsigned int pos = (unsigned int)CompFirst[i].second->GetInterfaceType();
        if (pos >= 274) abort();
        UsingComponents[pos / 32] &= ~(1u << (pos % 32));
        delete CompFirst[i].second;
        CompFirst[i].second = 0;
    }
    AddingTCsPermitted = true;
    CompLast = CompFirst;
    Unregister();
    for (int i = 0; i < 9; ++i) g_bitsAfter[i] = UsingComponents[i];
}
static int fail(int line) { printf("004c8310_TEST FAIL\n"); printf("line %d\n", line); return 1; }
int main()
{
    CThing::TCsToBeRemoved.Init();
    static SEntry storage[4];
    CThing* other = new CThing;
    CThing* thing = new CThing;
    thing->CompFirst = thing->CompLast = storage;
    thing->Add(3); thing->Add(0x8E); thing->Add(273);
    CThing::TCsToBeRemoved.PushBack(thing, 1);
    CThing::TCsToBeRemoved.PushBack(other, 2);
    CThing::TCsToBeRemoved.PushBack(thing, 3);
    CThing::TCsToBeRemoved.PushBack(thing, 4);
    if (g_thingCount != 2) return fail(__LINE__);
    g_addingSeenDuringDelete = false;
    delete thing;
    if (g_thingCount != 1 || g_componentsDeleted != 3 || g_unregistered != 1 || g_addingSeenDuringDelete) return fail(__LINE__);
    for (int i = 0; i < 9; ++i) if (g_bitsAfter[i] != 0) return fail(__LINE__);
    if (CThing::TCsToBeRemoved.Count() != 1 || CThing::TCsToBeRemoved.Head->Next->Tag != 2) return fail(__LINE__);
    if (storage[0].second != 0 || storage[1].second != 0 || storage[2].second != 0) return fail(__LINE__);
    delete other;
    if (g_thingCount != 0 || CThing::TCsToBeRemoved.Count() != 0 || g_unregistered != 2) return fail(__LINE__);
    printf("004c8310_TEST PASS\n");
    return 0;
}