// CLongKeyedWideRecordMap006bf500::Add(const long& key, const SWideRecordValue& value) -- append a {key, value} pair to the unsorted pair vector and flag it for re-sorting
// Leaf: Add
// Retail: Fable.exe 0x006BF500, 105 bytes (`ret 8`).
// Label correction: the catalog "CTCSkeletalMorph::AddSkeletalMorph" is propagated; the body is the add of a sorted
// pair-vector map (CKeyPairCompare family: pair vector at +0, needs-sort flag +0x0D, as the landed
// C3DAnimFile::AddKeyedChunk 0x00A737B0) with a long key and a {CWideString, CWideString, long, long, bool} value
// (24-byte pair). No direct callers or address references were found; the owner type is not identified, so it is
// named by address.
// Callees: CWideString copy ctor @ 0x0099B720 (x2), the pair vector's push_back @ 0x006BF410 (manifest
// "Vector_PushBackMusicEntry"), ~CWideString @ 0x0099B510 (x2).
// Shape: the pair temporary copies the key word, then the value memberwise (two string copies out of line).
struct CWideString {
    void* m_pData;
    CWideString(const CWideString& other);          // 0x0099B720
    ~CWideString();                                 // 0x0099B510
};
struct SWideRecordValue {                           // 20 bytes
    CWideString m_text0;                            // +0x00
    CWideString m_text1;                            // +0x04
    long        m_08;                               // +0x08
    long        m_0c;                               // +0x0c
    bool        m_10;                               // +0x10
};
struct SWideRecord {                                // 24 bytes
    long             Key;                           // +0x00
    SWideRecordValue Value;                         // +0x04
    SWideRecord(const long& key, const SWideRecordValue& value) : Key(key), Value(value) { }
};
struct CWideRecordVector {
    SWideRecord* m_pFirst;                          // +0x00
    SWideRecord* m_pLast;                           // +0x04
    SWideRecord* m_pEnd;                            // +0x08
    void push_back(const SWideRecord& record);      // 0x006BF410
};
struct CLongKeyedWideRecordMap006bf500 {
    CWideRecordVector m_Pairs;                      // +0x00
    unsigned char     m_flag0C;                     // +0x0c
    bool              m_NeedsSort;                  // +0x0d
    void Add(const long& key, const SWideRecordValue& value);
};

void CLongKeyedWideRecordMap006bf500::Add(const long& key, const SWideRecordValue& value)
{
    m_Pairs.push_back(SWideRecord(key, value));
    m_NeedsSort = true;
}
