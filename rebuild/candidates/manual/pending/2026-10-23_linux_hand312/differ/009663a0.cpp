// CAIStateGroup_CarryCrateAndFollow::Update() -- walk to a free action point of the live crate (or up to it when the point is unreachable), face the crate, pick it up (storing the action id and the previously carried item), then follow whoever performs EXPRESSION_FOLLOW at it until someone performs EXPRESSION_WAIT or the follow fails; a dead/claimed crate or any failure terminates the group
// Grade: DIFFER(1165v1180) -- first pass: flow, calls and the conditional EXPRESSION_WAIT temporary follow retail; frame layout differs (retail: flag byte 0x13, key 0x14, claim 0x20, temp-dtor flags 0x24, look vector 0x28, point 0x34, orientation 0x40, action 0x58), LookAtPos vector built in the argument slot after the GetPosition call in retail
// Retail: Fable.exe (TLC, VC7.1) @ 0x009663A0, 1180 bytes, plain ret.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, interface LowerBound 0x0040F020 (types 0x2d action points,
//   0x47 crate claim, 0x46 carrying), CTCUserActionPointBase::GetFreeActionPoint 0x007FEDE0, AIAction_MoveToPos 0x0097EC00,
//   AIAction_MoveToPosProximity 0x0097E890, IS_THING_ALIVE 0x004CC340 (__fastcall), CThing::GetPosition 0x004C73D0,
//   AIAction_LookAtPos 0x0097D580, CCreatureAction_AddRealObjectToInventory 0x00843700 / ~ 0x007120D0, the action id
//   getter 0x00692620, CTCCarrying::GetThingInPrimarySlot 0x006A1440 / RemoveThingFromPrimarySlot 0x006A1E00,
//   CIntelligentPointer<CThing>(CThing*) 0x004A96F0 / operator= 0x004A9760 / ~ 0x00A01C10, AIAction_PerformActionAndWait
//   0x0097DB70, IsExpressionPerfomedTo 0x00966290, CCharString 0x0099EBF0 / ~ 0x0099EAE0, CIntelligentPointer<const CThing>
//   (const CIntelligentPointer<CThing>&) 0x007C2930, AIAction_MoveToThingProximity 0x0097E380, AIAction_DoNothing 0x0097E200,
//   Terminate 0x008FCF40.
#include "engine/CAIStateGroup_CarryCrateAndFollow.h"

struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
};
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
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline bool TestTC(long type) const { return ((m_presentBits[type >> 5] >> (type & 31)) & 1) != 0; }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {    // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointerThing(CThing* item);     // 0x004A96F0
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other);   // 0x004A9760
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>
    virtual void v00();
    CIntelligentPointerConstThing(const CIntelligentPointerThing& other);   // 0x007C2930
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointerThing* pointer);    // 0x004CC340
__forceinline bool IsAlive(const CIntelligentPointerThing& p)
{
    return p.GetPItem() != 0 && !(p.GetPItem()->m_flags91 & 1);
}
struct CTCUserActionPointBase {
    bool GetFreeActionPoint(CThing* user, C3DVector* pos, CRightHandedSet* orientation, long* index);   // 0x007FEDE0
};
struct CTCCrateClaim {
    unsigned char _pad[0x10];
    CThing* PClaimant;                          // +0x10
};
struct CTCCarrying {
    CThing* GetThingInPrimarySlot();            // 0x006A1440
    void RemoveThingFromPrimarySlot(bool drop); // 0x006A1E00
};
struct CCreatureActionBase {
    void* __vftable;
    unsigned char _body[0x24];
    long ActionID;                              // +0x28
    unsigned char _rest[0xa8 - 0x2c];
    long GetActionID();                         // 0x00692620
};
struct CCreatureAction_AddRealObjectToInventory : CCreatureActionBase {
    unsigned char _add[0x10];
    CCreatureAction_AddRealObjectToInventory(CThing& creature, CThing& object);   // 0x00843700
    ~CCreatureAction_AddRealObjectToInventory();    // 0x007120D0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupCarryCrateAndFollowMethods : CAIStateGroup_CarryCrateAndFollow {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CIntelligentPointerThing& Crate() { return *(CIntelligentPointerThing*)PCrate; }
    CIntelligentPointerThing& PreviouslyCarried() { return *(CIntelligentPointerThing*)PPreviouslyCarriedItem; }
    CIntelligentPointerThing& ThingToFollow() { return *(CIntelligentPointerThing*)PThingToFollow; }
    void Terminate();                           // 0x008FCF40
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movement, bool a, bool b,
        CThing* thing, bool c, float speed);    // 0x0097EC00
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* reached, long movementType,
        bool a, bool b, bool c, CThing* leader, bool d, float e);   // 0x0097E890
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing thing, float proximity, bool* arrived,
        long movement, bool a, bool b, bool c, bool d, float e);   // 0x0097E380
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float turnSpeed);   // 0x0097D580
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    bool IsExpressionPerfomedTo(const CCharString& expression, CIntelligentPointerThing& performer) const;   // 0x00966290
    void Update();
};

