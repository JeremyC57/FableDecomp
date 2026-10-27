// CTCCreatureHitNotification::CTCCreatureHitNotification(CThing& thing) -- thing-component constructor: the CTCBase base, the vtable, then the empty member vector
// Leaf: ??0CTCCreatureHitNotification@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E7F9F, 35 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): stores vtable 0x012436CC (RTTI .?AVCTCCreatureHitNotification@@); Ego_r.pdb ??0CTCCreatureHitNotification@@QAE@AAVCThing@@@Z with Flashes (vector<NCreatureHitNotification::CFlash>) at +0x0C; its caller is CTCCreatureHitNotification::Alloc 0x004EA695.
// Callees: CTCBase::CTCBase(CThing&) 0x00686800.
// Shape (family M098, representative 0x004DEBD4): `: CTCBase(thing) {}` -- the vtable store, then the STLport vector's inline
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
namespace NCreatureHitNotification { class CFlash; }
class CTCCreatureHitNotification : public CTCBase {
public:
    CTCCreatureHitNotification(CThing& thing);
    std::vector<NCreatureHitNotification::CFlash> Flashes;   // +0x0C
};
CTCCreatureHitNotification::CTCCreatureHitNotification(CThing& thing)
    : CTCBase(thing)
{
}