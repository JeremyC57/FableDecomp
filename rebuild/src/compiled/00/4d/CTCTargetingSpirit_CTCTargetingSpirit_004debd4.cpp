// CTCTargetingSpirit::CTCTargetingSpirit(CThing& thing) -- thing-component constructor: the CTCTargetingBase base, the vtable, then the empty member vector
// Leaf: ??0CTCTargetingSpirit@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DEBD4, 35 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): stores vtable 0x012419A4 (RTTI .?AVCTCTargetingSpirit@@); Ego_r.pdb ??0CTCTargetingSpirit@@QAE@AAVCThing@@@Z with Targets (vector<CIntelligentPointer<CThing> >) at +0x3C after MaxTargetingDistance; the base CTCTargetingBase(CThing&) 0x007F58E0 (Ego_r.pdb ??0CTCTargetingBase@@QAE@AAVCThing@@@Z; 0x38 bytes); its caller is CTCTargetingSpirit::Alloc 0x004E1006.
// Callees: CTCTargetingBase::CTCTargetingBase(CThing&) 0x007F58E0.
// Shape (family M098, representative 0x004DEBD4): `: CTCTargetingBase(thing) {}` -- the vtable store, then the STLport vector's inline
//   default constructor (three zero stores); `optimize("s")`.
// Header debt: CTCBase modelled locally (engine/CTCBase.h flattens the vptr), the intermediate base by its retail size.
#pragma optimize("s", on)
class CThing;
class CTCBase {                                          // retail 0x0C bytes (Ego_r.pdb)
public:
    CTCBase(CThing& thing);                              // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                                 // +0x04
    bool ToBeRemoved;                                    // +0x08
    bool IsInGlobalUpdateTCs;                            // +0x09
};
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
template <class T> class CIntelligentPointer;
class CTCTargetingBase : public CTCBase {
public:
    CTCTargetingBase(CThing& thing); // 0x007F58E0
    unsigned char _base[0x2c];                        // +0x0C .. retail size 0x38
};
class CTCTargetingSpirit : public CTCTargetingBase {
public:
    CTCTargetingSpirit(CThing& thing);
    float MaxTargetingDistance;                          // +0x38 (not constructed here)
    std::vector<CIntelligentPointer<CThing> > Targets;   // +0x3C
};
CTCTargetingSpirit::CTCTargetingSpirit(CThing& thing)
    : CTCTargetingBase(thing)
{
}