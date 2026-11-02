// Behaviour test for 0x009F4950 CScratchBuffer::Uninitialise: the block is freed and cleared only while the buffer is valid,
// and the base uninitialisation always runs last.
#include <cstdio>
class CInitBaseClass {
public:
    CInitBaseClass() : Valid(false), UninitCalls(0) { }
    virtual ~CInitBaseClass() { }
    bool IsValid() const { return Valid; }
    bool Initialise() { Valid = true; return true; }
    void Uninitialise() { Valid = false; ++UninitCalls; }
    bool Valid;
    int  UninitCalls;
};
static int g_deletes = 0;
class CScratchBuffer : public CInitBaseClass {
public:
    CScratchBuffer() : Buffer(0), Locked(false), NoBytes(0) { }
    bool Initialise(long noBytes)
    {
        Buffer = 0;
        if (CInitBaseClass::Initialise()) {
            Buffer = new unsigned char[noBytes];
            if (Buffer) { NoBytes = noBytes; Locked = false; return true; }
        }
        return false;
    }
    void Uninitialise()
    {
        if (IsValid()) {
            if (Buffer) {
                delete[] Buffer;
                ++g_deletes;
                Buffer = 0;
            }
        }
        CInitBaseClass::Uninitialise();
    }
    unsigned char* Buffer;
    bool           Locked;
    long           NoBytes;
};
int main()
{
    CScratchBuffer b;
    if (!b.Initialise(64) || b.Buffer == 0 || !b.IsValid()) { std::printf("009f4950_TEST FAIL\n"); return 1; }
    b.Uninitialise();
    if (b.Buffer != 0 || g_deletes != 1 || b.IsValid() || b.UninitCalls != 1) { std::printf("009f4950_TEST FAIL\n"); return 1; }
    // not valid any more: nothing freed, but the base still runs
    unsigned char marker = 0;
    b.Buffer = &marker;
    b.Uninitialise();
    if (b.Buffer != &marker || g_deletes != 1 || b.UninitCalls != 2) { std::printf("009f4950_TEST FAIL\n"); return 1; }
    std::printf("009f4950_TEST PASS\n");
    return 0;
}