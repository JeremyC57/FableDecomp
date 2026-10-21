#include <cstdio>
#include <cstring>
// Behaviour copy of C3DMeshFileXBoundingSphereChunk::ReadFromFile (0x00AA4E40): four inlined stream reads.
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
struct Sphere {
    float Center_X, Center_Y, Center_Z, Radius;
    void ReadFromFile(Stream& in) {
        Center_X = in.ReadFloat(); Center_Y = in.ReadFloat(); Center_Z = in.ReadFloat(); Radius = in.ReadFloat();
    }
};
int main() {
    float v[4] = { 1.0f, -2.0f, 3.5f, 10.0f };
    unsigned char b[16]; std::memcpy(b, v, 16);
    Stream s(b, 16); Sphere sp; sp.ReadFromFile(s);
    Stream t(b, 10); Sphere sq; sq.ReadFromFile(t);
    if (sp.Center_X != 1.0f || sp.Center_Y != -2.0f || sp.Center_Z != 3.5f || sp.Radius != 10.0f || s.StreamPos != 16
        || s.overflowCalls != 0 || t.overflowCalls != 2 || sq.Center_Y != -2.0f || sq.Radius != 0.0f) {
        std::printf("00aa4e40_TEST FAIL\n"); return 1; }
    std::printf("00aa4e40_TEST PASS\n"); return 0;
}
