// std::vector::push_back(const pair<NHitLocations::CHitLocation, long>& x) -- append a (hit location, value) pair: copy-construct it in place when there is room, else grow through _M_insert_overflow
// Leaf: ?push_back@?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00818DC0, 109 bytes, thiscall `ret 4`.
// Callers: one, 0x00816B73.
// Callees: vector::_M_insert_overflow(pos, x, const __false_type&, fill_len, atend) 0x00818CA0.
// Element: STLport pair<NHitLocations::CHitLocation, long> (0x1C bytes; Ego_r.pdb names the vector of this pair). CHitLocation
//   (Ego_r.pdb, 0x18 bytes): CThing* POwnerThing +0x0, CDefPointer<const CHitLocationDef> PDef +0x4, CDefPointer<const
//   CArmourDef> PArmourDef +0x8, EActionRegisteredType HitResponseAction +0xC / BlockResponseAction +0x10, bool Enabled
//   +0x14; its implicit copy constructor bumps each def's reference count (+0x4 of the def) through CDefPointer's inline
//   copy constructor (null-tested).
// Shape: STLport 4.6 `push_back(const _Tp& x) { if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish, x);
//   ++_M_finish; } else _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true); }` -- the placement new keeps its null
//   test, the pair copy is memberwise, `add [edi+4],1Ch`, and the empty __false_type temporary is homed in the dead `x`
//   argument slot for the overflow call (as the landed push_back 0x00709E60).
inline void* operator new(unsigned int, void* p) { return p; }

class CThing;

class CDefClassBase {
public:
    virtual ~CDefClassBase();                   // +0x0 vptr
    long RefCount;                              // +0x4 (bumped by CDefPointer copies)
};
class CHitLocationDef : public CDefClassBase { };
class CArmourDef : public CDefClassBase { };

template <class T> class CDefPointer {
public:
    T* Object;                                  // +0x0
    CDefPointer(const CDefPointer<T>& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
};

enum EActionRegisteredType { ACTION_REGISTERED_TYPE_NULL };

namespace NHitLocations {
class CHitLocation {                            // Ego_r.pdb, 0x18 bytes
public:
    CThing*                     POwnerThing;            // +0x00
    CDefPointer<CHitLocationDef> PDef;                  // +0x04
    CDefPointer<CArmourDef>     PArmourDef;             // +0x08
    EActionRegisteredType       HitResponseAction;      // +0x0C
    EActionRegisteredType       BlockResponseAction;    // +0x10
    bool                        Enabled;                // +0x14
};
}

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;                                  // +0x00
    _T2 second;                                 // +0x18
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend);                                                    // 0x00818CA0
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

template void vector<pair<NHitLocations::CHitLocation, long> >::push_back(const pair<NHitLocations::CHitLocation, long>&);
}