// CCountedPointer<CPolymorphicData_00419428>::Reset(CPolymorphicData_00419428* data) -- drop the current reference, take ownership of data with a fresh shared-count record
// Leaf: ?Reset@?$CCountedPointer@VCPolymorphicData_00419428@@@@QAEXPAVCPolymorphicData_00419428@@@Z
// Retail: Fable.exe @ 0x00419428, 59 bytes, `ret 4`; none found (no static callers; reached through a vtable/thunk).
// Catalog label CCountedPointer<std::vector<CCharString>>::Reset is wrong: the instantiation's delete function (0x0041901D:
// `test ecx,ecx / mov eax,[ecx] / push 1 / call [eax]`) is a virtual `delete`, so T is polymorphic, not a vector. The
// pointee class is not identified (the body is one template stamped out per T); it is named by address here.
// Body: Release() (0x004190B6); Data = data; if (Data) Info = new CCPPointerInfo(1, DeleteData, Data) (operator new 12 bytes,
// null-checked).
// Layout: CCountedPointer {Data +0, Info +4}; CCPPointerInfo {RefCount +0, DeleteFunc +4 (__fastcall), Data +8}.
// Opt: size TU (unaligned entry).
#pragma optimize("s", on)
#include <new>
class CPolymorphicData_00419428 {
public:
    virtual ~CPolymorphicData_00419428();
};
struct CCPPointerInfo {
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
    void Release();                                      // 0x004190B6
    static void __fastcall DeleteData(void* data);       // 0x0041901D
    void Reset(T* data);
};
template <class T> void CCountedPointer<T>::Reset(T* data)
{
    Release();
    Data = data;
    if (Data)
        Info = new CCPPointerInfo(1, &DeleteData, Data);
}
template void CCountedPointer<CPolymorphicData_00419428>::Reset(CPolymorphicData_00419428* data);