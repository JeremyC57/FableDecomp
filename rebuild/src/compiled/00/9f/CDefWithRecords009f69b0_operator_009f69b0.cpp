// CDefWithRecords_009f69b0::operator=(const CDefWithRecords_009f69b0&) -- memberwise copy assignment of a 0x30-byte
// aggregate: a CArray of 0x34-byte records at +0 (its own out-of-line operator=), a long at +0xc, a four-dword
// POD block at +0x10 copied by its implicit assignment, then four more dwords at +0x20..+0x2c; returns *this.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F69B0, length 79 bytes.
// Callees: CArray<SRecord34>::operator= 0x009F66A0 (called on `this` itself: the array is the first member).
// The manifest label (CAugmentationDef::operator=) is propagated garbage: the PDB CAugmentationDef has no array at
// +0 and is 0x4c bytes. The 16-byte block is a plain four-dword POD member: its compiler-generated assignment is the
// `lea ecx,[edi+10h] ; lea edx,[esi+10h]` pair with four dword moves (a user memberwise operator= drops the leas).
template<class T> struct CArray {
    T* first; T* last; T* end_of_storage;
    CArray& operator=(const CArray& o);                  // 0x009F66A0
};

struct SRecord34 { unsigned char _body[0x34]; };

struct SQuad {
    long a; long b; long c; long d;
};

struct CDefWithRecords_009f69b0 {
    CArray<SRecord34> m_records;                         // +0
    long m_count;                                        // +0xc
    SQuad m_quad;                                        // +0x10
    long m_value0;                                       // +0x20
    long m_value1;                                       // +0x24
    long m_value2;                                       // +0x28
    long m_value3;                                       // +0x2c
    CDefWithRecords_009f69b0& operator=(const CDefWithRecords_009f69b0& o);
};

CDefWithRecords_009f69b0& CDefWithRecords_009f69b0::operator=(const CDefWithRecords_009f69b0& o)
{
    m_records = o.m_records;
    m_count = o.m_count;
    m_quad = o.m_quad;
    m_value0 = o.m_value0;
    m_value1 = o.m_value1;
    m_value2 = o.m_value2;
    m_value3 = o.m_value3;
    return *this;
}