// CAIStateGroupBase::AIAction_MoveToThingProximity(EAIActionRepeat, CIntelligentPointer<const CThing>, float, bool&, EMovementType, bool, bool, bool, bool, float) -- steer the creature along its navigation route to within `proximity` of a live thing (using the route's end point when it lies outside the creature's reach but near the thing), one brain output per tick, until arrived (2), failed or the thing died (3) or stopped (1)
// Grade: DIFFER(1240v1286) nd=299 (unchanged) -- local CCountBlock/CCountedPointer model moved onto engine/CCountedPointer.h (CCountedPointer<CCreatureActionBase>), byte-neutral; residue: register allocation (retail this in edi, navigation in ebx from the frame word it shares with trackInteresting, face target in ebp)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097E380, 1296 bytes (ret 2Ch). Thing twin of the landed AIAction_MoveToPosProximity 0x0097E890.
// Callees: as 0x0097E890 plus CBaseIntelligentPointer::GetPItem 0x00A01B50 / dtor 0x00A01C10, CVectorMap::LowerBound 0x0040F020,
//   CThing::GetPosition 0x004C73D0, the navigation route queries 0x006CB990 / 0x006CB9A0 (catalog labels propagated),
//   ~CBrainOutput 0x006D60C0 (out of line). Constants 72.25f (8.5^2) @ 0x012995BC, 12.25f (3.5^2) @ 0x0128FFFC.
#include "engine/CCountedPointer.h"
struct CThing;
struct C3DVector {
    float X, Y, Z;
    float GetLengthSquared() const { return X * X + (Y * Y + Z * Z); }
};
C3DVector operator+(const C3DVector& a, const C3DVector& b);
inline C3DVector operator+(const C3DVector& a, const C3DVector& b)
{
    C3DVector r;
    r.X = a.X + b.X;
    r.Y = a.Y + b.Y;
    r.Z = a.Z + b.Z;
    return r;
}
extern const float EPSILON;                     // 0x0129BA3C (0.0001f)

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CNavigationInput {
    C3DVector Destination;                      // +0x00
    float Proximity;                            // +0x0c
    long  MovementType;                         // +0x10
    float Speed;                                // +0x14
};
struct CCreatureNavOutput {
    long Status;                                // 0 arrived, 1 failed, 2 moving
    C3DVector Offset;
};
struct CTCLook {
    bool TrackNearestInterestingThing(bool track);   // 0x00753840
};
struct CTCCreatureNavigation {
    bool HasRouteTo(const C3DVector& pos, float proximity);   // 0x006CB990
    const C3DVector& GetRouteEnd();             // 0x006CB9A0
    CCreatureNavOutput GetNextNavPosOnRoute(const CNavigationInput& input, bool b1, bool b2, bool b3, bool b4, bool b5);   // 0x006CBAC0
};
struct CCreatureActionBase {
    virtual void v00();
    bool BlocksMovement();                      // virtual slot 49 (+0xc4), declared below
};
struct CCreatureActionVirtuals {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48();
    virtual bool BlocksMovement();              // slot 49 (+0xc4)
};

struct CThingGraphic {
    unsigned char _pad00[0x34];
    float Radius;                               // +0x34
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CThingGraphic* PGraphic;                    // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[0x100 - 0x92];
    CCreatureActionVirtuals* CurrentAction;     // +0x100
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long type)
    {
        if (HasTC(type)) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *ppOut = (T*)(it == map->last || type < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing {
    bool IsPerformingAnyAction();               // 0x006622A0
    const C3DVector& GetPosition() const;       // 0x006A4D00
};
CThing* __fastcall GetPBestTargetToFaceForCreature(CThingCreatureBase* creature);   // 0x0097E140

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>
    virtual void v00();
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    return p.GetPItem() != 0 && !(p.GetPItem()->m_flags91 & 1);
}
struct CCreatureActionBase;
struct CBrainOutput {
    unsigned long           Types;              // +0x00
    long                    LookFlags;          // +0x04
    CNavigationInput        NavigationInput;    // +0x08
    CCountedPointer<CCreatureActionBase> PAction;   // +0x20
    C3DVector               MoveToPos;          // +0x28
    C3DVector               LookAtPos;          // +0x34
    CBaseIntelligentPointer LookAtThing;        // +0x40
    CBrainOutput();                             // 0x00835620
    ~CBrainOutput();                            // 0x006D60C0
    void SetAsLookAtThing(const CThing* thing, long lookFlags);   // 0x0093E760
    void SetAsMoveToPos(const C3DVector& pos, const CNavigationInput& input)
    {
        Types |= 1;
        MoveToPos = pos;
        NavigationInput = input;
    }
};
struct CAIBrain {
    virtual void v00(); virtual void v01();
    virtual void Yield();                       // slot 2 (+0x08)
    unsigned char _pad04[0x14];
    CThingCreatureBase* Creature;               // +0x18
    void SetBrainOutput(const CBrainOutput& output);   // 0x0088B8F0
};

enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 0, AI_ACTION_REPEAT_UNTIL_DONE = 1, AI_ACTION_REPEAT_POLL = 2 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
enum EMovementType { MOVEMENT_DEFAULT };
inline bool IsDead(const CThing* thing) { return (thing->m_flags91 & 1) != 0; }

struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    bool IsStopping();                          // 0x008FD010
    void SetNavigationIgnoreThing(const CThing* thing);   // 0x0097E270
    void SetNavigationSpeedScale(float scale);            // 0x0097E2D0
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat, CIntelligentPointerConstThing thing, float proximity,
                                                  bool& moving, EMovementType anim, bool lookAround, bool b7, bool b8, bool b10,
                                                  float speedScale);
};

