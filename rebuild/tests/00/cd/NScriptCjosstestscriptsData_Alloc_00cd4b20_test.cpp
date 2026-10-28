#include <stdio.h>
// NScript::Cjoss_test_scriptsData::Alloc() -- Allocate an empty master-script data object.
// Leaf: ?Alloc@Cjoss_test_scriptsData@NScript@@SIPAVCScriptDataBase@@XZ
// Identity: original NScript class and static fastcall Alloc name in both PDB name sets; own script-info registration 00cda705/00cda73f pairs literal joss_test_scripts with this data allocator. Own vtable012c2730 has dtor/OnPersist/Init; own base constructor0099a2f0 is original CBaseClassNonCopyable. The propagated animation-util label is wrong.
// Layout: no owner engine header. Genuine CBaseClass/CBaseClassNonCopyable/CScriptDataBase hierarchy, original pure OnPersist/Init interface; size4 and no own fields proven by allocation and constructor.
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
class CBaseClass{public:virtual ~CBaseClass(){}};
class CBaseClassNonCopyable:public CBaseClass{public:CBaseClassNonCopyable();};
class CPersistContext;
class CScriptDataBase:public CBaseClassNonCopyable{public:virtual void OnPersist(CPersistContext&)=0;virtual void Init()=0;};
namespace NScript{class Cjoss_test_scriptsData:public CScriptDataBase{public:virtual void OnPersist(CPersistContext&){}virtual void Init(){}static CScriptDataBase* __fastcall Alloc();};CScriptDataBase* __fastcall Cjoss_test_scriptsData::Alloc(){return new Cjoss_test_scriptsData;}}
static int baseConstructed;CBaseClassNonCopyable::CBaseClassNonCopyable(){++baseConstructed;}class CPersistContext{};int main(){CScriptDataBase* first=NScript::Cjoss_test_scriptsData::Alloc();CScriptDataBase* second=NScript::Cjoss_test_scriptsData::Alloc();bool ok=first&&second&&first!=second&&baseConstructed==2;CPersistContext context;first->OnPersist(context);second->Init();delete first;delete second;puts(ok?"00cd4b20_TEST PASS":"00cd4b20_TEST FAIL");return ok?0:1;}