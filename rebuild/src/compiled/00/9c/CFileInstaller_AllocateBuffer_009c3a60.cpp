// CFileInstaller::AllocateBuffer(bool pending) -- when a buffer is free (+0x30), find the first buffer record of the vector at +0x1c whose in-use byte is clear, mark it in use, record the pending flag, drop the free count, count it as pending (+0x34) when flagged, and return it; 0 when none is free
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C3A60, length 116 bytes (+ int3 padding). No callees.
// Layout: the 8-byte CBufferChunk records as the landed CFileInstaller::FreeBuffer 0x009C3330 (in-use byte +4, pending byte +5;
// FreeBuffer is this function's exact inverse); the retail STLport vector {first, last, end} at +0x1c; free count +0x30, pending count +0x34.
// Shape: `size()` is the signed pointer difference `sar 3` compared unsigned (`jbe`/`jb`), re-evaluated each iteration; the
// element is `*(begin() + i)` (the loop walks a strength-reduced pointer, the first store re-uses the hoisted begin, later uses
// re-read it after the intervening stores); `+= pending ? 1 : 0` gives `setne dl`.
struct CBufferChunk {
    unsigned char* Mem;
    bool Used;                                  // +0x04
    bool IsInput;                                // +0x05
};

struct CBufferChunkArray {                      // retail STLport vector<CBufferChunk>
    CBufferChunk* _M_start;
    CBufferChunk* _M_finish;
    CBufferChunk* _M_end_of_storage;
    CBufferChunk* begin() { return _M_start; }
    unsigned long size() { return _M_finish - _M_start; }
    CBufferChunk& operator[](unsigned long n) { return *(begin() + n); }
};

class CFileInstaller { public:
    unsigned char        _pad[0x1c];
    CBufferChunkArray BufferChunks;                // +0x1c
    unsigned char        _pad28[8];
    long                 AvailableBuffers;            // +0x30
    long                 AllocatedInputBuffers;         // +0x34
    CBufferChunk* AllocateBuffer(bool pending);
};

CBufferChunk* CFileInstaller::AllocateBuffer(bool pending)
{
    if (AvailableBuffers) {
        for (unsigned long i = 0; i < BufferChunks.size(); ++i) {
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