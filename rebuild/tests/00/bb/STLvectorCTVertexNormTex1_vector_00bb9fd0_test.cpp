// behaviour test for 0x00BB9FD0 std::vector<CTVertexNormTex1>::vector(size_type n)
#include <cstdio>
#include <cstdlib>
#include <new>

struct Vertex { float X, Y, Z, NX, NY, NZ, U, V; Vertex() { } };

struct VertexVector {
    Vertex* Start;
    Vertex* Finish;
    Vertex* End;
    explicit VertexVector(unsigned int n) : Start(0), Finish(0), End(0)
    {
        Start = n != 0 ? (Vertex*)std::malloc(n * sizeof(Vertex)) : 0;
        Finish = Start;
        End = Start + n;
        Vertex tmp;
        tmp.X = 1.0f;
        Vertex* cur = Start;
        for (unsigned int k = n; k > 0; --k, ++cur)
            new (cur) Vertex(tmp);
        Finish = cur;
    }
};

int main()
{
    VertexVector v(5);
    if (v.Start == 0 || v.Finish != v.Start + 5 || v.End != v.Start + 5) { std::printf("00bb9fd0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (v.Start[i].X != 1.0f) { std::printf("00bb9fd0_TEST FAIL\n"); return 1; }
    VertexVector e(0);
    if (e.Start != 0 || e.Finish != 0 || e.End != 0) { std::printf("00bb9fd0_TEST FAIL\n"); return 1; }
    std::free(v.Start);
    std::printf("00bb9fd0_TEST PASS\n");
    return 0;
}