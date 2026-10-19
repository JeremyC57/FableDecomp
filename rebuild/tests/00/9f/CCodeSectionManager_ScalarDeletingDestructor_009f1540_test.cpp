#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
// CCodeSectionManager::ScalarDeletingDestructor(unsigned flags) -- destroy the manager, optionally release its storage.
// Retail009F1540,28 bytes after padding trim. RTTI0129C764 identifies the owner;
// this wrapper calls the complete destructor009F1560 and operator delete00BFE9BC.
struct CCodeSectionManager;
void __fastcall DestroyCodeSectionManager(CCodeSectionManager*); // 009F1560, member ABI
void DeleteCodeSectionManagerStorage(void*); // 00BFE9BC, cdecl
void* __fastcall CCodeSectionManager_ScalarDeletingDestructor(CCodeSectionManager* self,int,unsigned int flags) {
 DestroyCodeSectionManager(self);
 if(flags&1) DeleteCodeSectionManagerStorage(self);
 return self;
}

struct CCodeSectionManager { unsigned Marker; };
static CCodeSectionManager* expected;
static int calls; static bool bad;
void __fastcall DestroyCodeSectionManager(CCodeSectionManager* self) {
 if(self!=expected || calls!=0 || self->Marker!=123) bad=true;
 ++calls; self->Marker=456;
}
void DeleteCodeSectionManagerStorage(void* self) {
 if(self!=expected || calls!=1 || expected->Marker!=456) bad=true;
 ++calls;
}
int main() {
 for(unsigned flags=0;flags<4;++flags) {
  CCodeSectionManager manager; manager.Marker=123; expected=&manager; calls=0; bad=false;
  void* result=CCodeSectionManager_ScalarDeletingDestructor(&manager,0,flags);
  if(result!=&manager || bad || calls!=1+int(flags&1) || manager.Marker!=456) {
   std::printf("009f1540_TEST FAIL\n");return 1;
  }
 }
 std::printf("009f1540_TEST PASS\n");return 0;
}