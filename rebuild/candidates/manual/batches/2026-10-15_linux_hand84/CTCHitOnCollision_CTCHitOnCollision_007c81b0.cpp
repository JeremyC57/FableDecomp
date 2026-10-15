// CTCHitOnCollision::CTCHitOnCollision(CThing*) -- CTCBase(thing), own vtable, an empty collided list, the hit flags (pass through if blocked set, the rest clear), damage 2.0
// Leaf: ??0CTCHitOnCollision
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C81B0, length 55 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (the manifest label "CEngineSubPrimitive2DClockSprite"
// is propagated). Vtable 0x0126E7A4 = CTCHitOnCollision (rtti-vtables.tsv): the chunk label
// "CTCCreatureFlyOnSpline::CTCCreatureFlyOnSpline" is propagated.
// Layout = Ego_r.pdb CTCHitOnCollision less 4 bytes from +0x18 (the retail std::vector is 12 bytes):
// CollidedList +0xc, DontHitWhenCombatAction +0x18, Blockable +0x19, PassThroughIfBlocked +0x1a,
// PushSideways +0x1b, Damage +0x1c.
// Shape: every store follows the vtable store (the vector is a member object with an inline
// constructor); the zero bytes share al and are scheduled before the `1` byte store.
struct CThing;
struct CThingGameObject;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

struct CRetailVector_GameObjectPtr {                        // retail std::vector<CThingGameObject*>: 12 bytes
    CThingGameObject** First;
    CThingGameObject** Last;
    CThingGameObject** End;
    CRetailVector_GameObjectPtr() : First(0), Last(0), End(0) { }
};

struct CTCHitOnCollision : CTCBase_Calls {                  // vtable 0x0126E7A4
    CRetailVector_GameObjectPtr CollidedList;               // +0x0c
    bool                        DontHitWhenCombatAction;    // +0x18
    bool                        Blockable;                  // +0x19
    bool                        PassThroughIfBlocked;       // +0x1a
    bool                        PushSideways;               // +0x1b
    float                       Damage;                     // +0x1c
    virtual void v00();
    CTCHitOnCollision(CThing* thing);
};

CTCHitOnCollision::CTCHitOnCollision(CThing* thing)
    : CTCBase_Calls(thing)
    , CollidedList()
    , DontHitWhenCombatAction(false)
    , Blockable(false)
    , PassThroughIfBlocked(true)
    , PushSideways(false)
    , Damage(2.0f)
{
}
