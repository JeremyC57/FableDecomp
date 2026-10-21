#include <cstdio>
#include <new>
class CPolymorphicData_004194D1 {
public:
    virtual ~CPolymorphicData_004194D1();
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
    void Release();                                      // 0x00419108
    static void __fastcall DeleteData(void* data);       // 0x00419036
    void Reset(T* data);
};
template <class T> void CCountedPointer<T>::Reset(T* data)
{
    Release();
    Data = data;
    if (Data)
        Info = new CCPPointerInfo(1, &DeleteData, Data);
}
template void CCountedPointer<CPolymorphicData_004194D1>::Reset(CPolymorphicData_004194D1* data);

static int g_released = 0, g_deleted = 0;
CPolymorphicData_004194D1::~CPolymorphicData_004194D1() {}
template <class T> void CCountedPointer<T>::Release() { ++g_released; if (Info && --Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } Info = 0; Data = 0; }
template <class T> void __fastcall CCountedPointer<T>::DeleteData(void* data) { ++g_deleted; delete static_cast<T*>(data); }
int main()
{
    CCountedPointer<CPolymorphicData_004194D1> p; p.Data = 0; p.Info = 0;
    CPolymorphicData_004194D1* a = new CPolymorphicData_004194D1;
    p.Reset(a);
    bool ok = g_released == 1 && p.Data == a && p.Info && p.Info->RefCount == 1 && p.Info->Data == a
        && p.Info->DeleteFunc == &CCountedPointer<CPolymorphicData_004194D1>::DeleteData;
    p.Reset(0);
    ok = ok && g_released == 2 && g_deleted == 1 && p.Data == 0 && p.Info == 0;
    if (ok) std::printf("004194d1_TEST PASS\n");
    return 0;
}