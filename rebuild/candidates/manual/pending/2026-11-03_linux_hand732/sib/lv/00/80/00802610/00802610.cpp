// CTCTargetingPlayer::GetPTargetedThing() const -- in first-person targeting the projectile-targeted thing (virtual), otherwise the base class's targeted thing
// Leaf: ?GetPTargetedThing@CTCTargetingPlayer@@UBEPAVCThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00802610, 42 bytes (int3 padding 0x0080263A..F, next function 0x00802640), thiscall,
//   tails: `jmp [vtbl+8Ch]` (slot 35) or `lea ecx,[esi+25h] ; jmp GetPItem`.
// Identity (proven, two sources): (1) the only reference is slot 33 of the CTCTargetingPlayer vtable 0x0124280C (RTTI
//   .?AVCTCTargetingPlayer@@ : CTCTargetingBase : CTCBase); CTCTargetingBase's own slot 33 (vtable 0x01271E74) is
//   0x004DDAAC `add ecx,25h ; jmp GetPItem` = `return PTargetedThing.GetPItem();`, the base GetPTargetedThing;
//   (2) Ego_r.pdb ?GetPTargetedThing@CTCTargetingPlayer@@UBEPAVCThing@@XZ (const virtual) with the donor order
//   GetPTargetedThing 0x007FD4F0 / GetPBestThingForProjectileTargetingLock 0x007FD520 mirrored at retail 0x00802610 / 0x00802640
//   (slot 30); the landed GetTargetedPos 0x00802420 (slot 37) calls slot 33 first. The forge label is a stand-in.
// Slots 28-39 (vtable 0x0124280C, base bodies by `ret N`): 28 GetPBestThingForMeleeAttack 0x008031E0, 29
//   GetPBestEnemyCreatureForAttack 0x004DEBFC, 30 GetPBestThingForProjectileTargetingLock 0x00802640, 31
//   GetPBestCreatureForInteractionTypeWithinRangeLimit 0x004DEBFF (`ret 0Ch`), 32 GetPBestCreatureForInteractionType 0x004DEC04
//   (`ret 4`), 33 GetPTargetedThing (this row), 34 SetPTargetedThing 0x00802E00, 35 GetPProjectileTargetedThing 0x004E231D
//   (+0xDC), 36 GetPTargetLockedThing 0x004E2328 (+0xE4), 37 GetTargetedPos 0x00802420, 38 IsTargetedThingInRange 0x008039A0,
//   39 UpdateNearestInterestingThing 0x008023E0 (protected). Slot 35 = PProjectileTargetedThing: Ego_r +228 less the two
//   4-byte-narrower STLport vectors (Targets / RejectedTargetPoints) = +0xDC, engine/CTCTargetingPlayer.h's retail +0xDC.
// Callees: CThing::PeekPlayer() const 0x004C7A10 (?PeekPlayer@CThing@@QBEABVCPlayer@@XZ), CPlayer::IsFirstPersonTargeting() const
//   0x004874D0 (?IsFirstPersonTargeting@CPlayer@@QBE_NXZ), CBaseIntelligentPointer::GetPItem() const 0x00A01B50
//   (?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ, both PDBs; CIntelligentPointer<T>::GetPItem is inline over it).
// Layout: engine/CTCBase.h retail opt-in (FABLE_RETAIL_CTCBASE: ParentThing +0x04); Ego_r CTCTargetingBase {ScriptOverrideIgnoreEverything
//   +0x0C, PNearestInterestingThing +0x0D, PScriptOverrideInterestingThing +0x15, PScriptOverrideRangedTarget +0x1D, PTargetedThing
//   +0x25, PTargetingDef +0x30, LookAtCamera +0x34} (no engine header; byte-packed CIntelligentPointer, pack(1) on the pointer
//   classes as chunk 700/703); engine/CTCTargetingPlayer.h is flattened (vptr as data), so the player members this row reads
//   are restated here at its retail offsets. Every retail member of both classes is present (Ego_r order, real types incl.
//   NPlayerTargeting::CTargetPoint {PThing, Index, TargetingPosition, FrameRejected} = 0x1C), offsets from the retail ctor 0x004E2250
//   (Targets +0x68, CurrentZTargetIndex +0x80 = -1, TargetedPos +0x84, the four pointers +0xD4/+0xDC/+0xE4/+0xEC, LockedOntoEnemy
//   +0xF4): Ego_r offsets from +0x68 on less 8 (two 4-byte-narrower STLport vectors), sizeof 0xF8.
//   CPlayer comes from engine/CPlayer.h under the chunk 706 opt-in FABLE_RETAIL_PLAYER (declares IsFirstPersonTargeting; hdr/ overlay).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"

namespace std {
template <class _Tp> class allocator { public: };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { };
}

class CBaseObject;
class CBaseObjectPointer;
class C3DVector {
public:
    float X;
    float Y;
    float Z;
};
class C2DVector {
public:
    float X;
    float Y;
};
class CTargetingDef;
class CThingCreatureBase;
class CDefPointerStandIn;
enum ECreatureInteractionType { };

#define FABLE_RETAIL_PLAYER                                   // chunk 706 opt-in: declares IsFirstPersonTargeting (layout unchanged)
#include "engine/CPlayer.h"

class CThing {
public:
    const CPlayer& PeekPlayer() const;                      // 0x004C7A10
};

