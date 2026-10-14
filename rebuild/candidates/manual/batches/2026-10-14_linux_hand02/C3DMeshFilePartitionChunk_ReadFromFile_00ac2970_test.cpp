#include <cstdio>

struct Elem { void* p[8]; long read; Elem() { for (int i = 0; i < 8; ++i) p[i] = 0; read = 0; } void ReadFromFile(int& in) { read = in; } };
struct CP { Elem* d; CP() : d(0) {} void Reset(Elem* e) { delete d; d = e; } ~CP() { delete d; } };
struct Chunk { CP tree; void ReadFromFile(int& in) { Elem* e = new Elem; e->ReadFromFile(in); tree.Reset(e); } };
int main()
{
    Chunk c; int in = 9; c.ReadFromFile(in);
    if (!c.tree.d || c.tree.d->read != 9 || c.tree.d->p[7]) { std::printf("00ac2970_TEST FAIL\n"); return 1; }
    std::printf("00ac2970_TEST PASS\n");
    return 0;
}
