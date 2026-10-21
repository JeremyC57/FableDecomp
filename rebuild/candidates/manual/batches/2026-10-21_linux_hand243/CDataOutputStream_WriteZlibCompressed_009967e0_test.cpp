// behaviour test for 009967e0 CDataOutputStream::WriteZlibCompressed (host g++/VC): a fake compressor that copies the input in chunks
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct CCompressorZlib {
    const unsigned char* src; long left; int finishCalls;
    CCompressorZlib() : src(0), left(0), finishCalls(0) {}
    void SetStreamInput(const void* s, long n) { src = (const unsigned char*)s; left = n; }
    bool HasMoreInput() { return left > 0; }
    long CompressStream(unsigned char* dest, long destSize) { long n = left < destSize ? left : destSize; memcpy(dest, src, n); src += n; left -= n; return n; }
    long FinishCompressStream(unsigned char* dest, long destSize, bool* finished) { ++finishCalls; if (finishCalls < 3) { dest[0] = 'Z'; *finished = false; return 1; } *finished = true; return 0; }
};
struct CByteArray { unsigned char* b; unsigned char* e; unsigned char* c; CByteArray(unsigned long n) { b = (unsigned char*)calloc(n, 1); e = b + n; c = e; } ~CByteArray() { free(b); }
    unsigned char* GetData() { return e - b != 0 ? b : (unsigned char*)this; } };
struct CDataOutputStream {
    unsigned char out[4096]; long pos; long lastChunk;
    CDataOutputStream() : pos(0), lastChunk(0) { memset(out, 0, sizeof out); }
    void SetPosition(long p) { pos = p; }
    long GetPosition() { return pos; }
    void WriteData(const void* d, unsigned long n) { memcpy(out + pos, d, n); pos += n; }
    void WriteLong(long value) { WriteData(&value, 4); }
    void WriteZlibCompressed(const void* data, long size, long bufferSize) {
        if (size > 0) {
            long sizePosition = GetPosition();
            WriteLong(0);
            if (bufferSize < 1024) bufferSize = 1024;
            else if (bufferSize > size + 1024) bufferSize = size + 1024;
            lastChunk = bufferSize;
            CByteArray buffer(bufferSize);
            CCompressorZlib compressor;
            compressor.SetStreamInput(data, size);
            long dataPosition = GetPosition();
            while (compressor.HasMoreInput()) { long written = compressor.CompressStream(buffer.GetData(), bufferSize); WriteData(buffer.GetData(), written); }
            bool finished = false;
            do { long written = compressor.FinishCompressStream(buffer.GetData(), bufferSize, &finished); WriteData(buffer.GetData(), written); } while (!finished);
            long endPosition = GetPosition();
            SetPosition(sizePosition);
            WriteLong(endPosition - dataPosition);
            SetPosition(endPosition);
        }
    }
};
int main() {
    CDataOutputStream s;
    unsigned char payload[2000]; for (int i = 0; i < 2000; ++i) payload[i] = (unsigned char)i;
    s.WriteZlibCompressed(payload, 2000, 10);              // clamp up to 1024
    long written; memcpy(&written, s.out, 4);
    bool ok = written == 2002 && s.pos == 2006 && memcmp(s.out + 4, payload, 2000) == 0 && s.out[2004] == 'Z' && s.out[2005] == 'Z' && s.lastChunk == 1024;
    CDataOutputStream t; t.WriteZlibCompressed(payload, 100, 100000); ok = ok && t.lastChunk == 1124 && t.pos == 106;
    CDataOutputStream u; u.WriteZlibCompressed(payload, 0, 5); ok = ok && u.pos == 0;
    printf(ok ? "009967e0_TEST PASS\n" : "009967e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
