// Behaviour test for 0x00516D90 CAFile::ReadVar<int>
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CASuspendableProcess {};
static int g_buffers = 0;
struct CByteVector {
    unsigned char* _First; unsigned char* _Last; unsigned char* _End;
    explicit CByteVector(unsigned long n) { _First = n ? (unsigned char*)std::malloc(n) : 0; std::memset(_First, 0, n); _Last = _End = _First + n; ++g_buffers; }
    ~CByteVector() { std::free(_First); --g_buffers; }
    unsigned char& operator[](unsigned long i) { return _First[i]; }
};
struct CAFile {
    long alignment; unsigned char source[16]; long lastSize; unsigned long maxBuffer;
    virtual void Read(void* buffer, long size, CASuspendableProcess*) { std::memcpy(buffer, source, size); lastSize = size; }
    virtual long GetAlignment() const { return alignment; }
    virtual ~CAFile() {}
    template<class T> void ReadVar(T* value, CASuspendableProcess* process)
    {
        if (GetAlignment() <= 1) { Read(value, sizeof(T), process); return; }
        unsigned long a = GetAlignment();
        unsigned long n = ((sizeof(T) + a - 1) / a) * a;
        maxBuffer = n;
        CByteVector buffer(n);
        Read(&buffer[0], sizeof(T), process);
        *value = *(T*)&buffer[0];
    }
};
int main()
{
    typedef int T;
    T want = (T)-77;
    CAFile f; std::memcpy(f.source, &want, sizeof(T)); f.lastSize = 0; f.maxBuffer = 0;
    CASuspendableProcess p;
    T got = 0; f.alignment = 1; f.ReadVar(&got, &p);
    bool ok = got == want && f.lastSize == (long)sizeof(T) && f.maxBuffer == 0;
    got = 0; f.alignment = 2048; f.ReadVar(&got, &p);
    ok = ok && got == want && f.lastSize == (long)sizeof(T) && f.maxBuffer == 2048 && g_buffers == 0;
    if (!ok) { std::printf("00516d90_TEST FAIL\n"); return 1; }
    std::printf("00516d90_TEST PASS\n");
    return 0;
}
