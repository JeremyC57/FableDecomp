// CCountedPointer::Reset(T* object) -- release the old share then adopt a new raw object
// Grade: pending owner naming preflight; historical exact grade2 preserved
// Leaf: ?Reset@?$CCountedPointer@VCMeshDataBank@@@@QAEXPAVCMeshDataBank@@@Z
// Flags:
// Retail: 0042fc86, 59 bytes. PDB Reset/DeleteData, destructor-feed and callback targets prove CMeshDataBank.
// PDB complete member layout Data@0,Info@4 and no base fields; CCPPointerInfo ctor exact KP6IXPAX@Z0.
#pragma optimize("s", on)
class CMeshDataBank;
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
template void CCountedPointer<CMeshDataBank>::Reset(CMeshDataBank*);