// Behaviour test (0x006c88d0): CPersistTraits<COpinionDeed*>::TransferBinaryIn allocates a default deed and reads type, handle, position, frame (no longer happening), the two flags, extra data and the target
#include <cstdio>
#include <cstring>
#include <cmath>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
// Memory input stream model: the chunk pointer/length fast path of CMemoryDataInputStream::Read and a
// ReadWithSrcChunkOverflow that serves the rest from the backing buffer (counted, so the slow path is checked).
struct TestStream {
    unsigned int         position;
    int                  sourceLength;
    const unsigned char* chunkBase;
    int                  chunkPosition;
    int                  chunkLength;
    const unsigned char* data;
    int                  overflowCalls;
    void Init(const unsigned char* bytes, int size, int chunk)
    {
        position = 0; sourceLength = size; chunkBase = bytes; chunkPosition = 0; chunkLength = chunk; data = bytes; overflowCalls = 0;
    }
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size)
    {
        ++overflowCalls;
        std::memcpy(buffer, data + position, size);
        position += size;
        chunkBase = data + position;
        chunkLength = sourceLength - (int)position;
    }
    static bool IsValidPosition(i64 p) { return p >= 0 && p <= 0x7fffffff; }
    void Read(void* buffer, int size)
    {
        i64 newPosition = (i64)(unsigned int)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            std::memcpy(buffer, chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
static void PutLong(unsigned char* p, int v) { std::memcpy(p, &v, 4); }
static void PutFloat(unsigned char* p, float v) { std::memcpy(p, &v, 4); }
class C3DVector { public: float X, Y, Z; C3DVector() : X(0), Y(0), Z(0) {} };
struct CThingRef { int UID; };
class COpinionDeed { public:
    int DeedType; int DeedHandle; C3DVector Position; CThingRef PTarget; int FrameHappened;
    bool IsStillHappening; bool IsCrimeReportedToVillage; bool IsCrime; int ExtraData;
    COpinionDeed(int type, int handle, const C3DVector& position, bool still, bool crime, int extra)
        : DeedType(type), DeedHandle(handle), Position(position), FrameHappened(0), IsStillHappening(still),
          IsCrimeReportedToVillage(false), IsCrime(crime), ExtraData(extra) { PTarget.UID = 0; }
};
static int ReadLong(TestStream* s) { int n; s->Read(&n, 4); return n; }
static bool ReadBool(TestStream* s) { unsigned char b; s->Read(&b, 1); return b != 0; }
static float ReadFloat(TestStream* s) { float f; s->Read(&f, 4); return f; }
static void TransferBinaryInPointer(TestStream* s, CThingRef* p) { p->UID = ReadLong(s); }   // stand-in for 0x006C8780
static void TransferBinaryIn(TestStream* stream, COpinionDeed** value)
{
    *value = new COpinionDeed(0, 0, C3DVector(), true, false, 0);
    (*value)->DeedType = ReadLong(stream);
    (*value)->DeedHandle = ReadLong(stream);
    C3DVector p; p.X = ReadFloat(stream); p.Y = ReadFloat(stream); p.Z = ReadFloat(stream);
    (*value)->Position = p;
    (*value)->FrameHappened = ReadLong(stream);
    (*value)->IsStillHappening = false;
    (*value)->IsCrimeReportedToVillage = ReadBool(stream);
    (*value)->IsCrime = ReadBool(stream);
    (*value)->ExtraData = ReadLong(stream);
    TransferBinaryInPointer(stream, &(*value)->PTarget);
}
int main()
{
    unsigned char bytes[38];
    PutLong(bytes, 6); PutLong(bytes + 4, 21); PutFloat(bytes + 8, 1.0f); PutFloat(bytes + 12, 2.0f); PutFloat(bytes + 16, 3.0f);
    PutLong(bytes + 20, 1234); bytes[24] = 1; bytes[25] = 0; PutLong(bytes + 26, 8); PutLong(bytes + 30, 99); PutLong(bytes + 34, 0);
    TestStream s; s.Init(bytes, 38, 38);
    COpinionDeed* d = 0;
    TransferBinaryIn(&s, &d);
    bool ok = d != 0 && d->DeedType == 6 && d->DeedHandle == 21 && std::fabs(d->Position.Z - 3.0f) < 1e-6f && std::fabs(d->Position.Y - 2.0f) < 1e-6f
        && d->FrameHappened == 1234 && !d->IsStillHappening && d->IsCrimeReportedToVillage && !d->IsCrime && d->ExtraData == 8 && d->PTarget.UID == 99;
    delete d;
    if (!ok) { std::printf("006c88d0_TEST FAIL\n"); return 1; }
    std::printf("006c88d0_TEST PASS\n");
    return 0;
}