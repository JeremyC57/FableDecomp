#include <cstdio>
#include <cstring>
// Behaviour copy of C3DMeshFileXBoundingBoxChunk::ReadFromFile (0x00AA5030): six inlined stream reads.
struct Stream {
    unsigned int StreamPos; const unsigned char* CurrentSourcePtr; int SourceChunkBytesRemaining; int overflowCalls;
    Stream(const unsigned char* d, int n) : StreamPos(0), CurrentSourcePtr(d), SourceChunkBytesRemaining(n), overflowCalls(0) {}
    void ReadWithSrcChunkOverflow(unsigned char* buf, int size) { ++overflowCalls; std::memset(buf, 0, size); StreamPos += size; }
    void Read(void* buf, int size) {
        long long np = (long long)(StreamPos + size);
        if (!(np >= 0 && np <= 0x7fffffff)) return;
        if (SourceChunkBytesRemaining >= size) {
            std::memcpy(buf, CurrentSourcePtr, size); CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)buf, size);
    }
    float ReadFloat() { float v; Read(&v, 4); return v; }
};
struct Box {
    float Center_X, Center_Y, Center_Z, Diagonal_X, Diagonal_Y, Diagonal_Z;
    void ReadFromFile(Stream& in) {
        Center_X = in.ReadFloat(); Center_Y = in.ReadFloat(); Center_Z = in.ReadFloat();
        Diagonal_X = in.ReadFloat(); Diagonal_Y = in.ReadFloat(); Diagonal_Z = in.ReadFloat();
    }
};
int main() {
    float v[6] = { 1.0f, -2.0f, 3.5f, 10.0f, 4.0f, -0.5f };
    unsigned char b[24]; std::memcpy(b, v, 24);
    Stream s(b, 24); Box bx; bx.ReadFromFile(s);
    Stream t(b, 14); Box by; by.ReadFromFile(t);
    if (bx.Center_X != 1.0f || bx.Center_Y != -2.0f || bx.Center_Z != 3.5f || bx.Diagonal_X != 10.0f
        || bx.Diagonal_Y != 4.0f || bx.Diagonal_Z != -0.5f || s.StreamPos != 24 || s.overflowCalls != 0
        || t.overflowCalls != 3 || by.Center_Z != 3.5f || by.Diagonal_X != 0.0f || by.Diagonal_Z != 0.0f) {
        std::printf("00aa5030_TEST FAIL\n"); return 1; }
    std::printf("00aa5030_TEST PASS\n"); return 0;
}