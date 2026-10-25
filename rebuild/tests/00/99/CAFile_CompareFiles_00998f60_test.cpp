// Behaviour test for 0x00998F60 CAFile::CompareFiles: equal files compare true, a length mismatch or a byte difference
// (also one past the first 4 KB block) compares false, and both files are rewound before comparing.
#include <cstdio>
#include <cstdlib>
#include <cstring>

class CAFile {
public:
    unsigned char* Data; long Length; long Position;
    CAFile(long n, unsigned char seed) : Data((unsigned char*)std::malloc(n ? n : 1)), Length(n), Position(n)
    {
        for (long i = 0; i < n; ++i) Data[i] = (unsigned char)(seed + i * 7);
    }
    ~CAFile() { std::free(Data); }
    void Read(void* dst, long size, void*) { std::memcpy(dst, Data + Position, size); Position += size; }
    void SetPosition(long p) { Position = p; }
    long GetLength() const { return Length; }
};

static bool CompareFiles(CAFile& file1, CAFile& file2)
{
    file1.SetPosition(0);
    file2.SetPosition(0);
    if (file1.GetLength() != file2.GetLength())
        return false;
    long bufferSize = 0x1000;
    unsigned char* buffer1 = (unsigned char*)std::calloc(bufferSize, 1);
    unsigned char* buffer2 = (unsigned char*)std::calloc(bufferSize, 1);
    long remaining = file1.GetLength();
    bool same = true;
    while (remaining > 0) {
        long chunk = remaining < bufferSize ? remaining : bufferSize;
        file1.Read(buffer1, chunk, 0);
        file2.Read(buffer2, chunk, 0);
        if (std::memcmp(buffer1, buffer2, chunk) != 0) { same = false; break; }
        remaining -= chunk;
    }
    std::free(buffer2);
    std::free(buffer1);
    return same;
}

int main()
{
    int ok = 1;
    CAFile a(10000, 3), b(10000, 3), c(9999, 3), d(10000, 3), e(0, 0), f(0, 0);
    d.Data[9000] ^= 0x40;                                  // differs in the third block
    ok &= CompareFiles(a, b);
    ok &= a.Position == 10000 && b.Position == 10000;      // read from the start to the end
    ok &= !CompareFiles(a, c);
    ok &= c.Position == 0;                                 // rewound, nothing read
    ok &= !CompareFiles(a, d);
    ok &= CompareFiles(e, f);                              // empty files are equal
    std::printf("00998f60_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}