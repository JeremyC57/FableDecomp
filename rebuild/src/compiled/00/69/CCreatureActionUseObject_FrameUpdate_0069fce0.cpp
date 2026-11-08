// CCreatureAction_UseObject::FrameUpdate() -- finish at once when the used thing is gone; while using the dummy point: keep the base FrameUpdate running until the creature has stopped moving/turning to it, then (with no proximity left and a usable CTCActionUse on the thing) snap the creature's physics to the dummy facing and position; finally the per-frame slot-4 and slot-3 virtuals
// Leaf: ?FrameUpdate@CCreatureAction_UseObject@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069FCE0, length 369 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PUsedThing +0xa8, PCreature +8), this object's virtual slots 4 (+0x10) and
// 3 (+0x0c), CCreatureActionBase::FrameUpdate @ 0x00694020 (direct base call), CVectorMap::LowerBound (const instantiation) @ 0x004365B0
// (inlined const GetTC 0xD = CTCActionUse, bit 13 of +0x20), CTCActionUse::CanBeUsedFromDummy @ 0x007EABE0 (manifest
// "CTCShotDefinitionBase::CanBeHitByMeleeStrike" is propagated), _CIfmod @ 0x00BFEB28, the physics component's virtual slots 66
// (+0x108: facing fraction) and 30 (+0x78: position).
// Globals: 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 1/(2pi) 0x01237F08, the double 1.0 0x01237F00.
// Layout: engine/CCreatureAction_UseObject.h minus the PDB's 12 extra base bytes: PUsedThing +0xa8, Dummy +0xb4 (the facing axis at
// +0xc0), DummyPos +0xcc, Proximity +0xd8, UsingDummy +0xdc, MovingToDummy +0xdd, TurningToDummy +0xde.
// Shape: IS_THING_ALIVE on PUsedThing; UsingDummy is read as the member in both `if`s (one CSE'd load in al, two tests: VC7.1 does not thread the second);
// `Proximity == 0.0f` is `fucompp ; test ah,44h ; jp`; the facing fraction is the DuckAndCover 0x008B6EE0 atan2/fmod wrap.
// The CTCActionUse test reads the used thing ONCE (one GetPItem for both the presence bit and the lookup): a forceinline
// CanUseFromDummy(const CThing*) helper. The final SetPosition goes through a named creature local, so the physics
// pointer loads straight into ecx. Both fix the this/&PUsedThing esi/edi assignment. The physics vtable has
// SetFacingAngle at slot 66 (+0x108); the previous model declared one dummy slot too many.
#include "fable_compiler.h"
extern "C" double __cdecl atan2(double y, double x);
extern "C" double __cdecl fmod(double x, double y);
#pragma intrinsic(atan2, fmod)
class C3DVector { public: float X, Y, Z; };
class CTCBase;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCPhysicsBase { public:
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29)
    virtual void SetPosition(const C3DVector& pos);   // slot 30 (+0x78)
    VF10(3) VF10(4) VF10(5) VF(60) VF(61) VF(62) VF(63) VF(64)
    virtual void SetFacingAngle(float fraction);   // slot 66 (+0x108)
};

struct CTCActionUse {                           // interface type 0xD
    bool CanBeUsedFromDummy() const;            // 0x007EABE0
};
FABLE_FORCEINLINE bool CanUseFromDummy(const CThing* thing)
{
    return thing->HasTC(TCI_ON_ACTION_USE) && ((const CTCActionUse*)&thing->GetTC(TCI_ON_ACTION_USE))->CanBeUsedFromDummy();
}
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}
class CRightHandedSet { public:                        // 24 bytes: two axes
    C3DVector XAxis;                            // +0x00
    C3DVector YAxis;                            // +0x0c
};
class CCreatureActionBase { public:                    // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void UpdateMovement();              // slot 3 (+0x0c)
    virtual void FinishAction();                // slot 4 (+0x10)
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    void FrameUpdate();                         // 0x00694020
};
class CCreatureAction_UseObject : public CCreatureActionBase { public:
    CIntelligentPointer<CThing> PUsedThing;         // +0xa8
    CTCPhysicsBase*         PreviousPhysicsComponent;   // +0xb0
    CRightHandedSet         Dummy;              // +0xb4
    C3DVector               DummyPos;           // +0xcc
    float                   Proximity;          // +0xd8
    bool                    UsingDummy;         // +0xdc
    bool                    MovingToDummy;      // +0xdd
    bool                    TurningToDummy;     // +0xde
    bool                    AllowTurn;          // +0xdf
    virtual void FrameUpdate();
};

void CCreatureAction_UseObject::FrameUpdate()
{
    if (!IS_THING_ALIVE(PUsedThing)) {
        FinishAction();
        return;
    }
    if (UsingDummy) {
        if (MovingToDummy || TurningToDummy) {
            CCreatureActionBase::FrameUpdate();
            return;
        }
    }
    if (UsingDummy && Proximity == 0.0f && CanUseFromDummy(PUsedThing.Get())) {
        CTCPhysicsBase* physics = PCreature.Get()->GetPhysicsTC();
        float fraction = (float)atan2(Dummy.YAxis.X, Dummy.YAxis.Y) * 0.15915493667125702f;
        if (fraction < 0.0f || fraction >= 1.0f) {
            fraction = (float)fmod(fraction, 1.0);
            if (fraction < 0.0f)
                fraction += 1.0f;
        }
        physics->SetFacingAngle(fraction);
        CThing* creature = PCreature.Get();
        creature->GetPhysicsTC()->SetPosition(DummyPos);
    }
    FinishAction();
    UpdateMovement();
}