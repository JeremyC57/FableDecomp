// Behaviour test (0x006782b0): CPersistTraits<list<CReactionMatch*>>::TransferBinaryIn reads the count, clears the list, then per entry the type, creates that match class (unknown types create nothing), lets it read itself and appends it
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
struct CReactionMatch {
    int Type; int Payload;
    CReactionMatch(int type) : Type(type), Payload(0) {}
    virtual ~CReactionMatch() {}
    virtual void TransferBinaryIn(TestStream& stream) { int v; stream.Read(&v, 4); Payload = v; }
};
static CReactionMatch* CreateFromType(int type)
{
    switch (type) {
    case 0: return new CReactionMatch(0);
    case 1: return new CReactionMatch(1);
    case 2: return new CReactionMatch(2);
    case 3: return new CReactionMatch(3);
    }
    return 0;
}
struct CMatchList {                                      // list<CReactionMatch*> stand-in
    CReactionMatch* Items[8]; int Size;
    void clear() { Size = 0; }
    void push_back(CReactionMatch* m) { Items[Size++] = m; }
};
static void TransferBinaryIn(TestStream& stream, CMatchList& value)
{
    int count; { int n; stream.Read(&n, 4); count = n; }
    value.clear();
    for (int i = 0; i < count; ++i) {
        int type; { int n; stream.Read(&n, 4); type = n; }
        CReactionMatch* match = CreateFromType(type);
        if (match != 0) {
            match->TransferBinaryIn(stream);
            value.push_back(match);
        }
    }
}
int main()
{
    unsigned char bytes[28];
    PutLong(bytes, 3); PutLong(bytes + 4, 2); PutLong(bytes + 8, 55); PutLong(bytes + 12, 9); PutLong(bytes + 16, 3); PutLong(bytes + 20, 66);
    TestStream s; s.Init(bytes, 24, 24);
    CMatchList list; list.Size = 1; list.Items[0] = 0;
    TransferBinaryIn(s, list);
    bool ok = list.Size == 2 && list.Items[0]->Type == 2 && list.Items[0]->Payload == 55 && list.Items[1]->Type == 3 && list.Items[1]->Payload == 66;
    for (int i = 0; i < list.Size; ++i) delete list.Items[i];
    if (!ok) { std::printf("006782b0_TEST FAIL\n"); return 1; }
    std::printf("006782b0_TEST PASS\n");
    return 0;
}