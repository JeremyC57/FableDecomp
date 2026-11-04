// NParticleEngine::CPSCSingleSprite::~CPSCSingleSprite() -- the compiler-generated destructor: free the PrimIDList block, destroy the TrailUpdatePrimitive's Trail list, then ~CPSCSplineBase
// Leaf: ??1CPSCSingleSprite@NParticleEngine@@UAE@XZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0A160, 41 bytes, thiscall, tail `jmp ~CPSCSplineBase`.
// Identity (proven): the sole caller is the slot-0 `??_G` 0x00B0A140 of vtable 0x0129FB84 (RTTI
//   .?AVCPSCSingleSprite@NParticleEngine@@ : CPSCSplineBase : CPSCBase : CParticleUpdateParamCallback<KK> :
//   CBaseClassNonCopyable : CBaseClass); Ego_r.pdb (0x00ACB250) and FableWin.pdb (0x02F37EC0) spell
//   ??1CPSCSingleSprite@NParticleEngine@@UAE@XZ. The landed ~CPSCSplineBase 0x00B0A060 names this row as its tail-jumping
//   caller. No 0x0129FB84 store: the destructor is the implicit one. The catalog label "CTCInventoryItem::OnDie" is propagated.
// Callees: free 0x00BFEA14, std::_List_base<CEnginePrimitiveSpriteTrail::CSpriteTrailEntry, allocator<..> >::~_List_base
//   0x00B09E40 (this TU's COMDAT grades RELOCATION_MATCH at 0x00B09E40: STLport `~list() {}` inlines, ~_List_base stays out
//   of line, chunk 698), NParticleEngine::CPSCSplineBase::~CPSCSplineBase 0x00B0A060 (implicit as well; VC7.1 keeps the
//   111-byte base body out of line on its own).
// Layout (Ego_r.pdb re-laid with STLport 4.6 containers): CPSCSplineBase 0x34 (the landed 0x00B0A060 model), the
//   CPSCSingleSprite members from +0x34 (Ego_r +0x38), TrailUpdatePrimitive CEnginePrimitiveSpriteTrail +0xAC (0x80 bytes:
//   CEnginePrimitive 0xC, the 4-byte Trail list +0xC = +0xB8 here), PrimIDList CArray<long> +0x12C.
// Shape: members in reverse order; the base ~CPSCSplineBase as the tail jump. DestroyOne (a destruction by name, ungraded)
//   makes VC7.1 emit this out-of-line implicit `??1`.
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14

namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
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
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;
    ~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }
    void clear();
protected:
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;
};
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    ~list() { }
};
}   // namespace std

#include "engine/CBaseClassNonCopyable.h"           // ~CBaseClassNonCopyable 0x0099A300 (protected virtual)
#include "engine/CCountedPointer.h"

#pragma pack(push, 1)
#include "engine/C3DVector.h"
#pragma pack(pop)

template <class T> class CArray : public std::vector<T> { };   // Lionhead CArray over STLport vector (0xc)

class CCardinalSpline {
public:
    CArray<C3DVector> ControlPoints;                    // +0x00
    float             Tension;                          // +0x0c
    bool              EndPointsConnected;               // +0x10
};

class CRGBColour {
public:
    unsigned char B, G, R, A;
};

class CMatrix3x4 {
public:
    float M[12];
};

enum EEnginePrimitiveType { EEPT_NONE = 0 };
enum EEngineInterpolationSet { EEIS_NONE = 0 };
enum EEnginePrimitiveBlendMode { EEPBM_NONE = 0 };
enum EEnginePrimitiveBlendOp { EEPBO_NONE = 0 };
class CEnginePrimitiveThemeChannelDesc {
public:
    unsigned long Desc;
};

class CEnginePrimitive {                                // Ego_r.pdb, 0xc, no vptr
public:
    EEnginePrimitiveType             Type;              // +0x00
    EEngineInterpolationSet          InterpolationSet;  // +0x04
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;  // +0x08
};

