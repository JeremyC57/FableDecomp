#include <cstdio>

struct CArr { void* b; void* e; void* c; CArr() : b(0), e(0), c(0) {} };
struct Chunk { const char* type; long BoneIndex; CArr VertexList;
    Chunk() : type("VGRP"), BoneIndex(-1) {} };
int main()
{
    Chunk c;
    if (c.BoneIndex != -1 || c.VertexList.b || c.VertexList.e || c.VertexList.c || c.type[0] != 'V') { std::printf("00abc650_TEST FAIL\n"); return 1; }
    std::printf("00abc650_TEST PASS\n");
    return 0;
}
