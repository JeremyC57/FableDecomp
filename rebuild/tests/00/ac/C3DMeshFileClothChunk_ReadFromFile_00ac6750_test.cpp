#include <cstdio>
#include <cstring>
#include <vector>
// Behaviour copy of C3DMeshFileClothChunk::ReadFromFile (0x00AC6750): version, name, index, code text, optional bytes.
struct Stream {
    unsigned int StreamPos; const unsigned char* CurrentSourcePtr; int SourceChunkBytesRemaining; int overflowCalls;
    Stream(const unsigned char* d, int n) : StreamPos(0), CurrentSourcePtr(d), SourceChunkBytesRemaining(n), overflowCalls(0) {}
    void ReadWithSrcChunkOverflow(unsigned char* buf, int size) { ++overflowCalls; std::memset(buf, 0, size); StreamPos += size; }
    void Read(void* buf, int size) {
        if (size <= 0) return;
        long long np = (long long)(StreamPos + size);
        if (!(np >= 0 && np <= 0x7fffffff)) return;
        if (SourceChunkBytesRemaining >= size) {
            std::memcpy(buf, CurrentSourcePtr, size); CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)buf, size);
    }
    unsigned int ReadULong() { unsigned int v; Read(&v, 4); return v; }
    float ReadFloat() { float v; Read(&v, 4); return v; }
    unsigned short ReadUShort() { unsigned short v; Read(&v, 2); return v; }
    bool ReadBool() { char v; Read(&v, 1); return v != 0; }
    unsigned int GetPosition() const { return StreamPos; }
};
struct Writer {
    unsigned char buf[512]; int n;
    Writer() : n(0) {}
    void U32(unsigned int v) { std::memcpy(buf + n, &v, 4); n += 4; }
    void F32(float v) { std::memcpy(buf + n, &v, 4); n += 4; }
    void U16(unsigned short v) { std::memcpy(buf + n, &v, 2); n += 2; }
    void U8(unsigned char v) { buf[n++] = v; }
};
struct Chunk {
    int MaterialIndex; int strings; std::vector<char> CompiledCode;
    void ReadString(Stream& in) { ++strings; while (true) { unsigned char ch; in.Read(&ch, 1); if (!ch) break; } }
    void ReadFromFile(Stream& in) {
        in.ReadULong(); ReadString(in); MaterialIndex = (int)in.ReadULong(); ReadString(in);
        if (in.ReadBool()) {
            unsigned int size = in.ReadULong();
            CompiledCode.resize(size, 0);
            if (size) in.Read(&CompiledCode[0], (int)size);
        }
    }
};
int main() {
    Writer w; w.U32(1); w.U8('n'); w.U8(0); w.U32(4); w.U8(0); w.U8(1); w.U32(3); w.U8(7); w.U8(8); w.U8(9);
    Stream s(w.buf, w.n); Chunk c; c.strings = 0; c.ReadFromFile(s);
    bool ok = c.strings == 2 && c.MaterialIndex == 4 && c.CompiledCode.size() == 3 && c.CompiledCode[2] == 9 && s.StreamPos == (unsigned int)w.n;
    Writer v; v.U32(1); v.U8(0); v.U32(2); v.U8(0); v.U8(0);
    Stream t(v.buf, v.n); Chunk d; d.strings = 0; d.ReadFromFile(t);
    ok = ok && d.CompiledCode.empty() && d.MaterialIndex == 2;
    if (!ok) { std::printf("00ac6750_TEST FAIL\n"); return 1; }
    std::printf("00ac6750_TEST PASS\n"); return 0;
}