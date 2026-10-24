// Behaviour test for CPersistTraits<CObjectFamilyEntry>::TransferBinaryIn @ 0x0046C3B9 (src/0046c3b9.cpp): a memory
// stream stand-in with the inline Read (fast path + chunk-overflow slow path), the long traits reader and the entry reader.
#include <cstdio>
#include <cstring>
struct CDataInputStream {
    long           StreamPos;
    unsigned char* CurrentSourcePtr;
    long           SourceChunkBytesRemaining;
    const unsigned char* NextChunk;
    long           NextChunkSize;
    long           Overflows;
};
struct CDataInputStreamReader : CDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        // copy what is left, then continue from the next chunk
        long head = SourceChunkBytesRemaining;
        std::memcpy(buffer, CurrentSourcePtr, head);
        std::memcpy(buffer + head, NextChunk, size - head);
        CurrentSourcePtr = (unsigned char*)NextChunk + (size - head);
        SourceChunkBytesRemaining = NextChunkSize - (size - head);
        StreamPos += size;
        ++Overflows;
    }
    static bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (SourceChunkBytesRemaining >= size) {
            std::memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
};
struct CObjectFamilyEntry { long Object; long Probability; };
template<class T> struct CPersistTraits;
template<> struct CPersistTraits<long> {
    long DefaultVal;
    CPersistTraits() {}
    void TransferBinaryIn(CDataInputStream* stream, long* value) { ((CDataInputStreamReader*)stream)->Read(value, sizeof(long)); }
};
template<> struct CPersistTraits<CObjectFamilyEntry> {
    void TransferBinaryIn(CDataInputStream* stream, CObjectFamilyEntry* value);
};
// the reconstruction's logic
void CPersistTraits<CObjectFamilyEntry>::TransferBinaryIn(CDataInputStream* stream, CObjectFamilyEntry* value)
{
    CPersistTraits<long>().TransferBinaryIn(stream, &value->Object);
    value->Probability = (long)((CDataInputStreamReader*)stream)->ReadFloat();
}
int main()
{
    // chunk 1: Object = 0x1234, then the first 2 bytes of the float; chunk 2: the last 2 bytes of 37.9f (truncates to 37)
    float probability = 37.9f;
    unsigned char chunk1[6]; unsigned char chunk2[2];
    long object = 0x1234;
    std::memcpy(chunk1, &object, 4); std::memcpy(chunk1 + 4, &probability, 2); std::memcpy(chunk2, (unsigned char*)&probability + 2, 2);
    CDataInputStreamReader stream;
    stream.StreamPos = 100; stream.CurrentSourcePtr = chunk1; stream.SourceChunkBytesRemaining = 6;
    stream.NextChunk = chunk2; stream.NextChunkSize = 2; stream.Overflows = 0;
    CObjectFamilyEntry entry = { -1, -1 };
    CPersistTraits<CObjectFamilyEntry> traits;
    traits.TransferBinaryIn(&stream, &entry);
    bool ok = entry.Object == 0x1234 && entry.Probability == 37 && stream.StreamPos == 108 && stream.Overflows == 1 && stream.SourceChunkBytesRemaining == 0;
    // a second entry in one chunk, negative probability truncates toward zero
    float p2 = -2.75f; long o2 = 7; unsigned char chunk3[8];
    std::memcpy(chunk3, &o2, 4); std::memcpy(chunk3 + 4, &p2, 4);
    stream.CurrentSourcePtr = chunk3; stream.SourceChunkBytesRemaining = 8;
    traits.TransferBinaryIn(&stream, &entry);
    ok = ok && entry.Object == 7 && entry.Probability == -2 && stream.StreamPos == 116 && stream.Overflows == 1;
    // an invalid position (past 0x7fffffff) leaves the entry untouched
    stream.StreamPos = 0x7ffffffe; stream.CurrentSourcePtr = chunk3; stream.SourceChunkBytesRemaining = 8;
    entry.Object = 99; entry.Probability = 98;
    traits.TransferBinaryIn(&stream, &entry);
    ok = ok && entry.Object == 99;   // (the float read also skips: Probability is then an uninitialised conversion, as in retail)
    if (!ok) { std::printf("0046c3b9_TEST FAIL %ld %ld %ld\n", entry.Object, entry.Probability, stream.StreamPos); return 1; }
    std::printf("0046c3b9_TEST PASS\n");
    return 0;
}