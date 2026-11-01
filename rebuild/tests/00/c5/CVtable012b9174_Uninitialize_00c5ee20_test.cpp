// Behaviour test for 0x00C5EE20 (CVtable_012b9174::Uninitialize, slot 2 of the silent audio output): it dispatches to the
// object's own ReleaseBuffer (slot 5), which for the silent output zeroes the block count.
#include <cstdio>

class COutput {
public:
    COutput() : BlockCount(0), Releases(0) {}
    virtual ~COutput() {}
    virtual bool Initialize(const void*, void*, bool) { return true; }
    virtual void Uninitialize() { ReleaseBuffer(); }    // the retail body
    virtual bool CreateAndPlay(unsigned long, unsigned long bytes) { BlockCount = bytes >> 7; return true; }
    virtual bool CreateLoopingBuffer(unsigned long) { return false; }
    virtual void ReleaseBuffer() { BlockCount = 0; ++Releases; }
    unsigned long BlockCount;
    int Releases;
};

class CCountingOutput : public COutput {                 // a derived override must be reached through the vtable
public:
    CCountingOutput() : Derived(0) {}
    virtual void ReleaseBuffer() { COutput::ReleaseBuffer(); ++Derived; }
    int Derived;
};

int main()
{
    COutput out;
    out.CreateAndPlay(2, 0x1000);
    if (out.BlockCount != 0x20) { std::printf("00c5ee20_TEST FAIL\n"); return 1; }
    out.Uninitialize();
    if (out.BlockCount != 0 || out.Releases != 1) { std::printf("00c5ee20_TEST FAIL\n"); return 1; }
    CCountingOutput derived;
    COutput* p = &derived;
    p->CreateAndPlay(1, 0x800);
    p->Uninitialize();
    if (derived.Derived != 1 || derived.BlockCount != 0) { std::printf("00c5ee20_TEST FAIL\n"); return 1; }
    std::printf("00c5ee20_TEST PASS\n");
    return 0;
}