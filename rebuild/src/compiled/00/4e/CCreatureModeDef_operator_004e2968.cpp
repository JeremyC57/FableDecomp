// CCreatureModeDef::operator=(const CCreatureModeDef& other) -- the implicit copy assignment: CDefClassBase's, then every member in order
// Leaf: ??4CCreatureModeDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E2968, 51 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its only reference is the tail jump of CCreatureModeDef::Copy 0x004E2963; the 4 member operations read from
//   these bytes (byte/dword copies and the vector operator= call) are Ego_r.pdb CCreatureModeDef's members in order (names and types are Ego_r.pdb's).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10, vector<long>::operator= 0x00454886.
// Shape (family M037, representative 0x00454434): compiler-generated `operator=`, emitted through Copy's
//   `*this = *(const CCreatureModeDef*)other;`; `optimize("s", on)` keeps every scalar copy in eax/al.
// Header debt: the def base is modelled by its retail size only (its operator= is out of line).
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    vector& operator=(const vector& __x);                 // out of line
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
// def vtable 0x01241704: slot 0 0x004E0B61 (implicit inline dtor); real overrides from the def vtable scan
class CCreatureModeDef : public CDefClassBase {
public:
    void Copy(const CDefClassBase* other);
    long                      DefaultCreatureMode;               // +0x28
    long                      InitialCreatureMode;               // +0x2c
    long                      DefaultWeaponCreatureMode;         // +0x30
    std::vector<long>         AvailableIdleModes;                // +0x34
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004E7EA7
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004DE7F9
};
void CCreatureModeDef::Copy(const CDefClassBase* other)
{
    *this = *(const CCreatureModeDef*)other;
}