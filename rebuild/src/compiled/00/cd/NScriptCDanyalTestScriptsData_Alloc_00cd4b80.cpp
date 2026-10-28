// NScript::CDanyalTestScriptsData::Alloc() -- Allocate an empty master-script data object.
// Leaf: ?Alloc@CDanyalTestScriptsData@NScript@@SIPAVCScriptDataBase@@XZ
// Identity: original NScript class and static fastcall Alloc name in both PDB name sets; own script-info registration 00cda390/00cda3ca pairs literal DanyalTestScripts with this data allocator. Own vtable012c273c has dtor/OnPersist/Init; own base constructor0099a2f0 is original CBaseClassNonCopyable. The propagated animation-util label is wrong.
// Layout: no owner engine header. Genuine CBaseClass/CBaseClassNonCopyable/CScriptDataBase hierarchy, original pure OnPersist/Init interface; size4 and no own fields proven by allocation and constructor.
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
class CBaseClass{public:virtual ~CBaseClass(){}};
class CBaseClassNonCopyable:public CBaseClass{public:CBaseClassNonCopyable();};
class CPersistContext;
class CScriptDataBase:public CBaseClassNonCopyable{public:virtual void OnPersist(CPersistContext&)=0;virtual void Init()=0;};
namespace NScript{class CDanyalTestScriptsData:public CScriptDataBase{public:virtual void OnPersist(CPersistContext&){}virtual void Init(){}static CScriptDataBase* __fastcall Alloc();};CScriptDataBase* __fastcall CDanyalTestScriptsData::Alloc(){return new CDanyalTestScriptsData;}}