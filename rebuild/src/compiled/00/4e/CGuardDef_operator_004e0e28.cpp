// CGuardDef::operator=(const CGuardDef& other) -- the implicit copy assignment: CDefClassBase's, then every member in order
// Leaf: ??4CGuardDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0E28, 81 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its only reference is the tail jump of CGuardDef::Copy 0x004E0E23; the 10 member operations read from
//   these bytes (byte/dword copies) are Ego_r.pdb CGuardDef's members in order (names and types are Ego_r.pdb's).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10.
// Shape (family M099, representative 0x004E03D4): compiler-generated `operator=`, emitted through Copy's
//   `*this = *(const CGuardDef*)other;`; `optimize("s", on)` keeps every scalar copy in eax/al.
// Header debt: the def base is modelled by its retail size only (its operator= is out of line).
#pragma optimize("s", on)
class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
    unsigned char _base[0x25];
};
class CGuardDef : public CDefClassBase {
public:
    void Copy(const CDefClassBase* other);
    long                      BribeCostWeaponOut;                // +0x28
    long                      BribeCostTrespass;                 // +0x2c
    long                      BribeCostVandalism;                // +0x30
    long                      BribeCostShoplifter;               // +0x34
    long                      BribeCostPickPocket;               // +0x38
    long                      BribeCostAssault;                  // +0x3c
    long                      BribeCostGuardAssault;             // +0x40
    long                      BribeCostGBH;                      // +0x44
    long                      BribeCostGuardGBH;                 // +0x48
    long                      BribeCostMurder;                   // +0x4c
};
void CGuardDef::Copy(const CDefClassBase* other)
{
    *this = *(const CGuardDef*)other;
}