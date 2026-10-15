// CKeyedRecordOwner0088b910::FindLastRecordBelow(long key) -- scan the 12-byte keyed records at +0x94 and return the last one whose (signed) key is below `key`, or null
// Leaf: FindLastRecordBelow
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088B910, 101 bytes (+ int3 padding).
// Label correction: the manifest's "CEngineWorldMap::BuildAndSaveSea(CStaticMapGenerationVisibiilityInfo*)" is propagated
//   (the same label sits on 0x00D77340, an unrelated body); this one is a pure search returning a record pointer. The owner
//   class is not recoverable from the body, so it is named by address.
// Callees: none.
// Layout: a retail CArray (begin +0x94, end +0x98, capacity) of 12-byte records whose first dword is the key.
// Shape: the record count `(end - begin) / 12` (0x2AAAAAAB, sar 1) is re-derived every pass and compared unsigned; the
//   element walk is strength-reduced to a pointer stepping 12; `jge` skips, so the test is a signed `key < limit`.
struct SKeyedRecord {                            // 12 bytes
    long m_key;                                  // +0
    long m_a;                                    // +4
    long m_b;                                    // +8
};

template <class T> struct CArray {               // retail CArray: begin, end, capacity
    T* m_pBegin;
    T* m_pEnd;
    T* m_pCapacity;
    unsigned long Size() const { return m_pEnd - m_pBegin; }
    const T& operator[](unsigned long i) const { return m_pBegin[i]; }
};

struct CKeyedRecordOwner0088b910 {
    unsigned char        _pad0[0x94];
    CArray<SKeyedRecord> m_records;              // +0x94
    const SKeyedRecord* FindLastRecordBelow(long key) const;
};

const SKeyedRecord* CKeyedRecordOwner0088b910::FindLastRecordBelow(long key) const
{
    const SKeyedRecord* found = 0;
    for (unsigned long i = 0; i < m_records.Size(); ++i)
        if (m_records[i].m_key < key)
            found = &m_records[i];
    return found;
}