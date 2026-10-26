// CCodeSectionManager::ScalarDeletingDestructor(unsigned flags) -- destroy the manager, optionally release its storage.
// Retail009F1540,28 bytes after padding trim. RTTI0129C764 identifies the owner;
// this wrapper calls the complete destructor009F1560 and operator delete00BFE9BC.
class CCodeSectionManager;
void __fastcall DestroyCodeSectionManager(CCodeSectionManager*); // 009F1560, member ABI
void DeleteCodeSectionManagerStorage(void*); // 00BFE9BC, cdecl
void* __fastcall CCodeSectionManager_ScalarDeletingDestructor(CCodeSectionManager* self,int,unsigned int flags) {
 DestroyCodeSectionManager(self);
 if(flags&1) DeleteCodeSectionManagerStorage(self);
 return self;
}