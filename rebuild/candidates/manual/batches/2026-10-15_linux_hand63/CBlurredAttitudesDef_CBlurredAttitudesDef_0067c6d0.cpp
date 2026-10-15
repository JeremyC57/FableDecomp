// CBlurredAttitudesDef::CBlurredAttitudesDef(const CBlurredAttitudesDef&) -- copy constructor: the CParentDefClassBase copy (CDefClassBase(other), InstantiationName, SubDefInfoMap) inlined, own vtable, then the Attitudes vector by its copy constructor
// Leaf: ??0CBlurredAttitudesDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067C6D0, length 75 bytes.
// Callees: CDefClassBase::CDefClassBase(const CDefClassBase&) @ 0x0046644C,
//          vector<pair<unsigned long, CDefClassBase::CSubDefInfo> > copy constructor @ 0x0046EECF,
//          vector<EOpinionAttitudeType> copy constructor @ 0x00471F97.
// Identity: the manifest label "CParentDefClassBase::CParentDefClassBase" is propagated. The body
// stores vtable 0x01230CBC (CParentDefClassBase) and then 0x0125B8AC = CBlurredAttitudesDef
// (rtti-vtables.tsv), and its five callers (0x0067C9C5, 0x0067CBA3, ...) are the placement-copy
// loops of vector<CBlurredAttitudesDef> stepping 0x48 bytes = 0x3c base + the 12-byte retail
// vector Attitudes at +0x3c (PDB CBlurredAttitudesDef: base 0x40 + 16-byte vector Attitudes).
// Shape: in this TU the CParentDefClassBase copy constructor is inline (out of line it is
// 0x0046EE98): its base call, its vtable store, the InstantiationName word, the inline CVectorMap
// copy (vector copy constructor + Dirty byte through the map pointers ebx/ebp), then this class's
// vtable store (scheduled between the argument push and the call) and the member vector copy.
// Layout: pack(1) def classes (InstantiationName unaligned at +0x25, SubDefInfoMap at +0x29); the
// derived member starts at the aligned +0x3c. The generated header engine/CParentDefClassBase.h is
// the Ego layout (opaque 0x14-byte map), so the classes are modelled locally.

#pragma pack(push, 1)
struct CDefClassBase {
    virtual void v00();
    unsigned char _body[0x21];                                            // +0x04..+0x25
    CDefClassBase(const CDefClassBase& other);                            // 0x0046644C
};
#pragma pack(pop)

struct CDefString {
    long TablePos;
};

struct CSubDefInfoVector {                                                // retail std::vector (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CSubDefInfoVector(const CSubDefInfoVector& other);                    // 0x0046EECF
};

struct CKeyPairCompareLess {
};

struct CSubDefInfoMap {                                                   // CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, ...>
    CSubDefInfoVector   Vector;                                           // +0x00
    CKeyPairCompareLess Compare;                                          // +0x0c
    bool                Dirty;                                            // +0x0d
    CSubDefInfoMap(const CSubDefInfoMap& other)
        : Vector(other.Vector), Compare(other.Compare), Dirty(other.Dirty)
    {
    }
};

#pragma pack(push, 1)
struct CParentDefClassBase : CDefClassBase {                              // vtable 0x01230CBC
    CDefString     InstantiationName;                                     // +0x25
    CSubDefInfoMap SubDefInfoMap;                                         // +0x29
    virtual void v00();
    CParentDefClassBase(const CParentDefClassBase& other)
        : CDefClassBase(other)
        , InstantiationName(other.InstantiationName)
        , SubDefInfoMap(other.SubDefInfoMap)
    {
    }
};
#pragma pack(pop)

struct CAttitudeVector {                                                  // retail std::vector<EOpinionAttitudeType> (12 bytes)
    long* m_first;
    long* m_last;
    long* m_end;
    CAttitudeVector(const CAttitudeVector& other);                        // 0x00471F97
};

struct CBlurredAttitudesDef : CParentDefClassBase {                       // vtable 0x0125B8AC
    CAttitudeVector Attitudes;                                            // +0x3c
    virtual void v00();
    CBlurredAttitudesDef(const CBlurredAttitudesDef& other);
};

CBlurredAttitudesDef::CBlurredAttitudesDef(const CBlurredAttitudesDef& other)
    : CParentDefClassBase(other)
    , Attitudes(other.Attitudes)
{
}
