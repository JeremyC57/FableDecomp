#include <cstdio>
#include <cstring>
struct File { unsigned char data[256]; long pos; void Write(const void* d, long n, long) { std::memcpy(data + pos, d, n); pos += n; } void Seek(long p) { pos = p; } };
struct Stream {
    File* file; unsigned char* Buffer; long BufferSize, BufferDataSize, DataStartFilePtr, FileSize, FilePos; int flushes;
    void Flush() { ++flushes; file->Seek(DataStartFilePtr); file->Write(Buffer, BufferDataSize, 0); DataStartFilePtr = FilePos; BufferDataSize = 0; }
    void Write(const void* data, long size)
    {
        long bufferSize = BufferSize;
        if (size > bufferSize) {
            Flush(); file->Seek(FilePos); file->Write(data, size, 0); FilePos += size;
            if (FileSize < FilePos) FileSize = FilePos;
            file->Seek(FileSize); return;
        }
        long offset = FilePos - DataStartFilePtr;
        if (FilePos < DataStartFilePtr || offset + size > bufferSize || offset > BufferDataSize) { Flush(); offset = FilePos - DataStartFilePtr; }
        std::memcpy(Buffer + offset, data, size);
        FilePos += size;
        if (BufferDataSize < offset + size) BufferDataSize = offset + size;
        if (FileSize < FilePos) FileSize = FilePos;
    }
};
int main()
{
    File f; std::memset(f.data, 0, sizeof f.data); f.pos = 0;
    unsigned char buf[8];
    Stream s = { &f, buf, 8, 0, 0, 0, 0, 0 };
    s.Write("abcd", 4);
    s.Write("efgh", 4);
    bool ok = s.flushes == 0 && s.BufferDataSize == 8 && s.FilePos == 8 && s.FileSize == 8;
    s.Write("ij", 2);                                   // overflows: flush first
    ok = ok && s.flushes == 1 && s.DataStartFilePtr == 8 && s.BufferDataSize == 2 && s.FilePos == 10;
    s.Write("0123456789", 10);                          // oversize: direct
    ok = ok && s.flushes == 2 && s.FilePos == 20 && s.FileSize == 20 && f.pos == 20 && std::memcmp(f.data, "abcdefghij0123456789", 20) == 0;
    if (!ok) { std::printf("00994870_TEST FAIL\n"); return 1; }
    std::printf("00994870_TEST PASS\n");
    return 0;
}