// CFileDataInputStream::InternalReadNextSourceChunk(const unsigned char*& chunk, long& length) -- the next chunk is the smaller of the chunk size (+0x20) and what remains to the end (+8) from the current position (virtual slot 2); seek the source (+0x18) there, read that many bytes into the buffer (+0x1c) and hand the buffer back
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00994360, length 69 bytes.
// Callees: virtual slot 2 (+8) of `this` (GetPosition), the source's Seek (slot 5) and Read (slot 3: buffer, length, 0). Shape: the unsigned minimum (`cmp edx,ecx ;
// jae`), the length stored through the reference before the seek and re-read for the read; `ret 8`.
struct CSourceStream_00994360 {                   // the underlying file: slot 3 (+0xc) Read(buffer, length, flags), slot 5 (+0x14) Seek(position)
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void Read(unsigned char* buffer, long length, long flags);   // slot 3 (+0x0c)
    virtual void v04();
    virtual void Seek(long position);              // slot 5 (+0x14)
};

class CFileDataInputStream { public:
    virtual void v00(); virtual void v01();
    virtual long GetPosition();                    // slot 2 (+0x08)
    unsigned char           _pad04[4];
    unsigned long           m_end;                 // +0x08
    unsigned char           _pad0c[0xc];
    CSourceStream_00994360* m_pSource;             // +0x18
    unsigned char*          m_pBuffer;             // +0x1c
    unsigned long           m_chunkSize;           // +0x20
    void InternalReadNextSourceChunk(const unsigned char*& chunk, long& length);
};

void CFileDataInputStream::InternalReadNextSourceChunk(const unsigned char*& chunk, long& length)
{
    long position = GetPosition();
    unsigned long remaining = m_end - position;
    if (m_chunkSize < remaining)
        remaining = m_chunkSize;
    length = remaining;
    m_pSource->Seek(position);
    m_pSource->Read(m_pBuffer, length, 0);
    chunk = m_pBuffer;
}