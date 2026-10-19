// CCodeSectionManager::~CCodeSectionManager() -- destroy the section and name maps, then the singleton base.
// Retail 009F1560,185 bytes; called by deleting destructor009F1540 (RTTI0129C764 CCodeSectionManager).
// Final vptr0129C75C belongs to CTBaseSingleton. No shared headers exist for either class.
// Donor map fields ShortNameToHandle/LongNameToHandle/SectionDesc are at+4/+14/+24 with16-byte headers;
// retail uses12-byte STLport headers at+4/+10/+1c. The last header word is unused here.
// Leaf: ??1CCodeSectionManager
extern "C" void __cdecl free(void*);
struct CSectionMapNode { long Color; CSectionMapNode* Parent; CSectionMapNode* Left; CSectionMapNode* Right; };
struct CNameToHandleMap {
 CSectionMapNode* Head; unsigned long Size; unsigned char OpaquePolicy[4];
 void Erase(CSectionMapNode*); // 00579435: recursive node destruction
 ~CNameToHandleMap() {
  if(Size) { Erase(Head->Parent); Head->Left=Head; Head->Parent=0; Head->Right=Head; Size=0; }
  if(Head) free(Head); // 00BFEA14
 }
};
struct CSectionDescriptorMap {
 CSectionMapNode* Head; unsigned long Size; unsigned char OpaquePolicy[4];
 void Erase(CSectionMapNode*); // 009F0D40: recursive section-descriptor destruction
 ~CSectionDescriptorMap() {
  if(Size) { Erase(Head->Parent); Head->Left=Head; Head->Parent=0; Head->Right=Head; Size=0; }
  if(Head) free(Head); // 00BFEA14
 }
};
struct CTBaseSingleton { virtual ~CTBaseSingleton() {} };
struct __declspec(novtable) CCodeSectionManager : CTBaseSingleton {
 CNameToHandleMap ShortNameToHandle; // +04
 CNameToHandleMap LongNameToHandle; // +10
 CSectionDescriptorMap SectionDesc; // +1c
 ~CCodeSectionManager();
};
CCodeSectionManager::~CCodeSectionManager() {}