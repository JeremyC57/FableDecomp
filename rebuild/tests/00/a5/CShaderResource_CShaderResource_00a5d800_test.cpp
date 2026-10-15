#include <cstdio>
static int g_log[32]; static int g_n = 0;
static void Log(int v) { g_log[g_n++] = v; }
struct Freed { int id; Freed(int i) : id(i) {} };
static void Free(void* p) { if (p) Log(((Freed*)p)->id); }

struct Vec { int id; void* p; Vec(int i) : id(i), p(0) {} ~Vec() { Log(id); Free(p); } };
struct InputBase { ~InputBase() { Log(-4); } };
struct Input : InputBase { Vec streams, decl; Input() : streams(0x1c), decl(0x28) {} };
struct Base { ~Base() { Log(-2); } };
struct Res : Base {
    void* bank; Vec shaders; void* mgr; Input input;
    Res() : shaders(8) {}
    void CloseShaderBank() { Log(0x750); bank = 0; }
    void Close() { CloseShaderBank(); mgr = 0; }
    ~Res() { Close(); if (mgr) Log(-9); }
};
static int Expect(const int* w, int n) { if (g_n != n) return 0; for (int i = 0; i < n; ++i) if (g_log[i] != w[i]) return 0; return 1; }
int main()
{

    { Res r; r.mgr = &r; }
    int w[] = { 0x750, 0x28, 0x1c, -4, 8, -2 };
    if (!Expect(w, 6)) { std::printf("00a5d800_TEST FAIL\n"); return 1; }
    std::printf("00a5d800_TEST PASS\n");
    return 0;
}