#pragma pack(push, 1)
class CBaseIntelligentPointer {                             // vtable 0x0129C95C: vptr + PData, 8 bytes, 1-aligned in retail
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
private:
    CBaseObjectPointer* PData;                              // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {      // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
#pragma pack(pop)

class CTCTargetingBase : public CTCBase {                    // RTTI .?AVCTCTargetingBase@@, 0x38 bytes
public:
    virtual CThing* GetPBestThingForMeleeAttack() const;                                         // 28
    virtual CThingCreatureBase* GetPBestEnemyCreatureForAttack() const;                          // 29
    virtual CThing* GetPBestThingForProjectileTargetingLock() const;                             // 30
    virtual CThingCreatureBase* GetPBestCreatureForInteractionTypeWithinRangeLimit(ECreatureInteractionType type, float minRange, float maxRange) const;   // 31
    virtual CThingCreatureBase* GetPBestCreatureForInteractionType(ECreatureInteractionType type) const;   // 32
    virtual CThing* GetPTargetedThing() const { return PTargetedThing.GetPItem(); }             // 33 (out of line 0x004DDAAC)
    virtual void SetPTargetedThing(CThing* thing);                                               // 34
    virtual CThing* GetPProjectileTargetedThing() const;                                         // 35
    virtual CThing* GetPTargetLockedThing() const;                                               // 36
    virtual bool GetTargetedPos(C3DVector& pos, float t) const;                                  // 37
    virtual bool IsTargetedThingInRange(unsigned long range) const;                              // 38
protected:
    virtual void UpdateNearestInterestingThing();                                                // 39
public:
    bool                        ScriptOverrideIgnoreEverything;     // +0x0C
    CIntelligentPointer<CThing> PNearestInterestingThing;           // +0x0D
    CIntelligentPointer<CThing> PScriptOverrideInterestingThing;    // +0x15
    CIntelligentPointer<CThing> PScriptOverrideRangedTarget;        // +0x1D
    CIntelligentPointer<CThing> PTargetedThing;                     // +0x25
    CDefPointer<const CTargetingDef> PTargetingDef;                 // +0x30 (engine/CDefPointer.h)
    bool                        LookAtCamera;                       // +0x34
};

namespace NPlayerTargeting {
class CTargetPoint {                                         // Ego_r 0x1C
public:
    CIntelligentPointer<CThing> PThing;                     // +0x00
    long                        Index;                      // +0x08
    C3DVector                   TargetingPosition;          // +0x0C
    long                        FrameRejected;              // +0x18
};
}

class CTCTargetingPlayer : public CTCTargetingBase {         // vtable 0x0124280C
public:
    virtual CThing* GetPTargetedThing() const;
    enum ETargetNextThingMethod { };                         // W4ETargetNextThingMethod (enumerators unproven)
    long                                    ZLockTargetSelectionTimer;      // +0x38
    bool                                    TargetNextThing;                // +0x3C
    bool                                    ForcedToHaveNoTarget;           // +0x3D
    ETargetNextThingMethod                  TargetNextThingMethod;          // +0x40
    long                                    FrameSetAsTargetNextThing;      // +0x44
    float                                   MaxTargetingDistance;           // +0x48
    NPlayerTargeting::CTargetPoint          TargetedPoint;                  // +0x4C
    std::vector<NPlayerTargeting::CTargetPoint> Targets;                    // +0x68
    std::vector<NPlayerTargeting::CTargetPoint> RejectedTargetPoints;       // +0x74
    long                                    CurrentZTargetIndex;            // +0x80
    C3DVector                               TargetedPos;                    // +0x84
    C3DVector                               OldTargetedPos;                 // +0x90
    C3DVector                               TargetedPosOffset3D;            // +0x9C
    C3DVector                               OldTargetedPosOffset3D;         // +0xA8
    C2DVector                               TargetedPosOffset;              // +0xB4
    C2DVector                               DeltaTargetedPosOffset;         // +0xBC
    C2DVector                               DesiredTargetedPosOffset;       // +0xC4
    C2DVector                               TargetedPosOffsetSecondOrder;   // +0xCC
    CIntelligentPointer<CThing>             PLastThreatenedThing;           // +0xD4
    CIntelligentPointer<CThing>             PProjectileTargetedThing;       // +0xDC
    CIntelligentPointer<CThing>             PTargetLockedThing;             // +0xE4
    CIntelligentPointer<CThing>             PTargetOnlyThing;               // +0xEC
    bool                                    LockedOntoEnemy;                // +0xF4
};

FABLE_STATIC_ASSERT(offsetof(CTCTargetingBase, PTargetingDef) == 0x30);
FABLE_STATIC_ASSERT(sizeof(CTCTargetingBase) == 0x38);
FABLE_STATIC_ASSERT(sizeof(NPlayerTargeting::CTargetPoint) == 0x1C);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, Targets) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, CurrentZTargetIndex) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, PProjectileTargetedThing) == 0xDC);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, LockedOntoEnemy) == 0xF4);
FABLE_STATIC_ASSERT(sizeof(CTCTargetingPlayer) == 0xF8);

CThing* CTCTargetingPlayer::GetPTargetedThing() const
{
    if (ParentThing->PeekPlayer().IsFirstPersonTargeting())
        return GetPProjectileTargetedThing();
    return CTCTargetingBase::GetPTargetedThing();
}