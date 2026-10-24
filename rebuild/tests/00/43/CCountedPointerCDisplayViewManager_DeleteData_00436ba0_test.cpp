// Behaviour test for CCountedPointer<CDisplayViewManager>::DeleteData (0x00436BA0): a null pointer is ignored; otherwise the
// manager's current-view pointer drops one reference (destroying the view and its info block on the last one) and the manager
// is freed.
#include <cstdio>
static int g_viewsDeleted = 0;
struct CCPPointerInfo { long RefCount; void (*DeleteFunc)(void* data); void* Data; };
template <class T> class CCountedPointer {
public:
    T* Data; CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }
        }
        Data = 0; Info = 0;
    }
    static void DeleteData(void* data) { delete static_cast<T*>(data); }
};
struct CViewBase { int id; };
static void DeleteView(void* p) { delete static_cast<CViewBase*>(p); ++g_viewsDeleted; }
class CDisplayViewManager { public: CCountedPointer<CViewBase> PCurrentView; };
int main()
{
    int bad = 0;
    CCountedPointer<CDisplayViewManager>::DeleteData(0);
    CCPPointerInfo* info = new CCPPointerInfo; info->RefCount = 2; info->DeleteFunc = DeleteView; info->Data = new CViewBase;
    CDisplayViewManager* a = new CDisplayViewManager; a->PCurrentView.Data = (CViewBase*)info->Data; a->PCurrentView.Info = info;
    CDisplayViewManager* b = new CDisplayViewManager; b->PCurrentView.Data = (CViewBase*)info->Data; b->PCurrentView.Info = info;
    CCountedPointer<CDisplayViewManager>::DeleteData(a);
    if (info->RefCount != 1 || g_viewsDeleted != 0) bad = 1;
    CCountedPointer<CDisplayViewManager>::DeleteData(b);
    if (g_viewsDeleted != 1) bad = 1;
    CDisplayViewManager* c = new CDisplayViewManager; c->PCurrentView.Data = 0; c->PCurrentView.Info = 0;
    CCountedPointer<CDisplayViewManager>::DeleteData(c);
    if (g_viewsDeleted != 1) bad = 1;
    if (bad) { std::printf("00436ba0_TEST FAIL\n"); return 1; }
    std::printf("00436ba0_TEST PASS\n");
    return 0;
}