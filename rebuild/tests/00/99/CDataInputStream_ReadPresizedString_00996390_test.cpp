// behaviour test for 00996390 CDataInputStream::ReadPresizedString (host g++/VC)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct CCharString { char* s; CCharString(const char* t, long n = -1) { s = strdup(t); } CCharString(const CCharString& o) { s = strdup(o.s); } ~CCharString() { free(s); } };
struct CByteArray { unsigned char* b; unsigned char* e; unsigned char* c; CByteArray(unsigned long n) { b = (unsigned char*)calloc(n ? n : 1, 1); e = b + n; c = e; } ~CByteArray() { free(b); }
    unsigned char* GetData() { return e - b != 0 ? b : (unsigned char*)this; } unsigned char& operator[](long i) { return b[i]; } };
struct Stream {
    unsigned long StreamPos; long StreamSize; void* CurrentSourcePtr; unsigned long SourceChunkStreamPos; long SourceChunkBytesRemaining;
    int overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { ++overflowCalls; memset(buffer, 0, size); }
    static bool IsValidPosition(long long p) { return p >= 0 && p <= 0x7fffffff; }
    void Read(void* buffer, long size) {
        if (size <= 0) return;
        long long np = (long long)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (size <= SourceChunkBytesRemaining) { memcpy(buffer, CurrentSourcePtr, size); CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size; SourceChunkBytesRemaining -= size; StreamPos += size; }
        else ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    long ReadLong() { long v = 0; Read(&v, 4); return v; }
    CCharString ReadPresizedString() {
        long length = ReadLong();
        if (length) { CByteArray buffer(length + 1); Read(buffer.GetData(), length); buffer[length] = 0; CCharString result((const char*)buffer.GetData(), -1); return result; }
        else return CCharString("", -1);
    }
};
int main() {
    unsigned char data[16] = { 5, 0, 0, 0, 'h', 'e', 'l', 'l', 'o', 0, 0, 0, 0, 9, 9, 9 };
    Stream s = { 0, 100, data, 0, 16, 0 };
    CCharString a = s.ReadPresizedString();
    CCharString b = s.ReadPresizedString();
    bool ok = strcmp(a.s, "hello") == 0 && strcmp(b.s, "") == 0 && s.StreamPos == 13 && s.SourceChunkBytesRemaining == 3 && s.overflowCalls == 0;
    s.SourceChunkBytesRemaining = 2;                       // the count no longer fits the chunk: the overflow path
    CCharString c = s.ReadPresizedString();
    ok = ok && s.overflowCalls == 1 && strcmp(c.s, "") == 0;
    printf(ok ? "00996390_TEST PASS\n" : "00996390_TEST FAIL\n");
    return ok ? 0 : 1;
}