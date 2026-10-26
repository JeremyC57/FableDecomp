// CAnimFilterOf2Groups::Clone() -- virtual: heap-copies the filter via the two-group constructor.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073EA70, length 67 bytes.
// Callees: operator new 0x00BFEA1A, CCharString::CCharString() 0x0099E4B0 (twice, Group1 and Group2),
//          CAnimFilterOf2Groups::SetGroups(const CCharString&, const CCharString&) 0x0073E040.
// Shape: `new` null-checked by the compiler; the constructor is inline (vtable 0x01267578 stored, the two
// CCharString members default-constructed) and its body calls the out-of-line SetGroups, which assigns the
// strings and recomputes the CRCs -- that is why the vtable store and the member constructors sit in Clone.
// CAnimFilterOf3Groups / CAnimFilterOfGroup keep their constructors out of line instead (0x0073E2F0, 0x0073E9A0).
class CCharString { public:
    void* PStringData;
    CCharString();                                       // 0x0099E4B0
};

struct CAnimFilterOf2Groups {
    virtual long Accepts(const void* anim);
    virtual CAnimFilterOf2Groups* Clone();
    CCharString Group1;                                  // +4
    CCharString Group2;                                  // +8
    unsigned long Group1CRC;                             // +0xc
    unsigned long Group2CRC;                             // +0x10
    CAnimFilterOf2Groups(const CCharString& group1, const CCharString& group2) { SetGroups(group1, group2); }
    void SetGroups(const CCharString& group1, const CCharString& group2);   // 0x0073E040
};

CAnimFilterOf2Groups* CAnimFilterOf2Groups::Clone()
{
    return new CAnimFilterOf2Groups(Group1, Group2);
}