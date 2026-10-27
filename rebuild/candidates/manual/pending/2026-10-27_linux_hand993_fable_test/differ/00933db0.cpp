// CAIStateGroup_MinionKidAnnoy::Update() -- while the target lives and stays within 9m: run up to it (the two bounding radii apart), and on arrival with the target still alive perform the kid-annoy action; terminate when anything fails
// Grade: DIFFER(347v347) -- regalloc only: the two creature reloads use eax/eax (retail ecx then eax/esi) and the proximity float goes through edx (retail eax) Also tried here: a CAIBrain inline GetCreature() accessor (same bytes).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00933DB0, length 352 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor (PTarget +0x14; by-value CIntelligentPointer<const CThing>
//          vtable 0x01237F1C), CThing::GetPosition @ 0x004C73D0, CAIStateGroupBase::AIAction_MoveToThingProximity
//          @ 0x0097E380, the TU's kid-annoy action ctor @ 0x00933F10 (creature, target; manifest
//          "CCreatureAction_AddRealObjectToInventory" is propagated), AIAction_PerformActionAndWait @ 0x0097DB70,
//          ~CCreatureActionBase @ 0x00693EF0, Terminate @ 0x008FCF40.
// Globals: 81.0f (9m squared) @ 0x0129227C.
// Shape: the XY distance squared is one expression (VC7.1 squares Y first) tested with a plain `<`; the proximity is a
//          named float (the sum goes through a frame slot into the push); the action temporary's implicit destructor
//          destroys its intelligent-pointer member and then the base (no vtable restore).
// Layout: engine/CAIStateGroup_MinionKidAnnoy.h (PTarget +0x14); PBrain is the first dword at +0x04; the things'
//          +0x60 bounds and its +0x34 radius are descriptive.
#include "engine/CAIStateGroup_MinionKidAnnoy.h"

struct C3DVector {
    float X, Y, Z;
};
struct C2DVector {
    float X, Y;
    float GetLengthSquared() const { return X * X + Y * Y; }
};
struct CBoundsInfo {
    unsigned char _00[0x34];
    float Radius;                               // +0x34 (descriptive)
};
struct CThing {
    unsigned char _pad0[0x60];
    CBoundsInfo* PBounds;                       // +0x60 (descriptive)
    unsigned char _pad1[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition();             // 0x004C73D0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    __forceinline CIntelligentPointer(const CBaseIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];                  // +0x04..+0xa7
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_KidAnnoy : CCreatureActionBase {
    CBaseIntelligentPointer PTarget;            // +0xa8
    CCreatureAction_KidAnnoy(CThing* creature, CThing* target);   // 0x00933F10
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupMinionKidAnnoyMethods : CAIStateGroup_MinionKidAnnoy {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CBaseIntelligentPointer& Target() { return *(CBaseIntelligentPointer*)PTarget; }
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointer<const CThing> target, float proximity,
        bool* reached, long movement, bool a, bool b, bool c, bool d, float speed);   // 0x0097E380
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced);   // 0x0097DB70
    void Terminate();                           // 0x008FCF40
    void Update();
};

void CAIStateGroupMinionKidAnnoyMethods::Update()
{
    for (;;) {
        if (!IS_THING_ALIVE(Target()))
            break;
        float distanceSquared;
        {
            CThing* creature = GetCreature();
            const C3DVector& target = Target().GetPItem()->GetPosition();
            const C3DVector& me = creature->GetPosition();
            float dx = me.X - target.X;
            float dy = me.Y - target.Y;
            distanceSquared = dx * dx + dy * dy;
        }
        if (!(distanceSquared < 81.0f))
            break;
        CBoundsInfo* myBounds = GetCreature()->PBounds;
        float proximity = Target().GetPItem()->PBounds->Radius + myBounds->Radius;
        bool reached;
        long result = AIAction_MoveToThingProximity(2, CIntelligentPointer<const CThing>(Target()), proximity,
            &reached, 5, true, false, true, true, 0.0f);
        if (result == 4)
            continue;
        if (result != 2)
            break;
        if (!IS_THING_ALIVE(Target()))
            continue;
        CThing* annoyer = GetCreature();
        long kicked = AIAction_PerformActionAndWait(&CCreatureAction_KidAnnoy(annoyer, Target().GetPItem()), false);
        if (kicked != 2)
            break;
    }
    Terminate();
}
