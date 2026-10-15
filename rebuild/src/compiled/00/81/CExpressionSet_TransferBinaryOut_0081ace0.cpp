// CExpressionSet::TransferBinaryOut(CDataOutputStream& stream) -- write the element count, the raw 24-byte elements and the end marker
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081ACE0, 102 bytes (+ int3 padding).
// Callees: CDataOutputStream::WriteLong @ 0x00993EB0 (x2; the manifest "WriteCBYTE" writes 4 bytes),
//          CDataOutputStream::Write(const void*, long) virtual slot 4 (`call [edx+10h]`).
// Layout: `this` is a three-pointer vector (first +0, last +4); the elements are 44 bytes
//   (the 0x2AAAAAAB / sar 2 reciprocal multiply is a signed divide by 24).
// Shape: the element count is computed twice -- once for the leading WriteLong and once (re-reading both pointers
//   after the call) for the data block; the data pointer is the CSmallVector-style `count ? first : this` select
//   written into the dead this-register; 0x01AB6CBB (outside the image) is a constant end marker, not an address.
//   The same template body is instantiated for 24-, 40- and 12-byte elements at 0x00757500 (44), 0x0081ACE0 (24), 0x0081AD50 (40), 0x0086F590 (12).
struct CDataOutputStream {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void Write(const void* data, long bytes);  // slot 4
    void WriteLong(long value);                        // 0x00993EB0
};

struct SRecord24 { long m_words[6]; };                // 24-byte element

template <class T> struct CRecordVector {
    T* m_first;                                        // +0x00
    T* m_last;                                         // +0x04
    T* m_end;                                          // +0x08
    long size() const { return m_last - m_first; }
    const void* GetData(long count) const { return count ? (const void*)m_first : (const void*)this; }
    void TransferBinaryOut(CDataOutputStream& stream) const
    {
        stream.WriteLong(size());
        long count = size();
        stream.Write(GetData(count), count * sizeof(T));
        stream.WriteLong(0x01AB6CBB);
    }
};

struct CExpressionSet : CRecordVector<SRecord24> {
    void TransferBinaryOut(CDataOutputStream& stream) const;
};

void CExpressionSet::TransferBinaryOut(CDataOutputStream& stream) const
{
    CRecordVector<SRecord24>::TransferBinaryOut(stream);
}