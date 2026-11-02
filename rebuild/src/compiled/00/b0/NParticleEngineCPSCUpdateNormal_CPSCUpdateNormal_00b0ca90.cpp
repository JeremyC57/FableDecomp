// NParticleEngine::CPSCUpdateNormal::~CPSCUpdateNormal() -- store the update-normal component vtable, kill every decal emitter, destroy the decal-emitter list and the emitter name, then the inline CPSCBase chain and ~CBaseClassNonCopyable
// Leaf: ??1CPSCUpdateNormal@NParticleEngine@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0CA90, 60 bytes, thiscall, tail `jmp ~CBaseClassNonCopyable` 0x0099A300; no
//   callers (the slot-0 `??_G` 0x00B0F990 of vtable 0x0129FC1C holds the same body inline).
// Identity (proven): the first store is vtable 0x0129FC1C (RTTI .?AVCPSCUpdateNormal@NParticleEngine@@ : CPSCBase :
//   CParticleUpdateParamCallback<KK> : CBaseClassNonCopyable : CBaseClass); Ego_r.pdb / FableWin.pdb
//   ??1CPSCUpdateNormal@NParticleEngine@@UAE@XZ (a user dtor: the store and the KillAllDecalEmitters body), the body
//   ??_GCPSCUpdateNormal 0x00B0F990 inlines (landed). The catalog label CExclusiveAssert::~CExclusiveAssert is propagated.
// Callees: CPSCUpdateNormal::KillAllDecalEmitters 0x00B0C730 (?KillAllDecalEmitters@CPSCUpdateNormal@NParticleEngine@@IAEXXZ:
//   protected in both PDBs; the bound QAEXXZ is a stand-in, NAME_FIXES.tsv), std::_List_base<CIntelligentPointer<NParticleEngine::
//   CParticleEmitter> >::clear 0x00B0F630 (landed spelling), free 0x00BFEA14 (the sentinel), CCharString::~CCharString 0x0099EAE0,
//   CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (MAE, tail).
// Layout = Ego_r.pdb CPSCUpdateNormal (no container before DecalEmitterList, so the offsets are retail's; 0xa0 with the 4-byte
//   STLport list): CPSCBase 0xc, the ulong bit-field word +0xc, the bool bit-field byte +0x10, DecalEmitterName +0x14, the
//   floats +0x18..+0x50, five C3DVectors +0x54..+0x84, AccelerationParam +0x90, OrientFromGameParam +0x94, LifeTimer +0x98,
//   DecalEmitterList +0x9c.
// Shape: own vtable store, the user body, then the members in reverse order: the real STLport 4.6 list (`~list() {}`, inline
//   ~_List_base with the out-of-class clear() kept out of line -- member address in edi) and the string; CPSCBase /
//   CParticleUpdateParamCallback declare no destructor (no store of their own: the landed CPSCAttractor 0x00B18780 chain), so
//   the tail is ~CBaseClassNonCopyable.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
#include "engine/CBaseClassNonCopyable.h"                  // ~CBaseClassNonCopyable 0x0099A300 (protected virtual)

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                            // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                             // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                    // 0x00A01C10 ??1CBaseIntelligentPointer@@UAE@XZ
    CBaseObject* GetPItem() const;                         // 0x00A01B50
private:
    CBaseObjectPointer* PData;                             // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

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
struct _List_node_base {
    _List_node_base* _M_next;                              // +0x0
    _List_node_base* _M_prev;                              // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                           // +0x8
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;
    ~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }
    void clear();
protected:
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;   // +0x0: the sentinel node
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

class CCharStringData;
class CCharString {
public:
    ~CCharString();                                        // 0x0099EAE0
    CCharStringData* PStringData;
};

#pragma pack(push, 1)
class C3DVector {                                          // retail C3DVector is 1-aligned
public:
    float X, Y, Z;
};
#pragma pack(pop)

namespace NParticleEngine {
class CParticleSystem;
class CParticleEmitter;
template <class T1, class T2> class CParticleUpdateParameters;
template <class T1, class T2> class CParticleUpdateParamCallback : public CBaseClassNonCopyable {
public:
    virtual bool GetUpdateParam(C3DVector& param, unsigned long index, float time, bool flag,
                                const CParticleUpdateParameters<T1, T2>& parameters, unsigned long extra);
};
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {   // vtable 0x0129F3F4
public:
    CParticleSystem* ParticleSystem;                       // +0x04
    bool             UpdateStarted : 1;                    // +0x08
    bool             Initialised : 1;                      // +0x08
};
class CPSCUpdateNormal : public CPSCBase {                 // vtable 0x0129FC1C, 0xa0 bytes in retail
public:
    CPSCUpdateNormal();
    virtual ~CPSCUpdateNormal();
protected:
    void KillAllDecalEmitters();                           // 0x00B0C730
public:
    unsigned long FadeInEndInteger : 10;                   // +0x0c
    unsigned long FadeOutBeginInteger : 10;
    unsigned long UseParticleLifeSecs : 1;
    unsigned long UseRandomRotationAxis : 1;
    unsigned long UseRandomInitialRotation : 1;
    unsigned long UseAccelerationParam : 1;
    unsigned long StayWithEmitter : 1;
    unsigned long UseSystemLifeSecs : 1;
    unsigned long SystemAlphaFadeEnable : 1;
    unsigned long EmissionFadeEnable : 1;
    unsigned long ParticleCollideWithGround : 1;
    unsigned long ParticleDieOnCollision : 1;
    unsigned long UseAllAttractors : 1;
    unsigned long RandomisePosEnable : 1;
    bool          RandomisePosDistVaryEnable : 1;          // +0x10
    bool          SetOrientationFromDirection : 1;
    bool          SetOrientationFromGame : 1;
    bool          ParticleCreateDecal : 1;
    bool          ParticleCreateDecalEmitter : 1;
    bool          ParticleCollideWithAnything : 1;
    CCharString   DecalEmitterName;                        // +0x14
    float         SystemLifeSecs;                          // +0x18
    float         ParticleLifeSecs;                        // +0x1c
    float         WindFactor;                              // +0x20
    float         GravityFactor;                           // +0x24
    float         AirResistance;                           // +0x28
    float         ParticleAccelerationScale;               // +0x2c
    float         InitialRotationX;                        // +0x30
    float         InitialRotationY;                        // +0x34
    float         InitialRotationZ;                        // +0x38
    float         RotationMinAngleSpeed;                   // +0x3c
    float         RotationMaxAngleSpeed;                   // +0x40
    float         ParticleBounce;                          // +0x44
    float         SystemAlphaFadeMinimum;                  // +0x48
    float         RandomisePosDistVaryScale;               // +0x4c
    float         EmissionFadeMinimum;                     // +0x50
    C3DVector     ParticleSystemOffset;                    // +0x54
    C3DVector     ParticleAcceleration;                    // +0x60
    C3DVector     RotationAxis;                            // +0x6c
    C3DVector     RandomisePosSpeed;                       // +0x78
    C3DVector     RandomisePosScale;                       // +0x84
    unsigned long AccelerationParam;                       // +0x90
    unsigned long OrientFromGameParam;                     // +0x94
    long          LifeTimer;                               // +0x98
    std::list<CIntelligentPointer<CParticleEmitter> > DecalEmitterList;   // +0x9c
};

FABLE_STATIC_ASSERT(sizeof(CPSCUpdateNormal) == 0xa0);

CPSCUpdateNormal::~CPSCUpdateNormal()
{
    KillAllDecalEmitters();
}
} // namespace NParticleEngine