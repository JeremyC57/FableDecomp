// CFactionDef::operator=(const CFactionDef& other) -- the implicit copy assignment: CParentDefClassBase's, then every member in order
// Leaf: ??4CFactionDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045D2FC, 45 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its only reference is the tail jump of CFactionDef::Copy 0x0045D2F7; the 2 member operations read from
//   these bytes (byte/dword copies and the vector operator= call) are Ego_r.pdb CFactionDef's members in order (names and types are Ego_r.pdb's).
// Callees: CParentDefClassBase::operator=(const CParentDefClassBase&) 0x00431ED0, vector<CCharString>::operator= 0x00432EE9, vector<CCharString>::operator= 0x00432EE9.
// Shape (family M043, representative 0x0045D2FC): compiler-generated `operator=`, emitted through Copy's
//   `*this = *(const CFactionDef*)other;`; `optimize("s", on)` keeps every scalar copy in eax/al.
// Header debt: the def base is modelled by its retail size only (its operator= is out of line).
#pragma optimize("s", on)
class CCharString;
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
class CParentDefClassBase {
public:
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
    unsigned char _base[0x39];
};
class CFactionDef : public CParentDefClassBase {
public:
    void Copy(const CParentDefClassBase* other);
    std::vector<CCharString>  EnemyFactions;                     // +0x3c
    std::vector<CCharString>  AlliedFactions;                    // +0x48
};
void CFactionDef::Copy(const CParentDefClassBase* other)
{
    *this = *(const CFactionDef*)other;
}