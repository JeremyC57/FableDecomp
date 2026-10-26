// CNameTable_0055e7c0::GetEntryName(const unsigned long&) -- returns (by value, through the hidden result pointer) the CCharString at the head of the 16-byte entry selected by the index in the array at +0x28
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055E7C0, length 33 bytes.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// The manifest label (C3DBoundingSphere::AccurateBuildFromPoints) is propagated and does not fit:
// the body scales a by-reference index by 16, adds the array base at +0x28 and copy-constructs a
// CCharString from the element's first member into the hidden result (ret 8 = hidden pointer +
// one reference). The class and the entry are named by what the body does.
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other); // 0x0099EC30
};

struct SNamedEntry {                       // 16 bytes
    CCharString m_name;                    // +0
    long m_value[3];                       // +4
};

struct CNameTable_0055e7c0 {
    unsigned char _pad[0x28];
    SNamedEntry* m_pEntries;               // +0x28
    CCharString GetEntryName(const unsigned long& index);
};

CCharString CNameTable_0055e7c0::GetEntryName(const unsigned long& index)
{
    return m_pEntries[index].m_name;
}