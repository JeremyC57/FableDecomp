// CVectorMap_StringPairKey::Add(const SStringPair&, const long&) -- CVectorMap<{CCharString, CCharString}, long>: appends a {string-pair key, value} pair to the vector at +0 and marks the map dirty (+0xd)
// Leaf: Add
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A41A70, length 86 bytes (+10 int3 padding).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, the pair vector's push_back @ 0x00A41840,
// CCharString::~CCharString @ 0x0099EAE0.
// Label correction: the manifest "CTCSkeletalMorph::AddSkeletalMorph" is propagated (see 0x006FCF40 for the CVectorMap
// family: vector at +0, empty Compare +0xc, Dirty +0xd).
// Shape: the 12-byte pair temporary is built by an inline pair constructor: the key's implicit copy constructor (two
// out-of-line CCharString copies, the argument pointer kept in edi and advanced by 4), then the long value; inner
// scope so both string destructors (second first) precede the Dirty store. The manifest names the push_back callee
// "Vector_PushBack_CCharStringTriple".
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);   // 0x0099EC30
    ~CCharString();                          // 0x0099EAE0
};

struct SStringPair {
    CCharString m_first;
    CCharString m_second;
};
struct SStringPairKeyPair {
    SStringPair first;
    long second;
    SStringPairKeyPair(const SStringPair& key, const long& value) : first(key), second(value) { }
};

struct SStringPairKeyPairVector {
    SStringPairKeyPair* m_pFirst;
    SStringPairKeyPair* m_pLast;
    SStringPairKeyPair* m_pEnd;
    void PushBack(const SStringPairKeyPair& pair);   // 0x00A41840
};

struct CVectorMap_StringPairKey {
    SStringPairKeyPairVector m_vector;               // +0x00
    unsigned char Compare;                   // +0x0c (empty CKeyPairCompareLess)
    bool          Dirty;                     // +0x0d
    void Add(const SStringPair& key, const long& value);
};

void CVectorMap_StringPairKey::Add(const SStringPair& key, const long& value)
{
    {
        SStringPairKeyPair pair(key, value);
        m_vector.PushBack(pair);
    }
    Dirty = true;
}