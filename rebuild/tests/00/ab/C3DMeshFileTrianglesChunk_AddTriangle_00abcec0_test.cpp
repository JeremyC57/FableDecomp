// Behaviour test for C3DMeshFileTrianglesChunk::AddTriangle(const C3DTriangle2&) @ 0x00ABCEC0
// Appends one 6-byte triangle to TriangleList, growing the storage when it is full.
#include <cstdio>
#include <cstdlib>
#include <cstring>

class C3DTriangle2 { public: unsigned short Index[3]; };

struct CTriangleList {
    C3DTriangle2* start;
    C3DTriangle2* finish;
    C3DTriangle2* end;
    unsigned int overflows;
    CTriangleList() : start(0), finish(0), end(0), overflows(0) { }
    void insert_overflow(const C3DTriangle2& x)
    {
        ++overflows;
        unsigned int size = (unsigned int)(finish - start);
        unsigned int len = size + (size > 1 ? size : 1);
        C3DTriangle2* fresh = (C3DTriangle2*)std::malloc(len * sizeof(C3DTriangle2));
        if (size) std::memcpy(fresh, start, size * sizeof(C3DTriangle2));
        fresh[size] = x;
        std::free(start);
        start = fresh; finish = fresh + size + 1; end = fresh + len;
    }
    void push_back(const C3DTriangle2& x)
    {
        if (finish != end) { *finish = x; ++finish; }
        else insert_overflow(x);
    }
};

struct CTrianglesChunkModel {
    CTriangleList TriangleList;
    void AddTriangle(const C3DTriangle2& triangle) { TriangleList.push_back(triangle); }
};

int main()
{
    CTrianglesChunkModel chunk;
    int ok = sizeof(C3DTriangle2) == 6u;
    for (unsigned short i = 0; i < 5; ++i) {
        C3DTriangle2 t;
        t.Index[0] = (unsigned short)(i * 3); t.Index[1] = (unsigned short)(i * 3 + 1); t.Index[2] = (unsigned short)(i * 3 + 2);
        chunk.AddTriangle(t);
    }
    if (chunk.TriangleList.finish - chunk.TriangleList.start != 5) ok = 0;
    for (unsigned short i = 0; i < 5 && ok; ++i)
        for (int k = 0; k < 3; ++k)
            if (chunk.TriangleList.start[i].Index[k] != i * 3 + k) ok = 0;
    if (chunk.TriangleList.overflows < 2) ok = 0;
    std::free(chunk.TriangleList.start);
    if (!ok) { std::printf("00abcec0_TEST FAIL\n"); return 1; }
    std::printf("00abcec0_TEST PASS\n");
    return 0;
}