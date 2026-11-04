// NCameraModes::CProjectileWeapon::~CProjectileWeapon() -- implicit camera-mode destructor: free the Attractors vector storage (+0xf0), then NCameraModes::CBase::~CBase (tail jump)
// Leaf: ??1CProjectileWeapon@NCameraModes@@UAE@XZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087B660, 30 bytes: push esi ; mov esi,ecx ; mov eax,[esi+0F0h] ; test eax,eax ; je ;
//   push eax ; call free ; add esp,4 ; mov ecx,esi ; pop esi ; jmp 0x0056E0E0.
// Identity (proven): the only caller is 0x0087B640 (call at 0x0087B643), slot 0 (scalar deleting destructor) of vtable
//   0x01277804, RTTI .?AVCProjectileWeapon@NCameraModes@@ (rtti-vtables.tsv; RTTI base NCameraModes::CBase, rtti_bases.tsv),
//   the vtable the landed constructor 0x00879620 stores. Ego_r.pdb lists the class's ctor, ??_E and members but NO destructor;
//   FableWin.pdb (debug) has the generated ??1CProjectileWeapon@NCameraModes@@UAE@XZ: compiler-generated, hence no vtable store
//   and the straight tail jump to ~CBase 0x0056E0E0 (landed, Ego_r ??1CBase@NCameraModes@@UAE@XZ). The manifest label
//   "CTCInventoryItem::OnDie" is propagated from the byte-identical twin 0x007D9EF0.
// Callees: free 0x00BFEA14 (STLport malloc allocator, vector<C3DVector>::~vector), NCameraModes::CBase::~CBase 0x0056E0E0.
// Layout (Ego_r.pdb - 0x24; retail CBase 0xA4, landed 0x0056E0E0): ShotFiredTimer +0xa4, Finished +0xa8, XAxis +0xa9 / YAxis
//   +0xb5 / ZAxis +0xc1 (C3DVector packs to 1: the landed GetCamPosOffsetXY reads XAxis at +0xa9, Ego_r has it at +0xcd), Phi
//   +0xd0 .. DeltaLockDeltaHeightParam +0xec, Attractors (vector<C3DVector>) +0xf0, MaxZoomInFOV +0xfc, MinZoomInFOV +0x100,
//   ZoomInFOV +0x104 (GetDefaultFOV 0x0087B630 `fld [ecx+104h]`). The base is modelled locally (no engine header for
//   NCameraModes::CBase; engine/CBase.h is an unrelated namesake).
// Shape: STLport 4.6 vector over the malloc allocator: ~vector's _Destroy is a no-op for C3DVector, ~_Vector_base frees the
//   storage behind the null test. The class declares no destructor; the constructor is defined here (as retail's
//   CProjectileWeapon TU does, 0x00879620) so the vftable, the scalar deleting destructor and the implicit destructor are emitted.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"                            // the constructor's const CDefPointer<const CCameraModeDef>& parameter
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CGameCameraManager;
class CCameraModeDef;

#pragma pack(push, 1)                                      // genuine: C3DVector members sit at odd offsets (XAxis +0xa9)
#include "engine/C3DVector.h"
#pragma pack(pop)

namespace std {
struct __true_type { };
template <class _Tp> struct __type_traits { typedef __true_type has_trivial_destructor; };
template <class _ForwardIterator> inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector() { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

namespace NCameraModes {

class CBase {                                              // vtable 0x0124DD74, retail 0xA4 bytes
public:
    CBase(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def);   // 0x0084E420
    virtual ~CBase();                                      // 0x0056E0E0
private:
    unsigned char _body[0xa4 - 4];
};

class CProjectileWeapon : public CBase {                   // vtable 0x01277804
public:
    CProjectileWeapon(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def, float maxZoomInFOV,
                      float minZoomInFOV);
    long      ShotFiredTimer;                              // +0xa4
    bool      Finished;                                    // +0xa8
    C3DVector XAxis;                                       // +0xa9
    C3DVector YAxis;                                       // +0xb5
    C3DVector ZAxis;                                       // +0xc1
    float     Phi;                                         // +0xd0
    float     HeightParam;                                 // +0xd4
    float     DeltaPhi;                                    // +0xd8
    float     DeltaHeightParam;                            // +0xdc
    float     LockDeltaPhi;                                // +0xe0
    float     LockDeltaHeightParam;                        // +0xe4
    float     DeltaLockDeltaPhi;                           // +0xe8
    float     DeltaLockDeltaHeightParam;                   // +0xec
    std::vector<C3DVector> Attractors;                     // +0xf0
    float     MaxZoomInFOV;                                // +0xfc
    float     MinZoomInFOV;                                // +0x100
    float     ZoomInFOV;                                   // +0x104
};

CProjectileWeapon::CProjectileWeapon(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def,
                                     float maxZoomInFOV, float minZoomInFOV)
    : CBase(manager, def), ShotFiredTimer(-1), Finished(false), MaxZoomInFOV(maxZoomInFOV), MinZoomInFOV(minZoomInFOV)
{
}

} // namespace NCameraModes