#include <cstdio>
#include <new>
class CPolymorphicData_00433D3A {
public:
    virtual ~CPolymorphicData_00433D3A();
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
    void Release();                                      // 0x00433CC0
    static void __fastcall DeleteData(void* data);       // 0x00433CB5
    void Reset(T* data);
};
template <class T> void CCountedPointer<T>::Reset(T* data)
{
    Release();
    Data = data;
    if (Data)
        Info = new CCPPointerInfo(1, &DeleteData, Data);
}
template void CCountedPointer<CPolymorphicData_00433D3A>::Reset(CPolymorphicData_00433D3A* data);

static int g_released = 0, g_deleted = 0;
CPolymorphicData_00433D3A::~CPolymorphicData_00433D3A() {}
template <class T> void CCountedPointer<T>::Release() { ++g_released; if (Info && --Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } Info = 0; Data = 0; }
template <class T> void __fastcall CCountedPointer<T>::DeleteData(void* data) { ++g_deleted; delete static_cast<T*>(data); }
int main()
{
    CCountedPointer<CPolymorphicData_00433D3A> p; p.Data = 0; p.Info = 0;
    CPolymorphicData_00433D3A* a = new CPolymorphicData_00433D3A;
    p.Reset(a);
    bool ok = g_released == 1 && p.Data == a && p.Info && p.Info->RefCount == 1 && p.Info->Data == a
        && p.Info->DeleteFunc == &CCountedPointer<CPolymorphicData_00433D3A>::DeleteData;
    p.Reset(0);
    ok = ok && g_released == 2 && g_deleted == 1 && p.Data == 0 && p.Info == 0;
    if (ok) std::printf("00433d3a_TEST PASS\n");
    return 0;
}