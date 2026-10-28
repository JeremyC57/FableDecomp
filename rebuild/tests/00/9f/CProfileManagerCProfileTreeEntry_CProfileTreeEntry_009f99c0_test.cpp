#include <stdio.h>
#include <stdlib.h>
#define free TestFree
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#endif
extern "C" void __cdecl free(void*);
namespace std { template<class T> class allocator {}; template<class V,class T,class Alloc> class _STLP_alloc_proxy:public Alloc { public: V _M_data; }; template<class T,class Alloc=allocator<T> > class vector { public: T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage; ~vector() { if(_M_start) free(_M_start); } }; template<class T> void __fastcall _Destroy(T* p) { p->~T(); } }
class CProfileManager; class CWideStringData; class CWideString { public: CWideStringData* PStringData; ~CWideString(); };
class CProfile { public: CProfileManager* Manager; double StartTime,TotalTime,LastTime; bool Running; double Percentage; CWideString Name; long TreeDepth; bool NewNode; long Visible,CallCounter,UpdatedCallCounter; };
class CProfileManager { public: struct CProfileTreeEntry { CProfile Profile; long ID; std::vector<CProfileTreeEntry*> Children; CProfileTreeEntry* Parent; ~CProfileTreeEntry(); }; };
CProfileManager::CProfileTreeEntry::~CProfileTreeEntry() {}
#undef free
static void* expectedBuffer; static void* expectedSub; static int calls,bad;
extern "C" void __cdecl TestFree(void* p) { if(p!=expectedBuffer || calls++) bad=1; free(p); }
CWideString::~CWideString() { if(this!=expectedSub || calls++!=1) bad=1; }
int main() { CProfileManager::CProfileTreeEntry* object=new CProfileManager::CProfileTreeEntry; object->Children._M_start=(CProfileManager::CProfileTreeEntry**)malloc(2*sizeof(CProfileManager::CProfileTreeEntry*)); expectedBuffer=object->Children._M_start; expectedSub=&object->Profile.Name; object->~CProfileTreeEntry(); if(bad || calls!=2) goto fail;
 object=new CProfileManager::CProfileTreeEntry; object->Children._M_start=0; expectedSub=&object->Profile.Name; calls=1; object->~CProfileTreeEntry(); if(bad || calls!=2) goto fail;
 puts("009f99c0_TEST PASS"); return 0; fail: puts("009f99c0_TEST FAIL"); return 1; }