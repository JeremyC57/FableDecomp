// Behaviour test for CTCInGameMenu::~CTCInGameMenu 0x0047E07F (model copy with stub manager/components)
#include <cstdio>
#include <cstdlib>

static int g_deleted = 0, g_removed = 0, g_slots = 0, g_cleared = 0, g_defDestroyed = 0, g_baseDtor = 0;

template <class T> struct CPODVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CPODVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CPODVector() { if (_M_start) std::free(_M_start); }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) { return *(begin() + n); }
    T* erase(T* first, T* last) { (void)last; _M_finish = first; return first; }
    void clear() { erase(begin(), end()); }
};
struct CComponent { virtual ~CComponent() { ++g_deleted; } };
struct CFrontEndManager {
    virtual void Slot06() { g_slots += 1; }
    virtual void Slot29() { g_slots += 10; }
    virtual void Slot37() { g_slots += 100; }
    static CFrontEndManager* GetInstance() { static CFrontEndManager m; return &m; }
};
struct CNodeList { void clear() { ++g_cleared; } };
static CNodeList g_frontEndNodeList;
struct CInventoryDef {
    long RefCount;
    virtual void Destroy() { ++g_defDestroyed; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CDefPointer {
    CInventoryDef* Object;
    ~CDefPointer() { if (Object) { Object->Release(); Object = 0; } }
};
struct CYesNo;
struct CTCBase {
    virtual ~CTCBase() { ++g_baseDtor; }
    void RemoveFromGlobalTCs() { ++g_removed; }
};
struct CTCInGameMenu : CTCBase {
    CDefPointer PDef;
    CPODVector<CComponent*> Components;
    CPODVector<long> FilePlayTimes;
    CYesNo* PVideoOptionsNotAcceptedDialog;
    CYesNo* PVideoOptionsChangedDialog;
    virtual ~CTCInGameMenu();
};

CTCInGameMenu::~CTCInGameMenu()
{
    RemoveFromGlobalTCs();
    for (unsigned long i = 0; i < Components.size(); ++i)
        delete Components[i];
    Components.clear();
    PVideoOptionsNotAcceptedDialog = 0;
    PVideoOptionsChangedDialog = 0;
    CFrontEndManager::GetInstance()->Slot06();
    CFrontEndManager::GetInstance()->Slot29();
    g_frontEndNodeList.clear();
    CFrontEndManager::GetInstance()->Slot37();
}

int main()
{
    CInventoryDef def; def.RefCount = 1;
    CTCInGameMenu* m = new CTCInGameMenu;
    m->PDef.Object = &def;
    m->Components._M_start = (CComponent**)std::malloc(3 * sizeof(CComponent*));
    m->Components._M_start[0] = new CComponent; m->Components._M_start[1] = 0; m->Components._M_start[2] = new CComponent;
    m->Components._M_finish = m->Components._M_end_of_storage = m->Components._M_start + 3;
    CTCBase* b = m;
    delete b;
    if (g_deleted != 2 || g_removed != 1 || g_slots != 111 || g_cleared != 1 || g_defDestroyed != 1 || g_baseDtor != 1) {
        std::printf("0047e07f_TEST FAIL\n"); return 1;
    }
    std::printf("0047e07f_TEST PASS\n");
    return 0;
}
