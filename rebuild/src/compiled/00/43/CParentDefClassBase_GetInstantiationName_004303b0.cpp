// CParentDefClassBase::GetInstantiationName() const -- returns the def's instantiation name (the CDefString member at +0x25) by value
// Retail: Fable.exe @ 0x004303B0, 12 bytes, `ret 4` (hidden result pointer). Virtual (manifest mangled name
// ?GetInstantiationName@CParentDefClassBase@@UBE?AVCDefString@@XZ).
// Layout: Ego_r.pdb CParentDefClassBase (61 bytes, packed): InstantiationName (CDefString) at +37 = +0x25, unaligned;
// the CDefClassBase part (vptr and 0x21 more bytes) is opaque here.
// Shape: CDefString has a user copy constructor (a non-POD 4-byte handle), so the result goes through the hidden
// pointer and the copy is one dword move.
#pragma pack(push, 1)
class CDefString {
public:
    long TablePos;
    CDefString(const CDefString& other) : TablePos(other.TablePos) {}
};
class CDefClassBase {
public:
    virtual ~CDefClassBase();
    unsigned char _body[0x21];                           // +0x04
};
class CParentDefClassBase : public CDefClassBase {
public:
    CDefString InstantiationName;                        // +0x25
    unsigned char SubDefInfoMap[0x14];                   // +0x29
    virtual CDefString GetInstantiationName() const;
};
#pragma pack(pop)

CDefString CParentDefClassBase::GetInstantiationName() const
{
    return InstantiationName;
}