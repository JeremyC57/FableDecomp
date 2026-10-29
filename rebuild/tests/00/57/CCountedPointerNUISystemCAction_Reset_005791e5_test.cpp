#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
// CCountedPointer::Reset(T* object) -- release the old share then adopt a new raw object
// Grade: pending owner naming preflight; historical exact grade2 preserved
// Leaf: ?Reset@?$CCountedPointer@VCAction@NUISystem@@@@QAEXPAVCAction@NUISystem@@@Z
// Flags:
// Retail: 005791e5, 59 bytes. PDB Reset/DeleteData, destructor-feed and callback targets prove NUISystem::CAction.
// PDB complete member layout Data@0,Info@4 and no base fields; CCPPointerInfo ctor exact KP6IXPAX@Z0.
#pragma optimize("s", on)
namespace NUISystem { class CAction; }
class CCPPointerInfo { public:
 unsigned long RefCount;
 void (__fastcall* DeleteFunc)(void*);
 void* Data;
 CCPPointerInfo(unsigned long n, void (__fastcall* fn)(void*), void* p)
 : RefCount(n), DeleteFunc(fn), Data(p) {}
};
template<class T> class CCountedPointer { public:
 T* Data;
 CCPPointerInfo* Info;
 ~CCountedPointer();
 static void __fastcall DeleteData(void*);
 void Reset(T* object);
};
template<class T> void CCountedPointer<T>::Reset(T* object)
{
 this->~CCountedPointer<T>();
 Data=object;
 if (Data) Info=new CCPPointerInfo(1, DeleteData, Data);
}
template void CCountedPointer<NUISystem::CAction>::Reset(NUISystem::CAction*);

// Fixture-only empty pointee type: source uses it solely through pointer identity.
namespace NUISystem{class CAction{};}
static void*deletedObject=0;static unsigned int deletedCount=0;
template<class T>void __fastcall CCountedPointer<T>::DeleteData(void*p){deletedObject=p;++deletedCount;}
template<class T>CCountedPointer<T>::~CCountedPointer(){
 if(Info&&--Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}
 Data=0;Info=0;
}
typedef char InfoLayout[sizeof(CCPPointerInfo)==12?1:-1];
int main(){
 NUISystem::CAction oldObject,newObject;CCountedPointer<NUISystem::CAction>value;
 CCPPointerInfo*shared=new CCPPointerInfo(2,CCountedPointer<NUISystem::CAction>::DeleteData,&oldObject);
 value.Data=&oldObject;value.Info=shared;value.Reset(&newObject);
 if(shared->RefCount!=1||deletedCount||value.Data!=&newObject||!value.Info||value.Info->RefCount!=1||value.Info->Data!=&newObject||value.Info->DeleteFunc!=CCountedPointer<NUISystem::CAction>::DeleteData)return 1;
 delete shared;value.Reset(0);if(value.Data||value.Info||deletedCount!=1||deletedObject!=&newObject)return 2;
 value.Reset(0);if(value.Data||value.Info||deletedCount!=1)return 3;
 value.Data=&oldObject;value.Info=new CCPPointerInfo(1,CCountedPointer<NUISystem::CAction>::DeleteData,&oldObject);value.Reset(&newObject);
 if(deletedCount!=2||deletedObject!=&oldObject||value.Info->RefCount!=1)return 4;
 value.Reset(0);if(deletedCount!=3||deletedObject!=&newObject)return 5;
 std::puts("005791e5_TEST PASS");return 0;}