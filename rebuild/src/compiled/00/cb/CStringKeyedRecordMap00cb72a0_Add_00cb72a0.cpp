// CStringKeyedRecordMap00cb72a0::Add(const CCharString& key, const SKeyedRecordValue& value) -- append a {key, value} pair to the unsorted pair vector and flag it for re-sorting
// Leaf: Add
// Retail: Fable.exe 0x00CB72A0, 105 bytes (`ret 8`).
// Label correction: the catalog "CTCSkeletalMorph::AddSkeletalMorph" is propagated; the body is the add of a sorted
// pair-vector map (CKeyPairCompare family: pair vector at +0, needs-sort flag +0x0D, as the landed
// C3DAnimFile::AddKeyedChunk 0x00A737B0) whose element is the 24-byte {CCharString, long, long, CCharString, long, bool}
// record of the landed push_back 0x00CB7210. No direct callers or address references were found; the owner type is
// not identified, so it is named by address.
// Callees: CCharString copy ctor @ 0x0099EC30 (x2), the pair vector's push_back @ 0x00CB7210 (manifest
// "CParticleSkeletalMorphMap::EmplaceOrAppend" is synthetic), ~CCharString @ 0x0099EAE0 (x2).
// Shape: the 24-byte pair temporary is built by the pair ctor (key copy, then the value's memberwise copy with its
// string copy out of line), pushed by address and destroyed.
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
};
struct SKeyedRecordValue {                          // 20 bytes
    long        m_00;                               // +0x00
    long        m_04;                               // +0x04
    CCharString m_value;                            // +0x08
    long        m_0c;                               // +0x0c
    bool        m_10;                               // +0x10
};
struct SKeyedRecord {                               // 24 bytes (the 0x00CB7210 element)
    CCharString       Key;                          // +0x00
    SKeyedRecordValue Value;                        // +0x04
    SKeyedRecord(const CCharString& key, const SKeyedRecordValue& value) : Key(key), Value(value) { }
};
struct CKeyedRecordVector {
    SKeyedRecord* m_pFirst;                         // +0x00
    SKeyedRecord* m_pLast;                          // +0x04
    SKeyedRecord* m_pEnd;                           // +0x08
    void push_back(const SKeyedRecord& record);     // 0x00CB7210
};
struct CStringKeyedRecordMap00cb72a0 {
    CKeyedRecordVector m_Pairs;                     // +0x00
    unsigned char      m_flag0C;                    // +0x0c
    bool               m_NeedsSort;                 // +0x0d
    void Add(const CCharString& key, const SKeyedRecordValue& value);
};

void CStringKeyedRecordMap00cb72a0::Add(const CCharString& key, const SKeyedRecordValue& value)
{
    m_Pairs.push_back(SKeyedRecord(key, value));
    m_NeedsSort = true;
}