void CAIStateGroupCarryCrateAndFollowMethods::Update()
{
    if (!IsAlive(Crate())) {
        Terminate();
        return;
    }
    bool flag;
    CThing* crate = Crate().GetPItem();
    CTCUserActionPointBase* points;
    flag = crate->TestTC(0x2d);
    if (flag)
        points = (CTCUserActionPointBase*)crate->GetTC(0x2d);
    C3DVector pointPosition;
    CRightHandedSet pointOrientation;
    if (!points->GetFreeActionPoint(GetCreature(), &pointPosition, &pointOrientation, &ActionPointIndex)) {
        Terminate();
        return;
    }
    C3DVector lookDirection = pointOrientation.Forward;
    GotActionPoint = true;
    CThing* claimed = Crate().GetPItem();
    CTCCrateClaim* claim;
    flag = claimed->TestTC(0x47);
    if (flag)
        claim = (CTCCrateClaim*)claimed->GetTC(0x47);
    long result;
    do {
        if (!IsAlive(Crate()) || claim->PClaimant) {
            Terminate();
            return;
        }
        result = AIAction_MoveToPos(2, pointPosition, &flag, 5, true, true, Crate().GetPItem(), true, 0.0f);
    } while (result == 4);
    if (result == 3) {
        do {
            if (!IsAlive(Crate()) || claim->PClaimant) {
                Terminate();
                return;
            }
            CThing* target = Crate().GetPItem();
            result = AIAction_MoveToPosProximity(2, target->GetPosition(), 1.0f, &flag, 5, true, false, true,
                Crate().GetPItem(), true, 0.0f);
        } while (result == 4);
        if (IS_THING_ALIVE(&Crate())) {
            CThing* me = GetCreature();
            CThing* target = Crate().GetPItem();
            lookDirection = target->GetPosition() - me->GetPosition();
        }
    }
    if (!IS_THING_ALIVE(&Crate()) || claim->PClaimant || result != 2) {
        Terminate();
        return;
    }
    result = AIAction_LookAtPos(1, lookDirection + GetCreature()->GetPosition(), 0x21, 0.0027777f);
    if (!IS_THING_ALIVE(&Crate()) || claim->PClaimant || result != 2) {
        Terminate();
        return;
    }
    CCreatureAction_AddRealObjectToInventory pickUp(*GetCreature(), *Crate().GetPItem());
    PickUpActionID = pickUp.GetActionID();
    CThing* creature = GetCreature();
    CTCCarrying* carrying;
    if (creature->HasTC(0x46))
        carrying = (CTCCarrying*)creature->GetTC(0x46);
    PreviouslyCarried() = CIntelligentPointerThing(carrying->GetThingInPrimarySlot());
    carrying->RemoveThingFromPrimarySlot(false);
    if (AIAction_PerformActionAndWait(&pickUp, false) == 2) {
        do {
            bool follow;
            {
                CCharString expression("EXPRESSION_FOLLOW");
                follow = IsExpressionPerfomedTo(expression, ThingToFollow());
            }
            if (follow) {
                for (;;) {
                    bool stop = !IS_THING_ALIVE(&ThingToFollow())
                        || IsExpressionPerfomedTo(CCharString("EXPRESSION_WAIT"), ThingToFollow());
                    if (stop)
                        break;
                    if (AIAction_MoveToThingProximity(1, ThingToFollow(), 2.0f, &flag, 5, true, false, true, true, 0.0f) != 2)
                        break;
                }
            }
        } while (AIAction_DoNothing() == 4);
    }
    Terminate();
}
