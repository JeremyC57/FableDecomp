// CAnimFilterOfGroup::Clone() -- virtual: heap-copies the filter through its out-of-line one-group constructor.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073E9A0, length 34 bytes.
// Callees: operator new 0x00BFEA1A, CAnimFilterOfGroup::CAnimFilterOfGroup(const CCharString&) 0x0073E8F0
//          (stores vtable 0x01267568 and returns `this`, which Clone hands back directly).
// Layout (PDB, 0xc bytes): vptr +0, Group +4, GroupCRC +8.
struct CCharString { void* PStringData; };

struct CAnimFilterOfGroup {
    virtual long Accepts(const void* anim);
    virtual CAnimFilterOfGroup* Clone();
    CCharString Group;                                   // +4
    unsigned long GroupCRC;                              // +8
    CAnimFilterOfGroup(const CCharString& group);        // 0x0073E8F0
};

CAnimFilterOfGroup* CAnimFilterOfGroup::Clone()
{
    return new CAnimFilterOfGroup(Group);
}