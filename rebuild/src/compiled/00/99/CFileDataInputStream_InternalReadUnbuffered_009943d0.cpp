// CFileDataInputStream::InternalReadUnbuffered(unsigned char* buffer, long length) -- seek the source (+0x18) to the current position (virtual slot 2) and read the bytes straight into the caller's buffer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009943D0, length 39 bytes.
// Callees: virtual slot 2 (+8) of `this` (GetPosition), the source's Seek (slot 5) and Read (slot 3: buffer, length, 0). Shape: the two arguments are re-read from
// the stack for the pushes; `ret 8`.
struct CSourceStream_00994360 {                   // the underlying file: slot 3 (+0xc) Read(buffer, length, flags), slot 5 (+0x14) Seek(position)
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void Read(unsigned char* buffer, long length, long flags);   // slot 3 (+0x0c)
    virtual void v04();
    virtual void Seek(long position);              // slot 5 (+0x14)
};

struct CFileDataInputStream {
    virtual void v00(); virtual void v01();
    virtual long GetPosition();                    // slot 2 (+0x08)
    unsigned char           _pad04[0x14];
    CSourceStream_00994360* m_pSource;             // +0x18
    void InternalReadUnbuffered(unsigned char* buffer, long length);
};

void CFileDataInputStream::InternalReadUnbuffered(unsigned char* buffer, long length)
{
    long position = GetPosition();
    m_pSource->Seek(position);
    m_pSource->Read(buffer, length, 0);
}