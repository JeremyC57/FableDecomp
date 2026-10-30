// CRumbleDef::operator=(const CRumbleDef& other) -- the implicit copy assignment: CDefClassBase's, then every member in order
// Leaf: ??4CRumbleDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E64F3, 45 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its only reference is the tail jump of CRumbleDef::Copy 0x004E64EE; the 2 member operations read from
//   these bytes (byte/dword copies and the container operator= calls) are Ego_r.pdb CRumbleDef's members in order (names and types are Ego_r.pdb's).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10, map<EQuakeStrength, float>::operator= 0x004E6520, map<EQuakeLength, float>::operator= 0x004E6645.
// Shape (family M043, representative 0x0045D2FC): compiler-generated `operator=`, emitted through Copy's
//   `*this = *(const CRumbleDef*)other;`; `optimize("s", on)` keeps every scalar copy in eax/al.
// Header debt: the def base is modelled by its retail size only (its operator= is out of line).
#pragma optimize("s", on)
enum EQuakeStrength { EQUAKESTRENGTH_NONE = 0 };
enum EQuakeLength { EQUAKELENGTH_NONE = 0 };
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    map& operator=(const map& __x);                       // out of line
    void* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
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
// def vtable 0x0124273C: slot 0 0x004E32A6 (implicit inline dtor); real overrides from the def vtable scan
class CRumbleDef : public CDefClassBase {
public:
    void Copy(const CDefClassBase* other);
    std::map<EQuakeStrength, float> QuakeIntensities;                  // +0x28
    std::map<EQuakeLength, float> QuakeDurations;                    // +0x34
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004E676A
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004E1744
};
void CRumbleDef::Copy(const CDefClassBase* other)
{
    *this = *(const CRumbleDef*)other;
}