EAIActionReturn CAIStateGroupBase::AIAction_MoveToThingProximity(EAIActionRepeat repeat, CIntelligentPointerConstThing thing, float proximity,
                                                                 bool& moving, EMovementType anim, bool lookAround, bool b7, bool b8,
                                                                 bool b10, float speedScale)
{
    SetNavigationIgnoreThing(thing.GetPItem());
    SetNavigationSpeedScale(speedScale);
    if (IsStopping())
        return AI_ACTION_STOPPING;
    EAIActionReturn result = AI_ACTION_RUNNING;
    if (!IS_THING_ALIVE(thing))
        return AI_ACTION_ABORTED;
    bool trackInteresting = true;
    CTCCreatureNavigation* navigation;
    do {
        CThing* faceTarget = GetPBestTargetToFaceForCreature(GetCreature());
        if (GetCreature()->HasTC(0x43))
            ((CTCLook*)GetCreature()->GetTC(0x43))->TrackNearestInterestingThing(trackInteresting);
        if (IS_THING_ALIVE(thing)) {
            C3DVector targetPos = thing.GetPItem()->GetPosition();
            if (GetCreature()->HasTC(7))
                navigation = (CTCCreatureNavigation*)GetCreature()->GetTC(7);
            if (navigation->HasRouteTo(targetPos, proximity)) {
                C3DVector routeEnd = navigation->GetRouteEnd();
                const C3DVector& myPos = static_cast<const CThing*>(GetCreature())->GetPosition();
                float dx = myPos.X - routeEnd.X;
                float dy = myPos.Y - routeEnd.Y;
                float dz = myPos.Z - routeEnd.Z;
                float reach = proximity + GetCreature()->PGraphic->Radius;
                if (reach * reach < dz * dz + dy * dy + dx * dx) {
                    float ex = targetPos.X - routeEnd.X;
                    float ey = targetPos.Y - routeEnd.Y;
                    if (ey * ey + ex * ex < 72.25f) {
                        const C3DVector& me = static_cast<const CThing*>(GetCreature())->GetPosition();
                        float fx = targetPos.X - me.X;
                        float fy = targetPos.Y - me.Y;
                        if (fy * fy + fx * fx > 12.25f)
                            targetPos = routeEnd;
                    }
                }
            }
            CNavigationInput input;
            input.Destination = targetPos;
            input.Proximity = proximity;
            input.MovementType = anim;
            input.Speed = 0.0f;
            CCreatureNavOutput output = navigation->GetNextNavPosOnRoute(input, b7, b8, b10, true, false);
            moving = output.Status == 0 || output.Offset.GetLengthSquared() > EPSILON * EPSILON;
            if (!GetCreature()->IsPerformingAnyAction() || !GetCreature()->CurrentAction->BlocksMovement()) {
                switch (output.Status) {
                case 0:
                    result = AI_ACTION_DONE;
                    break;
                case 1:
                    result = AI_ACTION_ABORTED;
                    break;
                case 2: {
                    CBrainOutput brainOutput;
                    brainOutput.SetAsMoveToPos(output.Offset + GetCreature()->GetPosition(), input);
                    if (faceTarget && !IsDead(faceTarget))
                        brainOutput.SetAsLookAtThing(faceTarget, 0x97);
                    else if (lookAround) {
                        brainOutput.Types |= 0x10;
                        brainOutput.LookFlags = 0x51;
                        trackInteresting = false;
                    }
                    PBrain->SetBrainOutput(brainOutput);
                }
                    result = AI_ACTION_RUNNING;
                    break;
                }
            }
        } else
            result = AI_ACTION_ABORTED;
        PBrain->Yield();
        if (IsStopping())
            return AI_ACTION_STOPPING;
    } while (repeat == AI_ACTION_REPEAT_UNTIL_DONE && result == AI_ACTION_RUNNING);
    return result;
}
