// CSpecialAbilitiesBurntEffectDef::operator=(const CSpecialAbilitiesBurntEffectDef& other) -- the implicit copy assignment: CParentDefClassBase's, then every member in order
// Leaf: ??4CSpecialAbilitiesBurntEffectDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004604CF, 51 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its only reference is the tail jump of CSpecialAbilitiesBurntEffectDef::Copy 0x004604CA; the 4 member operations read from
//   these bytes (byte/dword copies and the vector operator= call) are Ego_r.pdb CSpecialAbilitiesBurntEffectDef's members in order (names and types are Ego_r.pdb's).
// Callees: CParentDefClassBase::operator=(const CParentDefClassBase&) 0x00431ED0, vector<long>::operator= 0x00454886.
// Shape (family M037, representative 0x00454434): compiler-generated `operator=`, emitted through Copy's
//   `*this = *(const CSpecialAbilitiesBurntEffectDef*)other;`; `optimize("s", on)` keeps every scalar copy in eax/al.
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
class CParentDefClassBase {
public:
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
    unsigned char _base[0x39];
};
class CSpecialAbilitiesBurntEffectDef : public CParentDefClassBase {
public:
    void Copy(const CParentDefClassBase* other);
    float                     FadeInTimeSecs;                    // +0x3c
    float                     FadeOutTimeStartSecs;              // +0x40
    float                     FadeOutTimeSecs;                   // +0x44
    std::vector<long>         Color;                             // +0x48
};
void CSpecialAbilitiesBurntEffectDef::Copy(const CParentDefClassBase* other)
{
    *this = *(const CSpecialAbilitiesBurntEffectDef*)other;
}