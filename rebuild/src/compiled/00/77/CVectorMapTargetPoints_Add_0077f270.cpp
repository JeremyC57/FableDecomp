// CVectorMap_TargetPoints::Add(const CCharString&, const STargetPoint&) -- CVectorMap<CCharString, 12-byte record>: appends a {name, record} pair to the vector at +0 and marks the map dirty (+0xd)
// Leaf: Add
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077F270, length 76 bytes (+4 int3 padding).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, the pair vector's push_back @ 0x0077F130
// (manifest "Vector_PushBack_TargetPoint"), CCharString::~CCharString @ 0x0099EAE0.
// Label correction: the manifest "CTCSkeletalMorph::AddSkeletalMorph" is propagated (see 0x006FCF40 for the family).
// Shape: the pair temporary (16 bytes) is built by an inline pair constructor: the out-of-line CCharString copy first,
// then the 12-byte value copied with three registers (eax as the source pointer, then ecx/edx/eax, the stores
// interleaved with the push_back set-up) -- that colouring needs a memberwise USER copy constructor over dword members
// (a POD value copies through ecx as the base and edx/eax/ecx); inner scope so the string destructor precedes the Dirty store. CCharString is modelled locally
// (fable_string.h inlines its copy constructor, retail calls 0x0099EC30).
struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);   // 0x0099EC30
    ~CCharString();                          // 0x0099EAE0
};

struct STargetPoint {                        // 12-byte value with a memberwise user copy constructor
    long m_word0, m_word4, m_word8;          // moved as dwords (a float copy constructor would go through the x87)
    STargetPoint(const STargetPoint& other) : m_word0(other.m_word0), m_word4(other.m_word4), m_word8(other.m_word8) { }
};

struct STargetPointPair {
    CCharString first;
    STargetPoint   second;
    STargetPointPair(const CCharString& key, const STargetPoint& value) : first(key), second(value) { }
};

struct CTargetPointPairVector {
    STargetPointPair* m_pFirst;
    STargetPointPair* m_pLast;
    STargetPointPair* m_pEnd;
    void PushBack(const STargetPointPair& pair);     // 0x0077F130
};

struct CVectorMap_TargetPoints {
    CTargetPointPairVector m_vector;         // +0x00
    unsigned char          Compare;          // +0x0c (empty CKeyPairCompareLess)
    bool                   Dirty;            // +0x0d
    void Add(const CCharString& key, const STargetPoint& value);
};

void CVectorMap_TargetPoints::Add(const CCharString& key, const STargetPoint& value)
{
    {
        STargetPointPair pair(key, value);
        m_vector.PushBack(pair);
    }
    Dirty = true;
}