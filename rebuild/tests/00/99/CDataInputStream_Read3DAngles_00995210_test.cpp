// Behaviour test: CDataInputStream::Read3DAngles (0x00995210) -- three 4-byte reads (XY, YZ, XZ) from the source chunk,
// each advancing the source pointer and stream position (falling back to the chunk-overflow reader when the chunk runs out).
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 int64;
#else
typedef long long int64;
#endif
class C3DAngles { public: float AngleXY, AngleYZ, AngleXZ; };
struct CStream {
    unsigned long StreamPos; const unsigned char* CurrentSourcePtr; long SourceChunkBytesRemaining; int Overflows;
    void Read(void* buffer, long size)
    {
        int64 newPosition = (int64)(StreamPos + size);
        if (!(newPosition >= 0 && newPosition <= 0x7fffffff)) return;
        if (SourceChunkBytesRemaining >= size) {
            std::memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else {
            ++Overflows; std::memset(buffer, 0, size);
        }
    }
    float ReadFloat() { float v; Read(&v, sizeof(v)); return v; }
    C3DAngles Read3DAngles() { C3DAngles a; a.AngleXY = ReadFloat(); a.AngleYZ = ReadFloat(); a.AngleXZ = ReadFloat(); return a; }
};
int main()
{
    float src[3] = { 1.5f, -2.0f, 0.25f };
    CStream s; s.StreamPos = 0; s.CurrentSourcePtr = (const unsigned char*)src; s.SourceChunkBytesRemaining = 12; s.Overflows = 0;
    C3DAngles a = s.Read3DAngles();
    bool ok = a.AngleXY == 1.5f && a.AngleYZ == -2.0f && a.AngleXZ == 0.25f && s.StreamPos == 12 && s.SourceChunkBytesRemaining == 0
        && s.Overflows == 0;
    CStream t; t.StreamPos = 0; t.CurrentSourcePtr = (const unsigned char*)src; t.SourceChunkBytesRemaining = 8; t.Overflows = 0;
    C3DAngles b = t.Read3DAngles();
    ok = ok && b.AngleXY == 1.5f && b.AngleYZ == -2.0f && t.Overflows == 1;
    if (!ok) { std::printf("00995210_TEST FAIL\n"); return 1; }
    std::printf("00995210_TEST PASS\n");
    return 0;
}