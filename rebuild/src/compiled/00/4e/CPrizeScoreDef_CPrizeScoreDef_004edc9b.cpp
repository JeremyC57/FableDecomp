#pragma optimize("s", on)
// CPrizeScoreDef::CPrizeScoreDef(const CPrizeScoreDef&) -- copy constructor: CDefClassBase copy, this vtable,
// then Score and Mult copied.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EDC9B, length 39 bytes.
// Callees: CDefClassBase::CDefClassBase(const CDefClassBase&) @ 0x0046644C. Vtable 0x012437D4 = CPrizeScoreDef
// (rtti-vtables.tsv); the manifest label "CCardDef::CCardDef" is propagated and rejected.
// PDB CPrizeScoreDef (sizeof 0x30): CDefClassBase occupies 0x25 bytes (padded to 0x28), +0x28 float Score,
// +0x2C long Mult. Score is copied with a plain dword move, so it is modelled as a dword here.
struct CDefClassBase {
    virtual void v00();
    unsigned char _body[0x24];                   // +0x04..+0x27 (PDefManager, GlobalIndex, flag bits)
    CDefClassBase(const CDefClassBase& other);   // 0x0046644C
};

struct CPrizeScoreDef : CDefClassBase {
    virtual void v00();                          // own vtable (0x012437D4)
    long Score;                                  // +0x28 (float in the PDB; copied as a dword)
    long Mult;                                   // +0x2C
    CPrizeScoreDef(const CPrizeScoreDef& other);
};

CPrizeScoreDef::CPrizeScoreDef(const CPrizeScoreDef& other)
    : CDefClassBase(other), Score(other.Score), Mult(other.Mult)
{
}