// CWorldMap::GetLoadableSections() -- collect manager section names and the NULL section into a returned set
// Retail 0x004FF490, 203 bytes. Set build 004B2CA0; insertion 004AC380;
// CCharString ctor/dtor 0099EBF0/0099EAE0; tree copy ctor 00514400; erase 004ABA70.
// Canonical CWorldMap has data only; a derived view adds this data-independent virtual method.
#include "engine/CWorldMap.h"
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (MSVCR71)
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
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
        Head = (CSectionNode*)malloc(20);
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
        if (Head) free(Head);
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