class CEnginePrimitiveSpriteTrail : public CEnginePrimitive {   // retail 0x80 (STLport 4-byte list)
public:
    class CSpriteTrailEntry {
    public:
        C3DVector Position;
        float     Width;
    };
    CEnginePrimitiveSpriteTrail();
    std::list<CSpriteTrailEntry> Trail;                 // +0x0c
    bool                      UpdateExisting;           // +0x10
    unsigned long             MaxLength;                // +0x14
    unsigned long             GraphicIndex;             // +0x18
    unsigned long             GraphicFrame;             // +0x1c
    float                     SkipPoints;               // +0x20
    float                     Width;                    // +0x24
    CRGBColour                HeadColour;               // +0x28
    CRGBColour                MidColour;                // +0x2c
    CRGBColour                TailColour;               // +0x30
    float                     FadeStart;                // +0x34
    C3DVector                 OffsetVector;             // +0x38
    unsigned short            Flags;                    // +0x44
    EEnginePrimitiveBlendMode BlendMode;                // +0x48
    EEnginePrimitiveBlendOp   BlendOp;                  // +0x4c
    CMatrix3x4                WorldTransform;           // +0x50
};

namespace NParticleEngine {
class CParticleSystem;
template <class T1, class T2> class CParticleUpdateParameters;
template <class T1, class T2> class CParticleUpdateParamCallback : public CBaseClassNonCopyable {
public:
    virtual bool GetUpdateParam(C3DVector& param, unsigned long index, float time, bool flag,
                                const CParticleUpdateParameters<T1, T2>& parameters, unsigned long extra);
};
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool             UpdateStarted : 1;                 // +0x08
    bool             Initialised : 1;                   // +0x08
};
class CPSCSplineBase : public CPSCBase {                // vtable 0x012A0B14
public:
    class CSplineRandomOffsetData;
    class CSplinePosParamData;
    CPSCSplineBase();
    float                                     SplineAnimSpeed;          // +0x0c
    CCardinalSpline                           Spline;                   // +0x10
    CCountedPointer<CSplineRandomOffsetData>  SplineRandomOffsetData;   // +0x24
    CCountedPointer<CSplinePosParamData>      SplinePosParamData;       // +0x2c
};
class CPSCSingleSprite : public CPSCSplineBase {        // vtable 0x0129FB84
public:
    CPSCSingleSprite();
    long        SpriteBankIndex;                        // +0x34
    CRGBColour  StartColour;                            // +0x38
    CRGBColour  MidColour;                              // +0x3c
    CRGBColour  EndColour;                              // +0x40
    float       AnimationTimeSecs;                      // +0x44
    float       StartRenderSize;                        // +0x48
    float       EndRenderSize;                          // +0x4c
    long        SpriteAlignment;                        // +0x50
    long        NoCrossedSprites;                       // +0x54
    float       InitialAngle;                           // +0x58
    long        FadeInEndInteger;                       // +0x5c
    long        FadeOutBeginInteger;                    // +0x60
    float       AlphaFadeMinimum;                       // +0x64
    float       SizeFadeMinimum;                        // +0x68
    unsigned long PositionParam;                        // +0x6c
    long        BlendMode;                              // +0x70
    long        BlendOp;                                // +0x74
    long        TrailBlendMode;                         // +0x78
    long        TrailBlendOp;                           // +0x7c
    long        TrailBankIndex;                         // +0x80
    float       TrailWidth;                             // +0x84
    long        MaxTrailLength;                         // +0x88
    long        StayWithEmitterIntFactor;               // +0x8c
    bool        UseStartColour;                         // +0x90
    bool        UseMidColour;                           // +0x91
    bool        UseEndColour;                           // +0x92
    bool        SelfIlluminating : 1;                   // +0x93
    bool        AlphaFadeEnable : 1;
    bool        RotateAroundCentre : 1;
    bool        FaceMe2D : 1;
    bool        FaceMe3D : 1;
    bool        CrossedSprites : 1;
    bool        SizeFadeEnable : 1;
    bool        ForceAnimationTime : 1;
    bool        StayWithEmitter : 1;                    // +0x94
    bool        UsePosition;                            // +0x95
    bool        UseSplinePoints;                        // +0x96
    bool        EnteringTimelineEnd;                    // +0x97
    unsigned long LastUpdateFrame;                      // +0x98
    long        Lifetimer;                              // +0x9c
    C3DVector   Offset;                                 // +0xa0
    CEnginePrimitiveSpriteTrail TrailUpdatePrimitive;   // +0xac
    CArray<long> PrimIDList;                            // +0x12c
};
} // namespace NParticleEngine

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(NParticleEngine::CPSCSingleSprite* component)
{
    component->NParticleEngine::CPSCSingleSprite::~CPSCSingleSprite();
}