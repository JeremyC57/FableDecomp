// Behaviour test for CScriptGameResourceObjectMovieBase::SetImplementation 0x008AB970: the handle takes ownership of the new implementation through its
// counted pointer (the previous one is released and deleted when its count drops to zero; the new one is counted once).
#include <cstdio>
static int g_deleted = 0;
struct CImpl { int id; ~CImpl() { ++g_deleted; } };
struct CCPPointerInfo { unsigned long RefCount; };
template <class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    void Reset(T* data) {
        if (Info && --Info->RefCount == 0) { delete Data; delete Info; }
        Info = 0; Data = data;
        if (data) { Info = new CCPPointerInfo; Info->RefCount = 1; }
    }
};
struct CScriptGameResourceObjectMovieBase {
    void* VPtr; long Word4; CCountedPointer<CImpl> PImp;
    void SetImplementation(CImpl* imp) { PImp.Reset(imp); }
};
int main()
{
    CScriptGameResourceObjectMovieBase h; h.SetImplementation(new CImpl());
    if (!h.PImp.Data || h.PImp.Info->RefCount != 1 || g_deleted != 0) { std::printf("008ab970_TEST FAIL\n"); return 1; }
    CImpl* second = new CImpl();
    h.SetImplementation(second);
    if (h.PImp.Data != second || g_deleted != 1) { std::printf("008ab970_TEST FAIL\n"); return 1; }
    h.SetImplementation(0);
    if (h.PImp.Data != 0 || h.PImp.Info != 0 || g_deleted != 2) { std::printf("008ab970_TEST FAIL\n"); return 1; }
    std::printf("008ab970_TEST PASS\n");
    return 0;
}