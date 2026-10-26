// CVectorMap_NamedEntry::Add(const CCharString&, const SNamedEntry&) -- CVectorMap<CCharString, {long, long, CCharString}>: appends a {name, entry} pair to the vector at +0 and marks the map dirty (+0xd)
// Leaf: Add
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF2910, length 91 bytes (+5 int3 padding).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, the pair vector's push_back @ 0x00AF39C0,
// CCharString::~CCharString @ 0x0099EAE0.
// Label correction: the manifest "CTCSkeletalMorph::AddSkeletalMorph" is propagated (see 0x006FCF40 for the CVectorMap
// family: vector at +0, empty Compare +0xc, Dirty +0xd). 0x00AF2910 and 0x00AF3A40 are byte-identical apart from the
// call displacement (the same instantiation emitted in two translation units, both calling 0x00AF39C0).
// Shape: the 20-byte pair temporary is built by an inline pair constructor: the out-of-line CCharString key copy, then
// the entry's implicit copy constructor (two dwords through ecx/edx, the argument advanced by 8 for its own CCharString
// copy); inner scope so the two string destructors (entry first, key second) precede the Dirty store.
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);   // 0x0099EC30
    ~CCharString();                          // 0x0099EAE0
};

struct SNamedEntry {
    long        m_word0;
    long        m_word4;
    CCharString m_name;
};
struct SNamedEntryPair {
    CCharString first;
    SNamedEntry second;
    SNamedEntryPair(const CCharString& key, const SNamedEntry& value) : first(key), second(value) { }
};

struct SNamedEntryPairVector {
    SNamedEntryPair* m_pFirst;
    SNamedEntryPair* m_pLast;
    SNamedEntryPair* m_pEnd;
    void PushBack(const SNamedEntryPair& pair);   // 0x00AF39C0
};

struct CVectorMap_NamedEntry {
    SNamedEntryPairVector m_vector;               // +0x00
    unsigned char Compare;                   // +0x0c (empty CKeyPairCompareLess)
    bool          Dirty;                     // +0x0d
    void Add(const CCharString& key, const SNamedEntry& value);
};

void CVectorMap_NamedEntry::Add(const CCharString& key, const SNamedEntry& value)
{
    {
        SNamedEntryPair pair(key, value);
        m_vector.PushBack(pair);
    }
    Dirty = true;
}