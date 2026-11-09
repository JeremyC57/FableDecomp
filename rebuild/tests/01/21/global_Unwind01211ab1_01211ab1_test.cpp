// Behaviour test for 0x00C14F90 PKCDeferredBank::~PKCDeferredBank(): a model of the row; every data object holder A yields
// is passed to DeleteData until it yields null, then the controller and the game state are deleted (controller first) and
// both pointers cleared; the members are then destroyed in reverse declaration order.
#include <cstdio>
static int g_order[16]; static int g_n = 0;
static void Log(int v) { if (g_n < 16) g_order[g_n++] = v; }
struct Data { int id; };
struct HolderA {
    Data* items[3]; int next;
    Data* Next() { return next < 3 ? items[next++] : 0; }
    ~HolderA() { Log(100); }
};
struct HolderB { ~HolderB() { Log(200); } };
struct State { ~State() { Log(2); } };
struct Controller { ~Controller() { Log(1); } };
struct Bank {
    State* PState; Controller* PController; HolderA A; HolderB B;
    int deleted;
    void DeleteData(Data* d) { Log(10 + d->id); ++deleted; }
    ~Bank()
    {
        Data* data;
        while ((data = A.Next()) != 0)
            DeleteData(data);
        delete PController;
        PController = 0;
        delete PState;
        PState = 0;
    }
};
int main()
{
    Data d0 = { 0 }, d1 = { 1 };
    {
        Bank b; b.PState = new State; b.PController = new Controller; b.deleted = 0;
        b.A.items[0] = &d0; b.A.items[1] = &d1; b.A.items[2] = 0; b.A.next = 0;
    }
    int expect[] = { 10, 11, 1, 2, 200, 100 };
    int ok = g_n == 6;
    for (int i = 0; ok && i < 6; ++i) ok = g_order[i] == expect[i];
    std::printf(ok ? "01211ab1_TEST PASS\n" : "00c14f90_TEST FAIL\n");
    return ok ? 0 : 1;
}