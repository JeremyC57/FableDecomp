// CSpecialAbilitiesForcePushDef::CSpecialAbilitiesForcePushDef() -- CParentDefClassBase base, this vtable, three empty retail vectors at +0x78, +0x8c, +0x98
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045767E, length 59 bytes.
// Callees: CParentDefClassBase::CParentDefClassBase @ 0x00430370.
// Vtable 0x012342CC = CSpecialAbilitiesForcePushDef (rtti-vtables.tsv); the manifest label
// "CBossDef" is propagated (the PDB CBossDef is 96 bytes, its vectors at +0x28..+0x48).
// Shape: the vtable store precedes the zero stores because each 12-byte block is a member object
// (first/last/end) with an inline constructor; the size-optimised def module (0x0045xxxx) forms
// each block's address once (`lea eax,[esi+N]`) and stores through short displacements.
#pragma optimize("s", on)
class CParentDefClassBase { public:
    virtual void v00();
    unsigned char _body[0x74];                  // +0x04..+0x77
    CParentDefClassBase();                      // 0x00430370
};

template <class T>
struct CRetailVector {                          // VC6-era Dinkumware vector: first / last / end
    T* first;
    T* last;
    T* end;
    CRetailVector() : first(0), last(0), end(0) { }
};

class CSpecialAbilitiesForcePushDef : public CParentDefClassBase { public:   // vtable 0x012342CC
    CRetailVector<long> m_values0;              // +0x78
    unsigned char       _pad[0x08];             // +0x84..+0x8b
    CRetailVector<long> m_values1;              // +0x8c
    CRetailVector<long> m_values2;              // +0x98
    virtual void v00();
    CSpecialAbilitiesForcePushDef();
};

CSpecialAbilitiesForcePushDef::CSpecialAbilitiesForcePushDef()
    : CParentDefClassBase(), m_values0(), m_values1(), m_values2()
{
}