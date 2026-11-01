// CNavigationAreaInit::~CNavigationAreaInit() -- the compiler-generated destructor: free the NavRegionSeeds and DetailedAreas blocks, then run ~CNavigationAreaUpdate on the base
// Leaf: ??1CNavigationAreaInit@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051D590, 43 bytes, thiscall, tail `jmp ~CNavigationAreaUpdate`.
// Identity (proven): Ego_r.pdb orders ??1CNavigationAreaUpdate@@QAE@XZ (0x004EAD30) then ??1CNavigationAreaInit@@QAE@XZ
//   (0x004EADE0); retail has the same pair, ~CNavigationAreaUpdate at 0x0051D530 (the four-vector body, bound
//   ??1CNavigationAreaUpdate@@QAE@XZ) immediately followed by this row, which tail-calls it. FableWin.pdb (0x01C8E290) has the
//   symbol too, in CWorldMap's TU beside ??0CNavigationAreaInit / ??0 / ??1CNavigationAreaUpdate (compiler-generated members
//   emitted there). Ego_r.pdb CNavigationAreaInit is CNavigationAreaUpdate (+0) + WorldArea C2DBoxI + MapIndex +
//   DetailedAreas vector<C2DBoxF> + NavRegionSeeds vector<CNavigationPosition>: with retail's 0x30-byte base and 12-byte
//   STLport vectors that is +0x30 / +0x40 / +0x44 / +0x50, the two blocks this body frees. No callers: the row is unreferenced
//   in retail (no call, jump or vtable slot reaches it), kept by the linker. The catalog label "~CPatchTesselationEdgeStrip"
//   is propagated.
// Callees: free 0x00BFEA14, CNavigationAreaUpdate::~CNavigationAreaUpdate 0x0051D530.
// Shape: members in reverse order: ~vector (trivial _Destroy) + ~_Vector_base's null-tested free for NavRegionSeeds then
//   DetailedAreas; WorldArea / MapIndex are trivial; then the base destructor. DestroyOne (a destruction by name, ungraded)
//   makes VC7.1 emit this out-of-line implicit `??1` (chunks 656 / 657 / 696 / 697).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
protected:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

}   // namespace std

class C2DVector {
public:
    float X, Y;
};

class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
};

class C2DBoxF {
public:
    float TLX, TLY, BRX, BRY;
};

class CNavigationPosition {                                 // 12 bytes
public:
    C2DVector Position;
    long Layer;
};

class CNavigationTransitionPoint;

class CNavigationAreaUpdate {                               // retail 0x30 bytes (four STLport vectors)
public:
    class CNavigationLayerUpdate;
    ~CNavigationAreaUpdate();                               // 0x0051D530
    std::vector<CNavigationLayerUpdate> NavigationLayers;   // +0x00
    std::vector<CNavigationTransitionPoint> TransitionPoints;   // +0x0c
    std::vector<CNavigationPosition> ActionPoints;          // +0x18
    std::vector<unsigned char> TopologyWeights;             // +0x24
};

class CNavigationAreaInit : public CNavigationAreaUpdate {  // retail 0x5c bytes
public:
    C2DBoxI WorldArea;                                      // +0x30
    long MapIndex;                                          // +0x40
    std::vector<C2DBoxF> DetailedAreas;                     // +0x44
    std::vector<CNavigationPosition> NavRegionSeeds;        // +0x50
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(CNavigationAreaInit* init)
{
    init->CNavigationAreaInit::~CNavigationAreaInit();
}