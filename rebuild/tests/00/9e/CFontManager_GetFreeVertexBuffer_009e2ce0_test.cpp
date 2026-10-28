// Test: CFontManager::GetFreeVertexBuffer -- round-robin cursor (CurrentVB + 1) % count, returns that buffer
#include <cstdio>
struct CVertexBuffer { int id; };
class CFontManager { public:
    CVertexBuffer** first; CVertexBuffer** last; long CurrentVB;
    CVertexBuffer& GetFreeVertexBuffer()
    {
        CurrentVB = (long)((unsigned long)(CurrentVB + 1) % (unsigned long)(last - first));
        return *first[CurrentVB];
    }
};
int main()
{
    CVertexBuffer b0 = { 0 }, b1 = { 1 }, b2 = { 2 };
    CVertexBuffer* v[3] = { &b0, &b1, &b2 };
    CFontManager m = { v, v + 3, 0 };
    int seq[4]; for (int i = 0; i < 4; ++i) seq[i] = m.GetFreeVertexBuffer().id;
    if (seq[0] != 1 || seq[1] != 2 || seq[2] != 0 || seq[3] != 1 || m.CurrentVB != 1) { std::printf("009e2ce0_TEST FAIL\n"); return 1; }
    std::printf("009e2ce0_TEST PASS\n");
    return 0;
}