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