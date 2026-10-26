#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __cdecl
#define FABLE_ENGINE_CWORLDMAP_H
class CWorldMap { public:};
#endif
// CWorldMap::GetLoadableSections() -- collect manager section names and the NULL section into a returned set
// Retail 0x004FF490, 203 bytes. Set build 004B2CA0; insertion 004AC380;
// CCharString ctor/dtor 0099EBF0/0099EAE0; tree copy ctor 00514400; erase 004ABA70.
// Canonical CWorldMap has data only; a derived view adds this data-independent virtual method.
#include "engine/CWorldMap.h"
extern "C" void* __cdecl AllocateSectionNode(unsigned int); // 0x00BFEA0E
extern "C" void __cdecl FreeSectionNode(void*); // 0x00BFEA14
// No generated CCharString header is present; retail string storage is one counted-data pointer.
struct CSectionName { void* PStringData; CSectionName(const char*, int); ~CSectionName(); };
struct CSectionNode { unsigned char Color; CSectionNode* Parent; CSectionNode* Left; CSectionNode* Right; CSectionName Value; };
struct CSectionInsertResult { CSectionNode* Position; bool Inserted; CSectionInsertResult() {} };
struct CSectionSet {
    CSectionNode* Head;
    unsigned long Count;
    unsigned long Compare; // empty comparator storage in the retail 12-byte tree
    CSectionSet() {
        Head = 0;
        Head = (CSectionNode*)AllocateSectionNode(20);
        Count = 0;
        Head->Color = 0;
        Head->Parent = 0;
        Head->Left = Head;
        Head->Right = Head;
    }
    CSectionSet(const CSectionSet&); // 0x00514400
    CSectionInsertResult Insert(const CSectionName&); // 0x004AC380
    void Erase(CSectionNode*); // 0x004ABA70
    ~CSectionSet() {
        if (Count) {
            Erase(Head->Parent);
            Head->Left = Head;
            Head->Parent = 0;
            Head->Right = Head;
            Count = 0;
        }
        if (Head) FreeSectionNode(Head);
    }
};
struct CSectionManager { void BuildLoadSections(CSectionSet*); }; // 0x004B2CA0
extern CSectionManager* gSectionManager; // 0x013B89FC
struct CWorldMapSectionView : CWorldMap { CSectionSet GetLoadableSections(); };
CSectionSet CWorldMapSectionView::GetLoadableSections()
{
    CSectionSet sections;
    gSectionManager->BuildLoadSections(&sections);
    {
        CSectionName nullSection("NULL", -1); // literal012393E4
        sections.Insert(nullSection);
    }
    return sections;
}

static int bad, allocated, freed, erased, built, inserted, named, destroyed;
static int initialCount, alreadyNull;
CSectionManager manager;
CSectionManager* gSectionManager = &manager;
extern "C" void* __cdecl AllocateSectionNode(unsigned int size)
{
    if (size != 20) bad = 1;
    ++allocated;
    return malloc(sizeof(CSectionNode));
}
extern "C" void __cdecl FreeSectionNode(void* value)
{
    CSectionNode* node = (CSectionNode*)value;
    if (node->Parent || node->Left != node || node->Right != node) bad = 1;
    ++freed;
    free(value);
}
CSectionName::CSectionName(const char* text, int length)
{
    if (length != -1 || text[0] != 'N' || text[1] != 'U' || text[2] != 'L' ||
        text[3] != 'L' || text[4] != 0) bad = 1;
    PStringData = (void*)text;
    ++named;
}
CSectionName::~CSectionName() { ++destroyed; }
void CSectionManager::BuildLoadSections(CSectionSet* sections)
{
    if (this != &manager || sections->Count || sections->Head->Color ||
        sections->Head->Parent || sections->Head->Left != sections->Head ||
        sections->Head->Right != sections->Head) bad = 1;
    sections->Count = initialCount;
    if (initialCount) sections->Head->Parent = sections->Head;
    ++built;
}
CSectionInsertResult CSectionSet::Insert(const CSectionName& name)
{
    if (built != 1 || !name.PStringData || named != 1 || destroyed) bad = 1;
    if (!alreadyNull) ++Count;
    Head->Parent = Head;
    ++inserted;
    CSectionInsertResult result;
    result.Position = Head;
    result.Inserted = !alreadyNull;
    return result;
}
CSectionSet::CSectionSet(const CSectionSet& source)
{
    if (inserted != 1 || destroyed != 1) bad = 1;
    Head = (CSectionNode*)AllocateSectionNode(20);
    Head->Color = 0;
    Head->Parent = source.Count ? Head : 0;
    Head->Left = Head->Right = Head;
    Count = source.Count;
}
void CSectionSet::Erase(CSectionNode* root)
{
    if (!Count || root != Head || inserted != 1 || destroyed != 1) bad = 1;
    ++erased;
}
static int run(int startingCount, int containsNull)
{
    bad = allocated = freed = erased = built = inserted = named = destroyed = 0;
    initialCount = startingCount;
    alreadyNull = containsNull;
    {
        CWorldMapSectionView world;
        CSectionSet result = world.GetLoadableSections();
        if (result.Count != (unsigned long)(startingCount + (containsNull ? 0 : 1)) ||
            result.Head->Color || result.Head->Parent != result.Head ||
            result.Head->Left != result.Head || result.Head->Right != result.Head) bad = 1;
    }
    return !bad && allocated >= 1 && allocated == freed && erased == allocated &&
        built == 1 && inserted == 1 && named == 1 && destroyed == 1;
}
int main()
{
    if (!run(0,0) || !run(2,0) || !run(2,1)) {
        printf("004ff490_TEST FAIL\n"); return 1;
    }
    printf("004ff490_TEST PASS\n"); return 0;
}