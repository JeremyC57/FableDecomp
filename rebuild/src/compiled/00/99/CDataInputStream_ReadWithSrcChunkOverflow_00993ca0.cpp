// CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size) -- slow path of the inline stream Read when the request straddles the current source chunk: drain what is left of the chunk, then either pull further chunks through InternalReadNextSourceChunk until the request is satisfied or, when NeedsBufferedRead says no, read the rest unbuffered and drop the chunk window
// Leaf: ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z (Ego_r.pdb, private non-virtual)
// Retail: Fable.exe @ 0x00993CA0, 351 bytes, `ret 8`, recursive through the inline Read's overflow branch.
// Body: remaining = SourceChunkBytesRemaining; > 0 -> copy it out (range-checked), buffer/size advanced. NeedsBufferedRead
// (vslot 9, +0x24) -> loop while size > 0 { SourceChunkStreamPos = StreamPos; InternalReadNextSourceChunk (vslot 8,
// +0x20)(CurrentSourcePtr, SourceChunkBytesRemaining); n = min(size, remaining); Read(buffer, n); advance }; else
// InternalReadUnbuffered (vslot 10, +0x28)(buffer, size); StreamPos += size; window cleared.
// Layout: offsets of engine/CDataInputStream.h (StreamPos +4, StreamSize +8, CurrentSourcePtr +0xC, SourceChunkStreamPos
// +0x10, SourceChunkBytesRemaining +0x14); the generated header models the vptr as `void*`, so this TU declares the
// class with its virtual slots (Ego_r.pdb order: InternalReadNextSourceChunk / NeedsBufferedRead /
// InternalReadUnbuffered are slots 8-10; slots 0-7 are placeholders) to dispatch through them.
// Names: CDataInputStream members from Ego_r.pdb.
// Shape: the inline Read is the one of the landed ReadZlibCompressed de-bake (0x00996230): size <= 0 guard, 64-bit
// position range check with a zero high word, `size <= SourceChunkBytesRemaining` chunk memcpy, else recurse. The drain is
// open-coded (not the inline Read): next/left locals computed before the range check give retail's hoisted buffer/size
// arithmetic and the zero-register high word; the unbuffered branch caches StreamPos in a local before the virtual call
// (retail loads [ebx+4] before `call [edx+28h]`). Probes: v/rc/a..n.
// Opt: /O2 (16-aligned entry).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream {
public:
    virtual void _slot0();
    virtual void _slot1();
    virtual void _slot2();
    virtual void _slot3();
    virtual void _slot4();
    virtual void _slot5();
    virtual void _slot6();
    virtual void _slot7();
private:
    virtual void InternalReadNextSourceChunk(const unsigned char*& sourcePtr, long& sourceBytes);   // +0x20
    virtual bool NeedsBufferedRead(long size);                                                     // +0x24
    virtual void InternalReadUnbuffered(unsigned char* buffer, long size);                         // +0x28
protected:
    unsigned long StreamPos;                                 // +0x04
    long StreamSize;                                         // +0x08
    const unsigned char* CurrentSourcePtr;                   // +0x0c
    unsigned long SourceChunkStreamPos;                      // +0x10
    long SourceChunkBytesRemaining;                          // +0x14

    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
public:
    __forceinline void Read(void* buffer, long size)
    {
        if (size <= 0)
            return;
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
private:
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);
};

void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
{
    long remaining = SourceChunkBytesRemaining;
    if (remaining > 0) {
        unsigned char* next = buffer + remaining;
        long left = size - remaining;
        if (IsValidPosition((__int64)(StreamPos + remaining))) {
            memcpy(buffer, CurrentSourcePtr, remaining);
            CurrentSourcePtr += remaining;
            SourceChunkBytesRemaining -= remaining;
            StreamPos += remaining;
        }
        buffer = next;
        size = left;
    }
    if (NeedsBufferedRead(size)) {
        while (size > 0) {
            SourceChunkStreamPos = StreamPos;
            InternalReadNextSourceChunk(CurrentSourcePtr, SourceChunkBytesRemaining);
            long chunk = size < SourceChunkBytesRemaining ? size : SourceChunkBytesRemaining;
            Read(buffer, chunk);
            buffer += chunk;
            size -= chunk;
        }
    }
    else {
        unsigned long position = StreamPos;
        InternalReadUnbuffered(buffer, size);
        StreamPos = position + size;
        SourceChunkBytesRemaining = 0;
        CurrentSourcePtr = 0;
        SourceChunkStreamPos = 0;
    }
}