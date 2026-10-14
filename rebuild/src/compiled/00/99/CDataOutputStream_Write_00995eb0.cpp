// CDataOutputStream::Write(const CByteBlock&) -- write a byte vector's contents: the data pointer (the vector object itself when it is empty) and its byte count through the virtual Write(const void*, long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00995EB0, length 42 bytes.
// Callees: virtual slot 4 Write(const void*, long) on `this`.
// Shape: the block holds a VC6-era vector<unsigned char> at +8 ({first, last, end}); the data pointer comes from an
// inline accessor that returns the first element's address, or the vector's own address when it is empty
// (`sub esi,edx ; jne ; mov edx,eax`), and the length is `last - first` re-read after the test. The record's
// first two words are not touched.
struct CRetailByteVector {                     // VC6 Dinkumware vector<unsigned char>: first, last, end
    unsigned char* m_pFirst;
    unsigned char* m_pLast;
    unsigned char* m_pEnd;
    long GetCount() const { return m_pLast - m_pFirst; }
    const void* GetData() const { return GetCount() != 0 ? (const void*)m_pFirst : (const void*)this; }
};

struct CByteBlock {
    long              m_type;                  // +0x00
    long              m_id;                    // +0x04
    CRetailByteVector m_bytes;                 // +0x08
};

struct CDataOutputStream {                     // engine/CDataOutputStream.h: vptr +0, NewLine +4
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, long length);   // slot 4
    void* NewLine_PStringData;                 // +0x04
    void Write(const CByteBlock& block);
};

void CDataOutputStream::Write(const CByteBlock& block)
{
    Write(block.m_bytes.GetData(), block.m_bytes.GetCount());
}