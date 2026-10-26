// CAnimationSet::CAnimationSet() -- empty animation map, zero defaults, no default group
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DD2E0, length 32 bytes.
// Callees: none.
// Layout: PDB CAnimationSet (0x24 bytes there, 0x20 in retail: the PDB CVectorMap is 4 bytes wider): the
// CVectorMap<unsigned long, CAnimationEntry> Anims at +0x00 is the retail 12-byte vector {first, last, end},
// the empty comparator at +0x0c and the Dirty flag at +0x0d; then DefaultFlags +0x10, DefaultTransitionTime +0x14,
// DefaultDelay +0x18 and the CDefString DefaultGroup +0x1c (TablePos -1).
// Shape: every zero store comes from one cleared register (`xor ecx,ecx` after `mov eax,ecx`, the returned this);
// the member initialisers run in declaration order, the vector map's inline constructor first.
struct CKeyPairCompareLess { };                 // empty comparator object (one byte at +0x0c)

template<class K, class V> class CVectorMap { public:
    struct SPair { K first; V second; };
    SPair* m_first;                             // +0x00
    SPair* m_last;                              // +0x04
    SPair* m_end;                               // +0x08
    CKeyPairCompareLess Compare;                // +0x0c
    bool Dirty;                                 // +0x0d
    CVectorMap() : m_first(0), m_last(0), m_end(0), Dirty(false) { }
};

class CDefString { public:
    long TablePos;
    CDefString() : TablePos(-1) { }
};

class CAnimationEntry { public:
    long BankIndex;                             // +0x00
    CDefString AnimName;                        // +0x04
    CDefString GroupName;                       // +0x08
    void* Components;                           // +0x0c CIVCountedPointer<CAnimComponentMap>
};

class CAnimationSet { public:
    CVectorMap<unsigned long, CAnimationEntry> Anims;   // +0x00
    long DefaultFlags;                          // +0x10
    long DefaultTransitionTime;                 // +0x14
    long DefaultDelay;                          // +0x18
    CDefString DefaultGroup;                    // +0x1c
    CAnimationSet();
};

CAnimationSet::CAnimationSet()
    : DefaultFlags(0), DefaultTransitionTime(0), DefaultDelay(0)
{
}