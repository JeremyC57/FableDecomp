// Behaviour test for C3DMeshFileVerticesChunk::GetChunkInfo (0x00ABBD70): "Number of vertices: %u" over the 32-byte vertex list.
#include <cstdio>
#include <cstring>
struct Vertex { unsigned char body[0x20]; };
struct Chunk { Vertex* start; Vertex* finish;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void GetChunkInfo(char* out) const { char buffer[128]; std::sprintf(buffer, "Number of vertices: %u", size()); std::strcpy(out, buffer); } };
int main()
{
    Vertex v[7]; Chunk c; c.start = v; c.finish = v + 7; char out[128];
    c.GetChunkInfo(out);
    if (std::strcmp(out, "Number of vertices: 7") != 0) { std::printf("00abbd70_TEST FAIL\n"); return 1; }
    c.finish = v; c.GetChunkInfo(out);
    if (std::strcmp(out, "Number of vertices: 0") != 0) { std::printf("00abbd70_TEST FAIL\n"); return 1; }
    std::printf("00abbd70_TEST PASS\n");
    return 0;
}