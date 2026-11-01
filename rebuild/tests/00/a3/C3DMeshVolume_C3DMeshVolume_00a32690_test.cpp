#include <cstdio>
#include <new>
#if defined(_MSC_VER)
#define FC __fastcall
#else
#define FC
#endif
static int g_stringDtors = 0;
static int g_deletes = 0;
class CCharString {
public:
    CCharString() : PStringData(0) { }
    ~CCharString() { ++g_stringDtors; }
    void* PStringData;
};
class CCPPointerInfo {
public:
    unsigned long RefCount;
    void (FC* DeleteFunc)(void*);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
static void FC RecordDelete(void* data) { if (data) ++g_deletes; }
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; pair() : first(_T1()), second(_T2()) { } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer; __pointer->~_Tp(); }
}
class CContainmentVolume;
class C3DMeshVolume { public: CCharString Name; CCountedPointer<CContainmentVolume> ConvexVolume; };

static int Check(unsigned long startCount, int expectDeletes)
{
    g_stringDtors = 0; g_deletes = 0;
    static int object = 7;
    void* raw = ::operator new(sizeof(C3DMeshVolume));
    C3DMeshVolume* p = new (raw) C3DMeshVolume;
    CCPPointerInfo* info = new CCPPointerInfo;
    info->RefCount = startCount; info->DeleteFunc = RecordDelete; info->Data = &object;
    p->ConvexVolume.Data = 0; p->ConvexVolume.Info = info;
    p->~C3DMeshVolume();
    int ok = g_stringDtors == 1 && g_deletes == expectDeletes && p->ConvexVolume.Data == 0 && p->ConvexVolume.Info == 0;
    if (expectDeletes == 0) { ok = ok && info->RefCount == startCount - 1; delete info; }
    ::operator delete(raw);
    return ok;
}
int main()
{
    if (!Check(1, 1) || !Check(3, 0)) { std::printf("00a32690_TEST FAIL\n"); return 1; }
    std::printf("00a32690_TEST PASS\n");
    return 0;
}