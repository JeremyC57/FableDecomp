// Grade: DIFFER(948v948) -- one instruction left: retail schedules the next GetPItem's mov ecx,edi right after the first fld of the Newton step, the lane four x87 ops later (15 tries)
// CAIStateGroup_HoverHornetQueen::Attack() -- register the queen as a ready combatant, fly (facing the target) to the combat manager's preferred position pulled onto the ground and back out to the original distance while the target and the first circle marker are valid, then perform the dragon roar and keep looking at the target for the def's look time
// Layout: retail STLport vectors are 12 bytes, so CircleMarkers is +0x50 (PDB header +0x54).
#include <math.h>
#pragma intrinsic(sqrt)
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    __forceinline C3DVector& operator*=(const float& s) { X *= s; Y *= s; Z *= s; return *this; }
    __forceinline void Normalise() { *this *= FastInvSqrt(Z * Z + Y * Y + X * X); }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
__forceinline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
__forceinline C3DVector operator*(const C3DVector& a, float s) { return C3DVector(a.X * s, a.Y * s, a.Z * s); }
struct CAICreatureWillPowerIndicatorDef {
    virtual void v00();
    virtual void DeleteThis();
    long RefCount;                              // +0x04
    unsigned char _pad08[0x34];
    long LookTime;                              // +0x3c
};
struct CDefClassBase {
    void GetSubDef(CAICreatureWillPowerIndicatorDef** out);   // 0x0095C5C0
};
#pragma auto_inline(off)
void CDefClassBase::GetSubDef(CAICreatureWillPowerIndicatorDef** out) { *out = 0; }
#pragma auto_inline(on)
struct CCombatManager;
struct CThing {
    unsigned char _pad0[0x70];
    CDefClassBase* PDef;                        // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                      // +0x91
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CThingCreatureBase : CThing {
    CCombatManager* GetCombatManager();         // 0x00661ED0
    bool IsCombatant();                         // 0x00662410
};
struct CThingAICreature : CThingCreatureBase { };
bool __fastcall IsToAttackTarget(CThingCreatureBase* creature, CThing* target);   // 0x00864D00
struct CCombatManager {
    void SetAttackerAsReadyToAttack(CThing* creature, bool ready);         // 0x006EA450
    C3DVector GetPreferredPos(CThingCreatureBase* creature);               // 0x006EA770
    void RegisterCombatant(CThing* creature, CThing* target, bool flag);   // 0x006EDC50
};
struct CWorldMap {
    float GetGroundSizeZAt(const C3DVector& pos) const;   // 0x0063A300
};
struct CWorld {
#define VF(n) virtual void v##n();
    VF(0) VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9) VF(10) VF(11)
    virtual CWorldMap* GetWorldMap();           // slot 12 (+0x30)
    unsigned char _pad[0x48];
    CCombatManager* PCombatManager;             // +0x4c
    static long GetWorldFrame();                // 0x0049D870
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    virtual void v00();
};
struct CIntelligentPointerConstThing {
    void* vptr;
    const CThing* m_pItem;
    CIntelligentPointerConstThing(const CIntelligentPointerThing& other);   // 0x0091F990
};
template <class T> struct CVec {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x110];
};
struct CCombatActionBase : CCreatureActionBase {
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCreatureAction_DragonRoar : CCombatActionBase {
    CCreatureAction_DragonRoar(CThingCreatureBase* creature, CThing* target);   // 0x0095CA00
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    bool IsStopping();                          // 0x008FD010
    void Terminate();                           // 0x008FCF40
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long flags, float tolerance);   // 0x0097D6F0
    long AIAction_CombatPerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DA40
};
struct CAIStateGroup_HoverMovementBase : CAIStateGroupBase {
    long AIAction_MoveToPosProximityAndFacePos(long repeat, const C3DVector& pos, float proximity,
        const C3DVector& facePos, bool& arrived, long movementType);   // 0x0091D510
};
struct CAIStateGroup_HoverHornetQueen : CAIStateGroup_HoverMovementBase {
    CIntelligentPointerThing PTarget;           // +0x14
    unsigned char _pad1c[0x40 - 0x1c];
    CVec<CIntelligentPointerThing> Things;      // +0x40
    long CurrentCirclePoint;                    // +0x4c
    CVec<CIntelligentPointerThing> CircleMarkers;   // +0x50
    bool IsStillRunnable();                     // 0x0095A420
    void Attack();
};

void CAIStateGroup_HoverHornetQueen::Attack()
{
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    CCombatManager* combat = GetCreature()->GetCombatManager();
    if (!GetCreature()->IsCombatant()) {
        CThingAICreature* creature = GetCreature();
        combat->RegisterCombatant(creature, PTarget.GetPItem(), true);
    }
    if (GetCreature()->IsCombatant()) {
        CThing* creature = GetCreature();
        CWorld* world = GetPWorld();
        world->PCombatManager->SetAttackerAsReadyToAttack(creature, true);
    }
    long result;
    do {
        if (!PTarget.GetPItem() || (PTarget.GetPItem()->Flags91 & 1)) {
            Terminate();
            return;
        }
        if (CircleMarkers.size() > 0) {
            CIntelligentPointerThing& first = *CircleMarkers._M_start;
            if (!first.GetPItem() || (first.GetPItem()->Flags91 & 1)) {
                Terminate();
                return;
            }
        }
        if (IsStopping() || !GetCreature()->IsCombatant()) {
            Terminate();
            return;
        }
        if (!IsToAttackTarget(GetCreature(), PTarget.GetPItem())) {
            Terminate();
            return;
        }
        C3DVector pos = combat->GetPreferredPos(GetCreature());
        float distance = (pos - PTarget.GetPItem()->GetPosition()).GetLength();
        pos.Z = GetPWorld()->GetWorldMap()->GetGroundSizeZAt(pos);
        C3DVector direction = pos - PTarget.GetPItem()->GetPosition();
        direction.Normalise();
        pos = PTarget.GetPItem()->GetPosition() + direction * distance;
        bool arrived;
        result = AIAction_MoveToPosProximityAndFacePos(2, pos, 0.05f, PTarget.GetPItem()->GetPosition(), arrived, 9);
    } while (result == 4);
    if (result != 2)
        return;
    if (!IsStillRunnable() || !GetCreature()->IsCombatant()) {
        Terminate();
        return;
    }
    {
        CThingAICreature* creature = GetCreature();
        AIAction_CombatPerformActionAndWait(&CCreatureAction_DragonRoar(creature, PTarget.GetPItem()), false);
    }
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    long start = GetPWorld()->GetWorldFrame();
    CAICreatureWillPowerIndicatorDef* def = 0;
    GetCreature()->PDef->GetSubDef(&def);
    CAICreatureWillPowerIndicatorDef* d = def;
    if (d->LookTime > 0) {
        do {
            AIAction_LookAtThing(2, PTarget, 0x21, 0.0027777001f);
            if (!IsStillRunnable())
                break;
        } while (GetPWorld()->GetWorldFrame() - start < d->LookTime);
    }
    if (--d->RefCount == 0)
        d->DeleteThis();
    if (!IsStillRunnable())
        Terminate();
}
