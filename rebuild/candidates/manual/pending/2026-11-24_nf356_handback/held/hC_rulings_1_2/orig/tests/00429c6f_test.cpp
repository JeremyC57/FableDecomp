#include <cstdio>
#include <new>
#include "rebuild_abi.h"
class CComponent {
public:
    virtual ~CComponent();
};
class CCPPointerInfo { public:
    long  RefCount;
    void  (FABLE_FASTCALL* DeleteFunc)(void* data);
    void* Data;
    CCPPointerInfo(long ref_count, void (FABLE_FASTCALL* delete_func)(void*), void* data)
        : RefCount(ref_count), DeleteFunc(delete_func), Data(data) {}
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    void Release();                                      // 0x004291DE (~CCountedPointer in retail; test stand-in)
    static void FABLE_FASTCALL DeleteData(void* data);       // 0x00428B10
    void Reset(T* data);
};
template <class T> void CCountedPointer<T>::Reset(T* data)
{
    Release();
    Data = data;
    if (Data)
        Info = new CCPPointerInfo(1, &CCountedPointer<T>::DeleteData, Data);
}
template void CCountedPointer<CComponent>::Reset(CComponent* data);

static int g_released = 0, g_deleted = 0;
CComponent::~CComponent() {}
template <class T> void CCountedPointer<T>::Release() { ++g_released; if (Info && --Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } Info = 0; Data = 0; }
template <class T> void FABLE_FASTCALL CCountedPointer<T>::DeleteData(void* data) { ++g_deleted; delete static_cast<T*>(data); }
int main()
{
    CCountedPointer<CComponent> p; p.Data = 0; p.Info = 0;
    CComponent* a = new CComponent;
    p.Reset(a);
    bool ok = g_released == 1 && p.Data == a && p.Info && p.Info->RefCount == 1 && p.Info->Data == a
        && p.Info->DeleteFunc == &CCountedPointer<CComponent>::DeleteData;
    p.Reset(0);
    ok = ok && g_released == 2 && g_deleted == 1 && p.Data == 0 && p.Info == 0;
    if (ok) std::printf("00429c6f_TEST PASS\n");
    return 0;
}