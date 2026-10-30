// CBrainDef::CBrainDef() -- the definition's default constructor: CParentDefClassBase, the vtable, then the empty member vectors
// Leaf: ??0CBrainDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00459A6F, 47 bytes, thiscall, plain `ret` (size-optimised module).
// Identity (proven): stores vtable 0x0123520C (RTTI .?AVCBrainDef@@); the zeroed vectors at +0x3c, +0x48, +0x54 are Ego_r.pdb
//   CBrainDef's vector members Behaviours, DisabledBehaviours, UpdateZones (12-byte retail vectors after the 0x39-byte CParentDefClassBase part).
// Callees: CParentDefClassBase::CParentDefClassBase() 0x00430370.
// Shape (family M041, representative 0x00457973): `optimize("s", on)` -- the base constructor out of line, the vtable
//   store, then each STLport vector's inline default constructor (three zero stores).
// Header debt: the base is modelled by its retail size, the scalar members before the first vector as opaque bytes
//   (Ego_r.pdb: none).
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _STLP_alloc_proxy(const _Alloc& __a, _Tp __p) : _Alloc(__a), _M_data(__p) { }
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
};
}
class CBrainBehaviour;
class CDefString;
class CBrainUpdateZone;
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
class CBrainDef : public CParentDefClassBase {
public:
    CBrainDef();
    std::vector<CBrainBehaviour> Behaviours;     // +0x3c
    std::vector<CDefString> DisabledBehaviours;  // +0x48
    std::vector<CBrainUpdateZone> UpdateZones;   // +0x54
};
CBrainDef::CBrainDef()
{
}