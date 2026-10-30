// CEngineLocalDetailLayerDef::operator=(const CEngineLocalDetailLayerDef& other) -- the implicit copy assignment: CParentDefClassBase's, then every member in order
// Leaf: ??4CEngineLocalDetailLayerDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00477A8A, 45 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its only reference is the tail jump of CEngineLocalDetailLayerDef::Copy 0x00477C65; the 2 member operations read from
//   these bytes (byte/dword copies and the vector operator= call) are Ego_r.pdb CEngineLocalDetailLayerDef's members in order (names and types are Ego_r.pdb's).
// Callees: CParentDefClassBase::operator=(const CParentDefClassBase&) 0x00431ED0, vector<float>::operator= 0x0045EF2D, vector<CEngineLocalDetailObjectDef>::operator= 0x00477AB7.
// Shape (family M043, representative 0x0045D2FC): compiler-generated `operator=`, emitted through Copy's
//   `*this = *(const CEngineLocalDetailLayerDef*)other;`; `optimize("s", on)` keeps every scalar copy in eax/al.
// Header debt: the def base is modelled by its retail size only (its operator= is out of line).
#pragma optimize("s", on)
class CEngineLocalDetailObjectDef;
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
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
class CEngineLocalDetailLayerDef : public CParentDefClassBase {
public:
    void Copy(const CParentDefClassBase* other);
    std::vector<float>        SpacingFromLayer;                  // +0x3c
    std::vector<CEngineLocalDetailObjectDef> Objects;                           // +0x48
};
void CEngineLocalDetailLayerDef::Copy(const CParentDefClassBase* other)
{
    *this = *(const CEngineLocalDetailLayerDef*)other;
}