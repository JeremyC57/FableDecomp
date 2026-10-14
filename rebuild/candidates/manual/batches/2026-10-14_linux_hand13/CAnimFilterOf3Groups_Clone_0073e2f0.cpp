// CAnimFilterOf3Groups::Clone() -- virtual: heap-copies the filter through its out-of-line three-group constructor.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073E2F0, length 42 bytes.
// Callees: operator new 0x00BFEA1A, CAnimFilterOf3Groups::CAnimFilterOf3Groups(const CCharString&, const CCharString&,
//          const CCharString&) 0x0073E0D0 (returns `this`, which Clone hands back directly).
// Layout (PDB, 0x1c bytes): vptr +0, Group1 +4, Group2 +8, Group3 +0xc, CRCs +0x10..+0x18.
struct CCharString { void* PStringData; };

struct CAnimFilterOf3Groups {
    virtual long Accepts(const void* anim);
    virtual CAnimFilterOf3Groups* Clone();
    CCharString Group1;                                  // +4
    CCharString Group2;                                  // +8
    CCharString Group3;                                  // +0xc
    unsigned long Group1CRC;                             // +0x10
    unsigned long Group2CRC;                             // +0x14
    unsigned long Group3CRC;                             // +0x18
    CAnimFilterOf3Groups(const CCharString& group1, const CCharString& group2, const CCharString& group3);   // 0x0073E0D0
};

CAnimFilterOf3Groups* CAnimFilterOf3Groups::Clone()
{
    return new CAnimFilterOf3Groups(Group1, Group2, Group3);
}
