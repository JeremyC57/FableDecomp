// CTCWeaponTrail::GetAllTrailsLastWeaponTrailPoints(vector<vector<pair<C3DVector,C3DVector> > >& points) const -- for every weapon trail collect its newest points into a fresh vector and append that vector to the output when the trail produced any
// Leaf: ?GetAllTrailsLastWeaponTrailPoints@CTCWeaponTrail
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076EA60, 160 bytes, `ret 4`.
// Name: FableWin.pdb `?GetAllTrailsLastWeaponTrailPoints@CTCWeaponTrail@@QBEXAAV?$vector@V?$vector@U?$pair@VC3DVector@@V1@@std@@...`
//   (void (vector<vector<pair<C3DVector,C3DVector> > >&) const); one caller (0x0078896F). Label correction: catalog
//   "WeaponTrail_ProcessCollisionPoints" -> CTCWeaponTrail::GetAllTrailsLastWeaponTrailPoints.
// Callees: the private CTCWeaponTrail::GetLastWeaponTrailPoints(vector&, const list<CWeaponTrailEntry>&) const 0x0076E4D0;
//   vector<pair<C3DVector,C3DVector> >'s copy constructor 0x0076FB00 (the push_back placement copy);
//   vector<vector<...> >::_M_insert_overflow(pos, x, const __false_type&, 1, true) 0x00770BB0; free 0x00BFEA14 (the
//   local vector's deallocate).
// Layout: the landed public GetLastWeaponTrailPoints(vector&, long) 0x0076E5F0 model: NoTrails +0x0c, Trail +0x48
//   (vector<list<CWeaponTrailEntry> >, 4-byte STLport lists).
// Shape: `for (long i = 0; i < NoTrails; ++i) { vector trailPoints; if (GetLastWeaponTrailPoints(trailPoints, i))
//   points.push_back(trailPoints); }` with the public (vector&, long) overload inlined (its `i < NoTrails` test and the
//   negative-index arm reading Trail[0]); STLport push_back with the placement copy's null test and `++_M_finish` as a
//   memory add; the overflow call's `__false_type()` tag is homed on the dead `points` argument slot.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

struct __false_type { __false_type() { } };

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};

typedef pair<C3DVector, C3DVector> CPointPair;

template <class _Tp>
struct allocator {
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

class CPointPairVector {                                // vector<pair<C3DVector, C3DVector> >
public:
    CPointPair* _M_start;                               // +0x00
    CPointPair* _M_finish;                              // +0x04
    _STLP_alloc_proxy<CPointPair*, CPointPair, allocator<CPointPair> > _M_end_of_storage;   // +0x08
    CPointPairVector() : _M_start(0), _M_finish(0), _M_end_of_storage(allocator<CPointPair>(), (CPointPair*)0) { }
    CPointPairVector(const CPointPairVector& __x);      // 0x0076FB00
    ~CPointPairVector() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};

class CPointPairVectorVector {                          // vector<vector<pair<C3DVector, C3DVector> > >
public:
    CPointPairVector* _M_start;                         // +0x00
    CPointPairVector* _M_finish;                        // +0x04
    CPointPairVector* _M_end_of_storage;                // +0x08
    void _M_insert_overflow(CPointPairVector* __position, const CPointPairVector& __x, const __false_type&,
                            unsigned int __fill_len, bool __atend);   // 0x00770BB0
    void push_back(const CPointPairVector& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CPointPairVector(__x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1, true);
    }
};

struct CWeaponTrailNode;
class CWeaponTrailList {                                // STLport list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>
public:
    CWeaponTrailNode* _M_node;
};

class CWeaponTrailListVector {                          // vector<list<...> >
public:
    CWeaponTrailList* _M_start;
    CWeaponTrailList* _M_finish;
    CWeaponTrailList* _M_end_of_storage;
    const CWeaponTrailList& operator[](long i) const { return _M_start[i]; }
};

class CTCWeaponTrail {
public:
    void*                  __vftable;                   // +0x00
    void*                  ParentThing;                 // +0x04
    bool                   ToBeRemoved;                 // +0x08
    bool                   IsInGlobalUpdateTCs;         // +0x09
    long                   NoTrails;                    // +0x0c
    long                   NoProjectileTrails;          // +0x10
    unsigned char          _pad_0x14[0x48 - 0x14];
    CWeaponTrailListVector Trail;                       // +0x48
    bool GetLastWeaponTrailPoints(CPointPairVector& points, const CWeaponTrailList& trail) const;   // 0x0076E4D0
    bool GetLastWeaponTrailPoints(CPointPairVector& points, long trailIndex) const                  // 0x0076E5F0
    {
        if (trailIndex < NoTrails)
            return GetLastWeaponTrailPoints(points, trailIndex >= 0 ? Trail[trailIndex] : Trail[0]);
        return false;
    }
    long GetNoTrails() const { return NoTrails; }
    void GetAllTrailsLastWeaponTrailPoints(CPointPairVectorVector& points) const;
};

void CTCWeaponTrail::GetAllTrailsLastWeaponTrailPoints(CPointPairVectorVector& points) const
{
    for (long i = 0; i < GetNoTrails(); ++i) {
        CPointPairVector trailPoints;
        if (GetLastWeaponTrailPoints(trailPoints, i))
            points.push_back(trailPoints);
    }
}