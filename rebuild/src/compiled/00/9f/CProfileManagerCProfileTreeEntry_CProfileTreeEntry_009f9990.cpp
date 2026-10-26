// CProfileManager::CProfileTreeEntry::CProfileTreeEntry() -- default constructor of a profiler tree node: the profile (no manager, an empty name), then an empty child vector
// Leaf: ??0CProfileTreeEntry@CProfileManager@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F9990, 42 bytes (+ padding to 48), __fastcall `ret`, returns `this`.
// Identity (label correction): the catalog's "pair<unsigned_long,C3DMeshVolume>" is propagated. Its neighbours are the
//   same record's copy constructor 0x009F99E0 (the landed CProfile copy constructor 0x009F9450 on +0, ID +0x48, the child
//   vector's copy 0x009F9930 on +0x4C, Parent +0x58) and destructors 0x009F99C0 / 0x009F9A10 (free the child storage,
//   then ~CWideString on +0x30): Ego_r.pdb CProfileManager::CProfileTreeEntry (Profile CProfile +0, ID +0x48, Children
//   vector<CProfileTreeEntry*> +0x4C with retail's 12-byte vector, Parent +0x58). No direct callers.
// Callee: CWideString::CWideString() @ 0x0099AED0 (the profile's Name at +0x30).
// Shape: CProfile's inline default constructor initialises only Manager (+0) and the name; STLport's vector default
//   constructor zeroes the three pointers; ID and Parent are left unset.
class CProfileManager;

class CWideString {
public:
    void* PStringData;                                  // +0x00
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
};

class CProfile {                                        // 0x48 bytes
public:
    CProfileManager* Manager;                           // +0x00
    double           StartTime;                         // +0x08
    double           TotalTime;                         // +0x10
    double           LastTime;                          // +0x18
    bool             Running;                           // +0x20
    double           Percentage;                        // +0x28
    CWideString      Name;                              // +0x30
    long             TreeDepth;                         // +0x34
    bool             NewNode;                           // +0x38
    long             Visible;                           // +0x3C
    long             CallCounter;                       // +0x40
    long             UpdatedCallCounter;                // +0x44
    CProfile() : Manager(0) { }
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp*                                          _M_start;            // +0
    _Tp*                                          _M_finish;           // +4
    _STLP_alloc_proxy<_Tp*, _Tp, allocator<_Tp> > _M_end_of_storage;   // +8
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
};
}

class CProfileManager {
public:
    struct CProfileTreeEntry {                           // 0x5C bytes
    public:
        CProfile                          Profile;      // +0x00
        long                              ID;           // +0x48
        std::vector<CProfileTreeEntry*>  Children;     // +0x4C
        CProfileTreeEntry*                Parent;       // +0x58
        CProfileTreeEntry();
    };
};

CProfileManager::CProfileTreeEntry::CProfileTreeEntry()
{
}