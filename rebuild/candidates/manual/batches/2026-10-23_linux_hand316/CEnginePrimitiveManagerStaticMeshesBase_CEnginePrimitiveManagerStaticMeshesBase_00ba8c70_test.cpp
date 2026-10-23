#include <cstdio>
// Model of the compiler-generated manager destructor: every CVertexShader / CPixelShader member (singles and
// [4] tables) drops one reference on its shared data, deleting the data when the count reaches 0, in reverse
// declaration order; the occlusion base destructor runs last.
static int g_log[32]; static int g_logCount = 0; static int g_deleted = 0; static int g_baseRan = 0;
struct Data { int id; long refCount; ~Data() { ++g_deleted; } };
struct Shader {
    Data* data;
    Shader() : data(0) { }
    ~Shader()
    {
        Data* d = data;
        if (d) {
            g_log[g_logCount++] = d->id;
            if (--d->refCount == 0)
                delete d;
        }
    }
};
struct Base { ~Base() { g_baseRan = g_logCount; } };
struct Manager : Base {
    Shader table[4];
    Shader single;
    Shader outline;
};
int main()
{
    int bad = 0;
    Data* shared = new Data; shared->id = 9; shared->refCount = 3;
    {
        Manager* m = new Manager;
        m->table[0].data = shared; m->table[1].data = shared;
        Data* a = new Data; a->id = 1; a->refCount = 1; m->table[3].data = a;
        Data* b = new Data; b->id = 2; b->refCount = 1; m->single.data = b;
        Data* c = new Data; c->id = 3; c->refCount = 1; m->outline.data = c;
        delete m;
    }
    if (g_logCount != 5 || g_log[0] != 3 || g_log[1] != 2 || g_log[2] != 1 || g_log[3] != 9 || g_log[4] != 9) bad = 1;
    if (g_deleted != 3 || shared->refCount != 1 || g_baseRan != 5) bad = 1;
    delete shared;
    if (bad) { std::printf("00ba8c70_TEST FAIL\n"); return 1; }
    std::printf("00ba8c70_TEST PASS\n");
    return 0;
}
