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
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
// def vtable 0x0123C75C: slot 0 0x004D8A10 (implicit inline dtor); real overrides from the def vtable scan
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
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004DE926
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004D500F
};
void CGuardDef::Copy(const CDefClassBase* other)
{
    *this = *(const CGuardDef*)other;
}