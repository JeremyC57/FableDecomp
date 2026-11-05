// NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::~CModeIceTrollGroundSmashIcicleAssault() -- implicit ice-troll icicle-assault creature-mode destructor: destroy ViablePoints, Icicles, PDef, Filter and PTarget, then ~CModeStandBase
// Leaf: ??1CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743FE0, int3 padding before and after, thiscall, tail `jmp ~CModeStandBase` 0x0073D860.
// Identity (proven, two sources): (1) its sole caller is the scalar deleting destructor 0x00743FC0, slot 0 of vtable 0x01268774
//   (RTTI .?AVCModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@ : CModeStandBase : CModeBase); (2) Ego_r.pdb 0x00756A20 /
//   FableWin.pdb 0x024BAA60 ??1CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@UAE@XZ. No vtable store: compiler-generated.
//   The catalog label "CNewFrontendGameComponent::~CNewFrontendGameComponent" is wrong.
// Callees: free 0x00BFEA14 (ViablePoints storage), ~vector<CIcicleData> out of line (VC7.1 does not inline it, as retail; this TU's
//   COMDAT also matches 0x00743A90), CAnimFilterBase::~CAnimFilterBase inline (vptr store, CCharString Group dtor 0x0099EAE0),
//   ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10 (PTarget), ??1CModeStandBase@@UAE@XZ 0x0073D860 (tail; RTTI .?AVCModeStandBase@@, global, and both PDBs spell it so).
// Shape: members in reverse order; the PDB ctor (body not graded) is defined so the vtable, ??_G (RELOCATION_MATCH vs 0x00743FC0)
//   and this ??1 are emitted.
// Layout: Ego_r.pdb members PTarget +0x68 .. ViablePoints +0x90 (STLport vectors 12 bytes), retail sizeof 0x9C; CModeStandBase is
//   engine/CModeStandBase.h FABLE_RETAIL_CMODESTANDBASE (chunk 710). CAnimFilterBase slots from retail vtables 0x0122FFCC /
//   0x01267568: operator() 0, Clone 1, ??_G 2 (chunk 710 had the dtor at slot 0). CTCCreatureModeManager here is the minimal
//   declaration CModeStandBase.h needs (its members are opaque storage at their Ego_r offsets).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"

class CThing;
class CBaseObject;
class CBaseObjectPointer;
class CAnimateThing;
class CAnimationEntry;
enum EMovementType;

#include "engine/C3DVector.h"

namespace std {
template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

#pragma pack(push, 1)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(pop)

class CAnimFilterBase {
public:
    CAnimFilterBase() { }
    virtual bool operator()(const CAnimationEntry& anim) const = 0;   // slot 0
    virtual CAnimFilterBase* Clone() const = 0;             // slot 1
    virtual ~CAnimFilterBase() { }                          // slot 2
};
class CAnimFilterOfGroup : public CAnimFilterBase {
public:
    CAnimFilterOfGroup(const CCharString& group);
    virtual bool operator()(const CAnimationEntry& anim) const;
    virtual CAnimFilterBase* Clone() const;
    CCharString   Group;                                    // +0x04
    unsigned long GroupCRC;                                 // +0x08
};

class CTCCreatureModeManager : public CTCBase {      // vtable 0x01268854, sizeof 0x24
public:
    class CModeBase {
    public:
        CModeBase(CThing& thing);
        virtual ~CModeBase();
        virtual unsigned long GetType() const = 0;
        virtual void AnimUpdate(CAnimateThing& animateThing);
        virtual void FrameUpdate();
        virtual const CAnimFilterBase& GetAnimFilter() const;
        virtual void OnActivate();
        virtual void OnDeactivate();
        virtual float GetMovementSpeed(const C3DVector& from, const C3DVector& to, EMovementType type) const;
        virtual bool IsIdleMode() const;
        virtual bool IsRemoveOnDeactivate() const;
        virtual bool IsHoldWeaponWithBothHands() const;
        virtual bool IsAllowingPlayerSpecialMovementMoves() const;
        CThing*            ParentThing;                     // +0x04
        CAnimFilterOfGroup WeaponBasedFilter;               // +0x08
        long               FramesBeforeRemovingSelf;        // +0x14
        bool               Active;                          // +0x18
    };
    CTCCreatureModeManager(CThing& thing);
    virtual ~CTCCreatureModeManager();                      // 0x0073AEA0
    unsigned char PModes_[0x4];                             // std::list<CCountedPointer<CModeBase> > PModes +0x0c
    long          TimeLeftForCurrentMode;
    long          TimeForNextIdleMode;
    unsigned char AvailableIdleModes_[0xc];                 // std::vector<NCreatureMode::EMode> AvailableIdleModes +0x18
};

#define FABLE_RETAIL_CMODESTANDBASE
#include "engine/CModeStandBase.h"

class CSnowTrollDef;
namespace NCreatureMode {
class CModeIceTrollGroundSmashIcicleAssault : public CModeStandBase {
public:
    class CIcicleData {
    public:
        CIcicleData();
        CIntelligentPointer<CThing> PEffect;                // +0x00
        CIntelligentPointer<CThing> PTrap;                  // +0x08
        long                        EffectTimer;            // +0x10
        long                        TrapTimer;              // +0x14
        C3DVector                   Position;               // +0x18
    };
    CModeIceTrollGroundSmashIcicleAssault(CThing& thing);
    virtual unsigned long GetType() const;
    CIntelligentPointer<CThing>        PTarget;             // +0x68
    CAnimFilterOfGroup                 Filter;              // +0x70
    long                               Timer;               // +0x7c
    CDefPointer<const CSnowTrollDef>   PDef;                // +0x80
    std::vector<CIcicleData>           Icicles;             // +0x84
    std::vector<C3DVector>             ViablePoints;        // +0x90
};
FABLE_STATIC_ASSERT(sizeof(CModeIceTrollGroundSmashIcicleAssault) == 0x9c);

CModeIceTrollGroundSmashIcicleAssault::CModeIceTrollGroundSmashIcicleAssault(CThing& thing)
    : CModeStandBase(thing), Filter(CCharString())
{
}
}