// CReaderBase_00c4b300::Read(void* buffer, unsigned long size) -- read up to `size` bytes (clamped to what is left of the segment) through the underlying block read; returns the byte count, 0 at the end, or -1 after resetting on a failed read
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3A9E0, 73 bytes (row padded to 80), `ret 8`.
// Label: the manifest's "NScript::CV_MurderTwistScript::Read" is propagated (as is "CV_MurderTwistScript::Init" on its reset
// callee): the body belongs to the stream-reader module next to CBufferedReader_00c3aac0 (landed ctor 0x00C3AAC0, whose
// reset @ 0x00C3A9C0 zeroes the same +0x70/+0x74/+0x78 words). Callers: a forwarder @ 0x00C2DA00 (`jmp`) and the parser
// @ 0x00C2DAB0.. (`if (Read(&header, 8) < 8)`, a signed long result). Names are descriptive; the class follows the landed ctor.
// Callees: the block read @ 0x00C4B890 (thiscall, (buffer, size), bool), Reset @ 0x00C3A9C0 (declared as in the landed ctor).
// Layout: segment size +0x74, position +0x78.
// Shape: the remaining count `size - position` is computed first; `if (size > remaining) size = remaining;` clamps the
// parameter in place (`cmp ; jbe ; mov`); three exits (0, -1 via `or eax,-1`, the count).
// Leaf: ?Read@CReaderBase_00c4b300
struct CReaderBase_00c4b300 {                      // vtable 0x012B8458 (no RTTI module)
    virtual void v00();
    unsigned char _body04[0x70 - 4];
    unsigned long m_segmentStart;                  // +0x70
    unsigned long m_segmentSize;                   // +0x74
    unsigned long m_position;                      // +0x78
    void Reset();                                  // 0x00C3A9C0
    bool ReadBlock(void* buffer, unsigned long size);   // 0x00C4B890
    long Read(void* buffer, unsigned long size);
};

long CReaderBase_00c4b300::Read(void* buffer, unsigned long size)
{
    unsigned long remaining = m_segmentSize - m_position;
    if (size > remaining)
        size = remaining;
    if (size == 0)
        return 0;
    if (!ReadBlock(buffer, size)) {
        Reset();
        return -1;
    }
    m_position += size;
    return size;
}