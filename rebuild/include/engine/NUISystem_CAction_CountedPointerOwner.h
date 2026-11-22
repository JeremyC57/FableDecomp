#ifndef FABLE_NUISYSTEM_CACTION_COUNTEDPOINTER_OWNER_H
#define FABLE_NUISYSTEM_CACTION_COUNTEDPOINTER_OWNER_H
#include "engine/NUISystem_CAction.h"
// Fidelity scaffolding: original bodies in the size-mode scope of their native owner TUs.
// Caller must have no row-head optimize pragma; restore the command-line mode after these definitions.
#pragma optimize("s",on)
#ifdef FABLE_RETAIL_CACTION_COUNTED_DELETE_DECL_PASS26
// The constructor only takes this callback's address. The separate native owner at
// 00429F43 calls the original CAction destructor and operator delete.
template<> void FABLE_FASTCALL CCountedPointer<NUISystem::CAction>::DeleteData(void* data);
#else
template<> inline void FABLE_FASTCALL CCountedPointer<NUISystem::CAction>::DeleteData(void* data) {delete static_cast<NUISystem::CAction*>(data);}
#endif // FABLE_RETAIL_CACTION_COUNTED_DELETE_DECL_PASS26
template<> inline void CCountedPointer<NUISystem::CAction>::ReleaseData() {
 if(Info) {--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}
 Data=0;Info=0;
}
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
CCountedPointer<NUISystem::CAction>::CCountedPointer(NUISystem::CAction* data):Data(data) {
 if(data) Info=new CCPPointerInfo(1,DeleteData,(void*)Data);
 else Info=0;
}
template<> inline CCountedPointer<NUISystem::CAction>::~CCountedPointer(){ReleaseData();}
#pragma optimize("",on)
#endif
