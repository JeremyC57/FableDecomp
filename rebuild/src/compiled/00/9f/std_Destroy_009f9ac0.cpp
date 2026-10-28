// std::_Destroy(CProfileManager::CProfileTreeEntry* object) -- destroy the owned vector then its earlier nontrivial member.
// Leaf: ??$_Destroy@UCProfileTreeEntry@CProfileManager@@@std@@YIXPAUCProfileTreeEntry@CProfileManager@@@Z
// Identity: probable function role; PDB unique CProfileTreeEntry Profile+0/Name+48,Children+76,Parent+88; own ctor0x9F9990/copy0x9F99E0/Construct0x9F9A90 corroborate record, both bodies unreferenced so role spelling probable.
// Layout: no record header. Typed real members provide vector start and tracked pointer/string destruction; CProfile uses actual double-aligned72-byte layout and nested record key is struct.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#endif
extern "C" void __cdecl free(void*);
namespace std { template<class T> class allocator {}; template<class V,class T,class Alloc> class _STLP_alloc_proxy:public Alloc { public: V _M_data; }; template<class T,class Alloc=allocator<T> > class vector { public: T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage; ~vector() { if(_M_start) free(_M_start); } }; template<class T> void __fastcall _Destroy(T* p) { p->~T(); } }
class CProfileManager; class CWideStringData; class CWideString { public: CWideStringData* PStringData; ~CWideString(); };
class CProfile { public: CProfileManager* Manager; double StartTime,TotalTime,LastTime; bool Running; double Percentage; CWideString Name; long TreeDepth; bool NewNode; long Visible,CallCounter,UpdatedCallCounter; };
class CProfileManager { public: struct CProfileTreeEntry { CProfile Profile; long ID; std::vector<CProfileTreeEntry*> Children; CProfileTreeEntry* Parent; ~CProfileTreeEntry() {} }; };
template void __fastcall std::_Destroy<CProfileManager::CProfileTreeEntry>(CProfileManager::CProfileTreeEntry*);