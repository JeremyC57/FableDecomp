#include <cstdio>
#include <new>
class CPolymorphicData_00429C6F {
public:
    virtual ~CPolymorphicData_00429C6F();
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
    void Release();                                      // 0x004291DE
    static void __fastcall DeleteData(void* data);       // 0x00428B10
    void Reset(T* data);
};
template <class T> void CCountedPointer<T>::Reset(T* data)
{
    Release();
    Data = data;
    if (Data)
        Info = new CCPPointerInfo(1, &DeleteData, Data);
}
template void CCountedPointer<CPolymorphicData_00429C6F>::Reset(CPolymorphicData_00429C6F* data);

static int g_released = 0, g_deleted = 0;
CPolymorphicData_00429C6F::~CPolymorphicData_00429C6F() {}
template <class T> void CCountedPointer<T>::Release() { ++g_released; if (Info && --Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } Info = 0; Data = 0; }
template <class T> void __fastcall CCountedPointer<T>::DeleteData(void* data) { ++g_deleted; delete static_cast<T*>(data); }
int main()
{
    CCountedPointer<CPolymorphicData_00429C6F> p; p.Data = 0; p.Info = 0;
    CPolymorphicData_00429C6F* a = new CPolymorphicData_00429C6F;
    p.Reset(a);
    bool ok = g_released == 1 && p.Data == a && p.Info && p.Info->RefCount == 1 && p.Info->Data == a
        && p.Info->DeleteFunc == &CCountedPointer<CPolymorphicData_00429C6F>::DeleteData;
    p.Reset(0);
    ok = ok && g_released == 2 && g_deleted == 1 && p.Data == 0 && p.Info == 0;
    if (ok) std::printf("00429c6f_TEST PASS\n");
    return 0;
}