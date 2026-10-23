// Behaviour test for CPlayer::~CPlayer (0x00488020): cached pointers cleared, def released,
// every scoped input process deleted in reverse declaration order, counted block freed when the count drops to 0.
#include <cstdio>

static int g_order[8];
static int g_n = 0;
static int g_defDeleted = 0;
static int g_listDestroyed = 0;
static int g_blockDestroyed = 0;
static int g_ipDestroyed = 0;

struct CInputProcess {
    int id;
    CInputProcess(int i) : id(i) { }
    virtual ~CInputProcess() { if (g_n < 8) g_order[g_n] = id; ++g_n; }
};
template<class T> struct scoped_ptr {
    T* ptr;
    scoped_ptr() : ptr(0) { }
    ~scoped_ptr() { delete ptr; }
};
struct CPlayerDef {
    long RefCount;
    CPlayerDef() : RefCount(1) { }
    virtual ~CPlayerDef() { }
    virtual void Delete() const { ++g_defDeleted; }
};
struct CDefPointer {
    CPlayerDef* Object;
    ~CDefPointer()
    {
        if (Object) {
            CPlayerDef* object = Object;
            if (--object->RefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
};
struct CPlayerModeList { ~CPlayerModeList() { ++g_listDestroyed; } };
struct CBlockObj { void Destroy() { ++g_blockDestroyed; } };
struct SCountBlock {
    long Count;
    void (CBlockObj::*Destroy)();
    CBlockObj* Object;
};
struct CCountedPointer {
    void* Data;
    SCountBlock* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->Count;
            if (Info->Count == 0) {
                (Info->Object->*Info->Destroy)();
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CIntelligentPointer { ~CIntelligentPointer() { ++g_ipDestroyed; } };

struct CPlayerModel {
    void* PlayerInterface;
    void* PlayerManager;
    void* Component;
    void* DefinitionManager;
    void* World;
    CIntelligentPointer ControlledCreature;
    CIntelligentPointer PlayerCharacter;
    CCountedPointer PHeroLogBook;
    scoped_ptr<CInputProcess> A;
    scoped_ptr<CInputProcess> B;
    scoped_ptr<CInputProcess> C;
    CPlayerModeList PlayerModes;
    CDefPointer PlayerDef;
    ~CPlayerModel()
    {
        PlayerInterface = 0;
        PlayerManager = 0;
        Component = 0;
        World = 0;
        DefinitionManager = 0;
    }
};

int main()
{
    CBlockObj obj;
    CPlayerDef def;
    {
        CPlayerModel* p = new CPlayerModel;
        p->PlayerInterface = p->PlayerManager = p->Component = p->DefinitionManager = p->World = &obj;
        p->PHeroLogBook.Data = &obj;
        p->PHeroLogBook.Info = new SCountBlock;
        p->PHeroLogBook.Info->Count = 1;
        p->PHeroLogBook.Info->Destroy = &CBlockObj::Destroy;
        p->PHeroLogBook.Info->Object = &obj;
        p->A.ptr = new CInputProcess(1);
        p->B.ptr = 0;
        p->C.ptr = new CInputProcess(3);
        p->PlayerDef.Object = &def;
        delete p;
    }
    if (g_n != 2 || g_order[0] != 3 || g_order[1] != 1) { std::printf("00488020_TEST FAIL\n"); return 1; }
    if (g_defDeleted != 1 || def.RefCount != 0) { std::printf("00488020_TEST FAIL\n"); return 1; }
    if (g_listDestroyed != 1 || g_blockDestroyed != 1 || g_ipDestroyed != 2) { std::printf("00488020_TEST FAIL\n"); return 1; }
    std::printf("00488020_TEST PASS\n");
    return 0;
}
