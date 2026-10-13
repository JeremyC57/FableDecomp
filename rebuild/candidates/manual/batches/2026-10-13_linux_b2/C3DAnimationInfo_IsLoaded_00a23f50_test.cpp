#include <cstdio>

struct Task { bool done; bool IsFinished() { return done; } };
struct Data { unsigned char pad[0x28]; void* Animation; unsigned char p2[4]; Task* LoadTask; };
struct A { unsigned char pad[0xc]; Data* Data_Object;
    bool IsLoaded() { Data* d = Data_Object; if (!d) return false; if (d->Animation) return true; Task* t = d->LoadTask; if (!t) return false; return t->IsFinished(); } };
int main()
{
    A a; Data d; Task t; a.Data_Object = 0; if (a.IsLoaded()) { std::printf("00a23f50_TEST FAIL\n"); return 1; }
    a.Data_Object = &d; d.Animation = 0; d.LoadTask = 0; if (a.IsLoaded()) { std::printf("00a23f50_TEST FAIL\n"); return 1; }
    d.LoadTask = &t; t.done = false; if (a.IsLoaded()) { std::printf("00a23f50_TEST FAIL\n"); return 1; }
    t.done = true; if (!a.IsLoaded()) { std::printf("00a23f50_TEST FAIL\n"); return 1; }
    int x; d.Animation = &x; d.LoadTask = 0; if (!a.IsLoaded()) { std::printf("00a23f50_TEST FAIL\n"); return 1; }
    std::printf("00a23f50_TEST PASS\n");
    return 0;
}
