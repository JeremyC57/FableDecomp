// CVectorMap_NamedRecord16::Add(const CCharString&, const SRecord16&) -- CVectorMap<CCharString, 16-byte record>: appends a {name, record} pair to the vector at +0 and marks the map dirty (+0xd)
// Leaf: Add
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A72B90, length 83 bytes.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, the pair vector's push_back @ 0x00A72690,
// CCharString::~CCharString @ 0x0099EAE0.
// Label correction: the manifest "CTCSkeletalMorph::AddSkeletalMorph" is propagated (see 0x006FCF40 for the CVectorMap
// family: vector at +0, empty Compare +0xc, Dirty +0xd).
// Shape: the 20-byte pair temporary is built by an inline pair constructor: the out-of-line CCharString copy, then the
// 16-byte value copied dword by dword through ecx/edx (memberwise user copy constructor over dword members); inner
// scope so the string destructor precedes the Dirty store. The manifest names the push_back callee
// "CScriptModuleFullTransition::OnInitLeaf" (propagated).
struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);   // 0x0099EC30
    ~CCharString();                          // 0x0099EAE0
};

struct SRecord16 {                           // 16-byte value with a memberwise user copy constructor
    long m_word0, m_word4, m_word8, m_wordC;
    SRecord16(const SRecord16& other) : m_word0(other.m_word0), m_word4(other.m_word4), m_word8(other.m_word8), m_wordC(other.m_wordC) { }
};
struct SNamedRecord16Pair {
    CCharString first;
    SRecord16 second;
    SNamedRecord16Pair(const CCharString& key, const SRecord16& value) : first(key), second(value) { }
};

struct SNamedRecord16PairVector {
    SNamedRecord16Pair* m_pFirst;
    SNamedRecord16Pair* m_pLast;
    SNamedRecord16Pair* m_pEnd;
    void PushBack(const SNamedRecord16Pair& pair);   // 0x00A72690
};

struct CVectorMap_NamedRecord16 {
    SNamedRecord16PairVector m_vector;               // +0x00
    unsigned char Compare;                   // +0x0c (empty CKeyPairCompareLess)
    bool          Dirty;                     // +0x0d
    void Add(const CCharString& key, const SRecord16& value);
};

void CVectorMap_NamedRecord16::Add(const CCharString& key, const SRecord16& value)
{
    {
        SNamedRecord16Pair pair(key, value);
        m_vector.PushBack(pair);
    }
    Dirty = true;
}