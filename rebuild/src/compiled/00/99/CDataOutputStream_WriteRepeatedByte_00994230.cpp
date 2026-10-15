// CDataOutputStream::WriteRepeatedByte(unsigned char value, long count) -- write `count` copies of `value` to the stream, through a 256-byte stack buffer filled once and written in chunks of at most 256 bytes
// Leaf: WriteRepeatedByte
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00994230, length 151 bytes.
// Callees: the stream's virtual Write(const void*, long) (slot 4, +0x10), __security_check_cookie @ 0x00BFE9F9 (the /GS
// cookie 0x0139C8A8 guards the buffer).
// Shape: /GS frame with the buffer above the spilled `this`; the fill size and each chunk size are `count < 256 ? count : 256`
// (`cmp ; mov ; jl ; mov 100h`); memset is the intrinsic (byte replicated into eax, `rep stosd ; rep stosb`); the chunk loop
// is a top-tested `while (count > 0)` that VC rotates. engine/CDataOutputStream.h types the vptr as `void*`, so the
// polymorphic stream is modelled locally (vptr +0, NewLine +4).
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memset)

struct CCharStringData;
struct CDataOutputStream {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void Write(const void* data, long size);   // slot 4 (+0x10)
    CCharStringData* NewLine_PStringData;              // +0x04
    void WriteRepeatedByte(unsigned char value, long count);
};

void CDataOutputStream::WriteRepeatedByte(unsigned char value, long count)
{
    unsigned char buffer[256];
    memset(buffer, value, count < 256 ? count : 256);
    while (count > 0) {
        long size = count < 256 ? count : 256;
        Write(buffer, size);
        count -= size;
    }
}