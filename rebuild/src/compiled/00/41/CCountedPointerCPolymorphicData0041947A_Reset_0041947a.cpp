// CCountedPointer<CPolymorphicData_0041947A>::Reset(CPolymorphicData_0041947A* data) -- drop the current reference, take ownership of data with a fresh shared-count record
// Leaf: ?Reset@?$CCountedPointer@VCPolymorphicData_0041947A@@@@QAEXPAVCPolymorphicData_0041947A@@@Z
// Retail: Fable.exe @ 0x0041947A, 59 bytes, `ret 4`; called from 0x00A5F1F4, 0x00A63524, tail-jumped from 0x00A5F780.
// Catalog label CCountedPointer<std::vector<CCharString>>::Reset is wrong: the instantiation's delete function (0x00419028:
// `test ecx,ecx / mov eax,[ecx] / push 1 / call [eax]`) is a virtual `delete`, so T is polymorphic, not a vector. The
// pointee class is not identified (the body is one template stamped out per T); it is named by address here.
// Body: Release() (0x004057A0); Data = data; if (Data) Info = new CCPPointerInfo(1, DeleteData, Data) (operator new 12 bytes,
// null-checked).
// Layout: CCountedPointer {Data +0, Info +4}; CCPPointerInfo {RefCount +0, DeleteFunc +4 (__fastcall), Data +8}.
// Opt: size TU (unaligned entry).
#pragma optimize("s", on)
#include <new>
class CPolymorphicData_0041947A {
public:
    virtual ~CPolymorphicData_0041947A();
};
class CCPPointerInfo { public:
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
    CCPPointerInfo(long ref_count, void (__fastcall* delete_func)(void*), void* data)
        : RefCount(ref_count), DeleteFunc(delete_func), Data(data) {}
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    void Release();                                      // 0x004057A0
    static void __fastcall DeleteData(void* data);       // 0x00419028
    void Reset(T* data);
};
template <class T> void CCountedPointer<T>::Reset(T* data)
{
    Release();
    Data = data;
    if (Data)
        Info = new CCPPointerInfo(1, &DeleteData, Data);
}
template void CCountedPointer<CPolymorphicData_0041947A>::Reset(CPolymorphicData_0041947A* data);