// CTCNamedUserActionPoints::CTCNamedUserActionPoints(CThing& thing) -- thing-component constructor: the CTCUserActionPointBase base, the vtable, then the empty member vector
// Leaf: ??0CTCNamedUserActionPoints@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0398, 35 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): stores vtable 0x01241F24 (RTTI .?AVCTCNamedUserActionPoints@@); Ego_r.pdb ??0CTCNamedUserActionPoints@@QAE@AAVCThing@@@Z with Names (vector<CCharString>) at +0x34, right after the 0x34-byte CTCUserActionPointBase (two 12-byte vectors, two longs, an intelligent pointer after the CTCBase part); the base constructor 0x007FEC60 is Ego_r.pdb ??0CTCUserActionPointBase@@QAE@AAVCThing@@@Z; its caller is CTCNamedUserActionPoints::Alloc 0x004E23FF.
// Callees: CTCUserActionPointBase::CTCUserActionPointBase(CThing&) 0x007FEC60.
// Shape (family M098, representative 0x004DEBD4): `: CTCUserActionPointBase(thing) {}` -- the vtable store, then the STLport vector's inline
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
class CCharString;
class CTCUserActionPointBase : public CTCBase {
public:
    CTCUserActionPointBase(CThing& thing); // 0x007FEC60
    unsigned char _base[0x28];                        // +0x0C .. retail size 0x34
};
class CTCNamedUserActionPoints : public CTCUserActionPointBase {
public:
    CTCNamedUserActionPoints(CThing& thing);
    std::vector<CCharString> Names;                      // +0x34
};
CTCNamedUserActionPoints::CTCNamedUserActionPoints(CThing& thing)
    : CTCUserActionPointBase(thing)
{
}