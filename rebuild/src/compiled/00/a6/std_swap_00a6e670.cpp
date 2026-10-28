// std::swap(pair<CCharString, C3DDummyObject>& a, pair<CCharString, C3DDummyObject>& b) -- exchange two named dummy-object entries through a temporary copy (string copy-constructed/assigned, the 0x34-byte dummy object block-copied)
// Leaf: swap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6E670, length 105 bytes. Reached through the /Gr jump thunk 0x00A6EF80
// (its neighbour 0x00A6E5E0 is swap<C3DSkeletalBoneMorph>, thunk 0x00A6EF70).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, CCharString::operator= @ 0x0099EFB0 (x2),
//          CCharString::~CCharString @ 0x0099EAE0.
// Identity: the catalog label "CPSCOrbit::DatabaseCopy" is wrong: this is a __fastcall (a in ecx, b in edx) SGI swap
// instantiation; the element is 0x38 bytes with a CCharString first, which is the PDB _Pair_base<CCharString,
// C3DDummyObject> (56 bytes).
// Shape: `T tmp(a); a = b; b = tmp;` with the implicit copy constructor/assignment: the string through its
// out-of-line members, the 13-dword POD through `rep movsd`; the algorithm TU is /Gr (__fastcall free templates).
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class C3DDummyObject { public:                                 // 0x34-byte POD (position/orientation record)
    unsigned long m_words[13];
};
struct SDummyObjectEntry {                              // pair<CCharString, C3DDummyObject>
    CCharString    first;
    C3DDummyObject second;
};

template <class T> void __fastcall swap(T& a, T& b)
{
    T tmp(a);
    a = b;
    b = tmp;
}

template void __fastcall swap<SDummyObjectEntry>(SDummyObjectEntry& a, SDummyObjectEntry& b);