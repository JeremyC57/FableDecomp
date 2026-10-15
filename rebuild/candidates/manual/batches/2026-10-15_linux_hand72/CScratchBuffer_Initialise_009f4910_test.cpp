#include <cstdio>
// Behaviour model of 0x009F4910: CScratchBuffer::Initialise (base Init, allocation, size).
static bool g_initOk = true;
struct CInitBaseClass {
    virtual ~CInitBaseClass() {}
    bool Valid;
    bool Init() { Valid = true; return g_initOk; }
};
struct CScratchBuffer : CInitBaseClass {
    unsigned char* Buffer; bool Locked; long NoBytes;
    bool Initialise(long noBytes)
    {
        Buffer = 0;
        if (Init()) {
            Buffer = new unsigned char[noBytes];
            if (Buffer) { NoBytes = noBytes; Locked = false; return true; }
        }
        return false;
    }
};
int main()
{
    CScratchBuffer b; b.Buffer = (unsigned char*)&b; b.Locked = true; b.NoBytes = -1; b.Valid = false;
    g_initOk = false;
    if (b.Initialise(16) || b.Buffer != 0 || b.NoBytes != -1 || !b.Locked) { std::printf("009f4910_TEST FAIL\n"); return 1; }
    g_initOk = true;
    if (!b.Initialise(64) || b.Buffer == 0 || b.NoBytes != 64 || b.Locked || !b.Valid) { std::printf("009f4910_TEST FAIL\n"); return 1; }
    b.Buffer[63] = 7;
    delete[] b.Buffer;
    std::printf("009f4910_TEST PASS\n");
    return 0;
}
