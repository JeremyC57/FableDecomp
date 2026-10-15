// Behaviour test for 0x009C3A60 CFileInstaller::AllocateBuffer.
#include <cstdio>
struct CBufferChunk { unsigned char* Mem; bool Used; bool IsInput; };
struct CBufferChunkArray {
    CBufferChunk* _M_start; CBufferChunk* _M_finish; CBufferChunk* _M_end_of_storage;
    CBufferChunk* begin() { return _M_start; }
    unsigned int size() { return (unsigned int)(_M_finish - _M_start); }
    CBufferChunk& operator[](unsigned int n) { return *(begin() + n); }
};
struct CFileInstaller {
    CBufferChunkArray BufferChunks; int AvailableBuffers; int AllocatedInputBuffers;
    CBufferChunk* AllocateBuffer(bool pending)
    {
        if (AvailableBuffers) {
            for (unsigned int i = 0; i < BufferChunks.size(); ++i) {
                if (!BufferChunks[i].Used) {
                    BufferChunks[i].Used = true;
                    BufferChunks[i].IsInput = pending;
                    --AvailableBuffers;
                    AllocatedInputBuffers += (pending ? 1 : 0);
                    return &BufferChunks[i];
                }
            }
        }
        return 0;
    }
};
static int Fail() { std::printf("009c3a60_TEST FAIL\n"); return 1; }
int main()
{
    CBufferChunk c[3];
    for (int i = 0; i < 3; ++i) { c[i].Mem = 0; c[i].Used = false; c[i].IsInput = false; }
    c[0].Used = true;
    CFileInstaller f; f.BufferChunks._M_start = c; f.BufferChunks._M_finish = c + 3; f.BufferChunks._M_end_of_storage = c + 3;
    f.AvailableBuffers = 0; f.AllocatedInputBuffers = 0;
    if (f.AllocateBuffer(true) != 0) return Fail();
    f.AvailableBuffers = 2;
    if (f.AllocateBuffer(true) != &c[1] || !c[1].Used || !c[1].IsInput || f.AvailableBuffers != 1 || f.AllocatedInputBuffers != 1) return Fail();
    if (f.AllocateBuffer(false) != &c[2] || c[2].IsInput || f.AvailableBuffers != 0 || f.AllocatedInputBuffers != 1) return Fail();
    f.AvailableBuffers = 1;
    if (f.AllocateBuffer(false) != 0) return Fail();
    std::printf("009c3a60_TEST PASS\n");
    return 0;
}
