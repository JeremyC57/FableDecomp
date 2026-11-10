// Behaviour test for 0x00485524 NUISystem::CActionParamCCountedPointer<CComponent> scalar deleting destructor: the held counted
// pointer is released (the component deleted with the last reference), then the object is freed only when flags & 1.
#include <stdio.h>
#include <new>
static int g_live = 0;
struct CComponent { CComponent() { ++g_live; } virtual ~CComponent() { --g_live; } };
struct CCPPointerInfo { unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
template <class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    static void DeleteData(void* d) { delete static_cast<T*>(d); }
    explicit CCountedPointer(T* d) : Data(d) { Info = new CCPPointerInfo; Info->RefCount = 1; Info->DeleteFunc = DeleteData; Info->Data = d; }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    void ReleaseData() { if (Info) { if (--Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } } Data = 0; Info = 0; }
    ~CCountedPointer() { ReleaseData(); }
};
struct CActionParam { long Type; CActionParam(long t) : Type(t) { } virtual ~CActionParam() { } };
struct CActionParamCCountedPointer : CActionParam {
    CCountedPointer<CComponent> Value;
    CActionParamCCountedPointer(CCountedPointer<CComponent>& v) : CActionParam(1), Value(v) { }
};
int main()
{
    bool ok = true;
    {
        CCountedPointer<CComponent> p(new CComponent);
        CActionParamCCountedPointer* param = new CActionParamCCountedPointer(p);   // two references
        ok = ok && p.Info->RefCount == 2;
        delete param;                                                          // flags & 1: release one, free
        ok = ok && p.Info->RefCount == 1 && g_live == 1;
    }
    ok = ok && g_live == 0;                                                    // last reference deletes the component
    {
        CComponent* c = new CComponent;
        CCountedPointer<CComponent> p(c);
        static double storage[(sizeof(CActionParamCCountedPointer) + 7) / 8];
        CActionParamCCountedPointer* param = new (storage) CActionParamCCountedPointer(p);
        p.ReleaseData();                                                       // the parameter holds the only reference
        param->~CActionParamCCountedPointer();                                 // flags == 0: destroy without freeing
        ok = ok && g_live == 0;
    }
    printf(ok ? "00485524_TEST PASS\n" : "00485524_TEST FAIL\n");
    return ok ? 0 : 1;
}
