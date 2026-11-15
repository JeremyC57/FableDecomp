// Behaviour test for 0x006F5C10 CCreatureActionBase::StatRegisterClass: the model below is the row body statement for statement
// (RegisterActionEventTypes, the null ACTION_NULL entry, then 54 `insert(make_pair(key, CCountedPointer(new X)))` in retail order);
// keys and operator-new sizes are the immediates retail 0x006F5C10 pushes / stores per insert block. CP is CCountedPointer's
// reference counting (Info block created at 1, copies increment, destructors release, DeleteFunc on zero). Checks: 55 entries,
// key -> prototype (size) mapping, null prototype for key 0, every surviving Info count back to exactly 1 (two copies + three
// releases per block), no prototype deleted, event types registered first.
#include <cstdio>
#include <map>
#include <utility>
static int g_live = 0, g_deleted = 0, g_events = 0, g_order_ok = 1;
struct Proto { int size; explicit Proto(int s) : size(s) { ++g_live; if (!g_events) g_order_ok = 0; } ~Proto() { --g_live; ++g_deleted; } };
struct Info { unsigned long RefCount; Proto* Data; };
struct CP {
    Proto* Data; Info* I;
    CP() : Data(0), I(0) {}
    explicit CP(Proto* p) : Data(p), I(p ? new Info : 0) { if (I) { I->RefCount = 1; I->Data = p; } }
    CP(const CP& o) : Data(o.Data), I(o.I) { if (I) ++I->RefCount; }
    ~CP() { if (I) { if (--I->RefCount == 0) { delete I->Data; delete I; } } Data = 0; I = 0; }
};
static std::map<int, CP> RegisteredActions;
static void RegisterActionEventTypes() { ++g_events; }
static void StatRegisterClass()
{
    RegisterActionEventTypes();
    RegisteredActions.insert(std::make_pair(0, CP()));
    RegisteredActions.insert(std::make_pair(1, CP(new Proto(332))));
    RegisteredActions.insert(std::make_pair(2, CP(new Proto(328))));
    RegisteredActions.insert(std::make_pair(4, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(5, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(6, CP(new Proto(284))));
    RegisteredActions.insert(std::make_pair(3, CP(new Proto(328))));
    RegisteredActions.insert(std::make_pair(29, CP(new Proto(328))));
    RegisteredActions.insert(std::make_pair(30, CP(new Proto(284))));
    RegisteredActions.insert(std::make_pair(34, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(45, CP(new Proto(340))));
    RegisteredActions.insert(std::make_pair(46, CP(new Proto(332))));
    RegisteredActions.insert(std::make_pair(7, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(8, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(9, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(10, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(11, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(21, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(22, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(23, CP(new Proto(280))));
    RegisteredActions.insert(std::make_pair(13, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(14, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(15, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(16, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(17, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(18, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(19, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(20, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(12, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(24, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(25, CP(new Proto(280))));
    RegisteredActions.insert(std::make_pair(27, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(26, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(28, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(41, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(42, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(43, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(44, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(53, CP(new Proto(304))));
    RegisteredActions.insert(std::make_pair(47, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(48, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(49, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(31, CP(new Proto(284))));
    RegisteredActions.insert(std::make_pair(32, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(33, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(35, CP(new Proto(284))));
    RegisteredActions.insert(std::make_pair(36, CP(new Proto(284))));
    RegisteredActions.insert(std::make_pair(37, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(38, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(39, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(40, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(50, CP(new Proto(284))));
    RegisteredActions.insert(std::make_pair(51, CP(new Proto(280))));
    RegisteredActions.insert(std::make_pair(52, CP(new Proto(276))));
    RegisteredActions.insert(std::make_pair(54, CP(new Proto(276))));
}
static const int K[][2] = {
    {0, 0},
    {1, 332},
    {2, 328},
    {4, 276},
    {5, 276},
    {6, 284},
    {3, 328},
    {29, 328},
    {30, 284},
    {34, 276},
    {45, 340},
    {46, 332},
    {7, 276},
    {8, 276},
    {9, 276},
    {10, 276},
    {11, 276},
    {21, 276},
    {22, 276},
    {23, 280},
    {13, 276},
    {14, 276},
    {15, 276},
    {16, 276},
    {17, 276},
    {18, 276},
    {19, 276},
    {20, 276},
    {12, 276},
    {24, 276},
    {25, 280},
    {27, 276},
    {26, 276},
    {28, 276},
    {41, 276},
    {42, 276},
    {43, 276},
    {44, 276},
    {53, 304},
    {47, 276},
    {48, 276},
    {49, 276},
    {31, 284},
    {32, 276},
    {33, 276},
    {35, 284},
    {36, 284},
    {37, 276},
    {38, 276},
    {39, 276},
    {40, 276},
    {50, 284},
    {51, 280},
    {52, 276},
    {54, 276}
};
int main()
{
    StatRegisterClass();
    int ok = g_order_ok && g_events == 1 && RegisteredActions.size() == 55 && g_deleted == 0 && g_live == 54;
    for (unsigned i = 0; ok && i < sizeof(K) / sizeof(K[0]); ++i) {
        std::map<int, CP>::iterator it = RegisteredActions.find(K[i][0]);
        if (it == RegisteredActions.end()) { ok = 0; break; }
        if (K[i][1] == 0) { if (it->second.Data != 0 || it->second.I != 0) ok = 0; continue; }
        if (!it->second.Data || it->second.Data->size != K[i][1] || !it->second.I || it->second.I->RefCount != 1) ok = 0;
    }
    std::printf(ok ? "006f5c10_TEST PASS\n" : "006f5c10_TEST FAIL\n");
    return 0;
}
