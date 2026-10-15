// CVectorMap_AnimEvents::Add(const unsigned long&, const CSmallVector<NAnimationEvents::CEvent,32>&) -- CVectorMap<unsigned long, CSmallVector<CEvent,32>>: appends a {key, events} pair to the vector at +0 and marks the map dirty (+0xd, re-sorted on the next lookup)
// Leaf: Add
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FCF40, length 76 bytes (+4 int3 padding).
// Callees: CSmallVector<NAnimationEvents::CEvent,32>::Copy(const CSmallVector&) @ 0x006FB5C0 (frees the old block with
// operator delete[], allocates size*8 bytes, copies the 8-byte events), the pair vector's push_back @ 0x006FCD90,
// operator delete[] @ 0x00BFEB1C (the pair temporary's CSmallVector destructor).
// Label correction: the manifest "CTCSkeletalMorph::AddSkeletalMorph" is propagated over seven CVectorMap<K,V>::Add
// instantiations (this chunk: 0x006A3DA0, 0x006FCF40, 0x0077F270, 0x00A41A70, 0x00A72B90, 0x00AF2910, 0x00AF3A40).
// PDB CVectorMap<K,V,CKeyPairCompareLess<K,V>> = vector (retail 12 bytes) + Compare (+0xc, empty) + Dirty (+0xd).
// Shape: the pair temporary is built by an inline pair constructor (key word first, then the CSmallVector copy
// constructor = two zero words + Copy(other)); it lives in an inner scope so its destructor (`delete[] m_pData`, the
// out-of-line operator delete[] with `add esp,4`) runs before the Dirty store.
struct CEvent;

struct CSmallVector_CEvent32 {               // CSmallVector<NAnimationEvents::CEvent,32>: 8 bytes
    CEvent*       m_pData;
    unsigned long m_size;
    CSmallVector_CEvent32(const CSmallVector_CEvent32& other) : m_pData(0), m_size(0) { Copy(other); }
    ~CSmallVector_CEvent32() { delete[] (char*)m_pData; }
    void Copy(const CSmallVector_CEvent32& other);   // 0x006FB5C0
};

struct SAnimEventsPair {
    unsigned long         first;
    CSmallVector_CEvent32 second;
    SAnimEventsPair(const unsigned long& key, const CSmallVector_CEvent32& value) : first(key), second(value) { }
};

struct CAnimEventsPairVector {
    SAnimEventsPair* m_pFirst;
    SAnimEventsPair* m_pLast;
    SAnimEventsPair* m_pEnd;
    void PushBack(const SAnimEventsPair& pair);       // 0x006FCD90
};

struct CVectorMap_AnimEvents {
    CAnimEventsPairVector m_vector;          // +0x00
    unsigned char         Compare;           // +0x0c (empty CKeyPairCompareLess)
    bool                  Dirty;             // +0x0d
    void Add(const unsigned long& key, const CSmallVector_CEvent32& value);
};

void CVectorMap_AnimEvents::Add(const unsigned long& key, const CSmallVector_CEvent32& value)
{
    {
        SAnimEventsPair pair(key, value);
        m_vector.PushBack(pair);
    }
    Dirty = true;
}