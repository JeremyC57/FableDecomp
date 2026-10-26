// CThingCreatureBase::CThingCreatureBase(long type) -- construct the physical thing with the given type, the game-object layer and the creature base's own members (null def / action / interaction pointers, empty containers and queued-action list, handedness 2, default flag bits)
// Leaf: ??0CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00666090, 273 bytes, `ret 4`. Label correction: the catalog calls this row
//   "CTCDParticleEmitter::CTCDParticleEmitter"; the vtable it stores last, 0x0125AC0C, is CThingCreatureBase
//   (rtti-vtables.tsv); its callers are the derived creature ctors 0x0052AB26 / 0x00831F87.
// Callees: CThingPhysical::CThingPhysical(long type) @ 0x005296B0 (manifest "CTCDPhysicalObstruction" is propagated),
//   CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (x3), malloc @ 0x00BFEA0E (the list header node).
// Vtables: CThingGameObject 0x01244F7C, CIntelligentPointer<CThing> 0x01238C6C, CIntelligentPointer<CThingCreatureBase>
//   0x0123E7D8, CThingCreatureBase 0x0125AC0C.
// Layout: Ego_r.pdb CThingCreatureBase (0x188) with the retail game-object base 0x18 narrower (see the landed
//   CThingDeadCreature ctor 0x00835C20) and the retail STLport containers (12-byte vectors, 4-byte list head):
//   LastMessageEventICreatedID +0xd0, CombatCollisionDebugGraphics +0xd4, PDef +0xe0, ShotAccuracyPercentage +0xe4,
//   InitialPos +0xe8, PLastAttackedByCreature +0xf4, WFLastAttackedByCreature +0xfc, PCurrentAction +0x100,
//   PQueuedActions +0x108, MovementVector +0x10c, HeadPosOffset +0x118, IdleCounter +0x124, TurnSpeed +0x128,
//   PCreatureInteraction +0x12c, PTCModeManager +0x134, PreviousActionHandedness +0x138, ..WF +0x13c, BodyReorienter
//   +0x140, CombatDebugGraphics +0x144, PItemToUnsheatheAfterCutscene +0x150, DebugText +0x158, flag bits +0x15c..+0x15e.
//   engine/CThingCreatureBase.h is a flat PDB-offset struct, so a local model is used.
// Shape: CThingGameObject's ctor inline (vtable store after the base call, its member pointer), the derived vtable store,
//   then the init list in declaration order through one zero register (edi); the list head is zeroed then given its
//   16-byte self-linked header node; the flag bits are body assignments merged into whole-byte stores.
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
};
struct CThingPointer : CBaseIntelligentPointer {            // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CThingPointer() { }
};
struct CCreaturePointer : CBaseIntelligentPointer {         // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CCreaturePointer() { }
};

extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

class CThingPhysical { public:
    virtual void v00();
    unsigned char _pad04[0xbc];
    CThingPhysical(long type);                  // 0x005296B0
};
class CThingGameObject : public CThingPhysical { public:      // vtable 0x01244F7C
    CThingPointer PThingStandingOn;             // +0xc0
    unsigned char m_gameObjectFlags;            // +0xc8 (bitfield group, not initialised here)
    unsigned char _padc9[7];
    virtual void v00();
    CThingGameObject(long type) : CThingPhysical(type) { }
};

class CEnginePrimitiveHandle;
template <class P> struct CAllocProxy {      // STLport _STLP_alloc_proxy: the end-of-storage pointer with its own ctor
    P _M_data;
    CAllocProxy(P v) : _M_data(v) { }
};
struct CPrimitiveHandleVector {                 // vector<CEnginePrimitiveHandle>, 12 bytes in retail
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CAllocProxy<CEnginePrimitiveHandle*> _M_end_of_storage;
    CPrimitiveHandleVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
class CDefPointeeBase;
struct CCreatureDefPointer {                    // CDefPointer<CThingCreatureDef const>
    CDefPointeeBase* Object;
    CCreatureDefPointer() : Object(0) { }
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
};
class CCPPointerInfo;
class CCreatureActionBase;
struct CActionPointer {                         // CCountedPointer<CCreatureActionBase>
    CCreatureActionBase* Data;
    CCPPointerInfo*      Info;
    CActionPointer() : Data(0), Info(0) { }
};
struct SActionListNode { SActionListNode* _M_next; SActionListNode* _M_prev; CActionPointer _M_data; };
struct CActionList {                            // list<CCountedPointer<CCreatureActionBase> >: the header-node pointer
    SActionListNode* _M_node;
    CActionList() : _M_node(0)
    {
        SActionListNode* node = (SActionListNode*)malloc(sizeof(SActionListNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};
class CCreatureInteraction;
struct CInteractionPointer {                    // CCountedPointer<CCreatureInteraction>
    CCreatureInteraction* Data;
    CCPPointerInfo*       Info;
    CInteractionPointer() : Data(0), Info(0) { }
};
class CTCCreatureModeManager;
class CThingBodyReorienter;
struct CReorienterPointer {                     // scoped_ptr<CThingBodyReorienter>
    CThingBodyReorienter* ptr;
    explicit CReorienterPointer(CThingBodyReorienter* p = 0) : ptr(p) { }
};
enum ECombatAnimationHandedness { HANDEDNESS_0, HANDEDNESS_1, HANDEDNESS_2 };

class CThingCreatureBase : public CThingGameObject { public:  // vtable 0x0125AC0C
    unsigned long          LastMessageEventICreatedID;    // +0xd0
    CPrimitiveHandleVector CombatCollisionDebugGraphics;  // +0xd4
    CCreatureDefPointer    PDef;                          // +0xe0
    long                   ShotAccuracyPercentage;        // +0xe4
    C3DVector              InitialPos;                    // +0xe8
    CCreaturePointer       PLastAttackedByCreature;       // +0xf4
    unsigned long          WFLastAttackedByCreature;      // +0xfc
    CActionPointer         PCurrentAction;                // +0x100
    CActionList            PQueuedActions;                // +0x108
    C3DVector              MovementVector;                // +0x10c
    C3DVector              HeadPosOffset;                 // +0x118
    long                   IdleCounter;                   // +0x124
    float                  TurnSpeed;                     // +0x128
    CInteractionPointer    PCreatureInteraction;          // +0x12c
    CTCCreatureModeManager* PTCModeManager;               // +0x134
    ECombatAnimationHandedness PreviousActionHandedness;  // +0x138
    long                   PreviousActionHandednessWF;    // +0x13c
    CReorienterPointer     BodyReorienter;                // +0x140
    CPrimitiveHandleVector CombatDebugGraphics;           // +0x144
    CThingPointer          PItemToUnsheatheAfterCutscene; // +0x150
    char*                  DebugText;                     // +0x158
    bool CurrentlyFrameUpdating : 1;                      // +0x15c
    bool Blinking : 1;
    bool MeleeAttacker : 1;
    bool MeleeAttacked : 1;
    bool LeaveDeadBodyOverride : 1;
    bool LeaveExperienceOrbsOverride : 1;
    bool FollowHeroThroughTeleporters : 1;
    bool HeadPosCached : 1;
    bool UseMovementInActions : 1;                        // +0x15d
    bool UpdateMovementVectorThisFrame : 1;
    bool AbortingCurrentAction : 1;
    bool DemonDoor : 1;
    bool Oracle : 1;
    bool HadFacingAngleYZSetByBodyOrientation : 1;
    bool UnderInfluenceOfEpicSpell : 1;
    bool HiddenOnMiniMap : 1;
    bool IsShowingDebugInfo : 1;                          // +0x15e
    virtual void v00();
    CThingCreatureBase(long type);
};

CThingCreatureBase::CThingCreatureBase(long type)
    : CThingGameObject(type)
    , LastMessageEventICreatedID(0)
    , ShotAccuracyPercentage(0)
    , WFLastAttackedByCreature(0)
    , TurnSpeed(0)
    , PTCModeManager(0)
    , PreviousActionHandedness(HANDEDNESS_2)
    , PreviousActionHandednessWF(0)
    , DebugText(0)
{
    CurrentlyFrameUpdating = false;
    Blinking = false;
    MeleeAttacker = false;
    MeleeAttacked = false;
    LeaveDeadBodyOverride = true;
    LeaveExperienceOrbsOverride = true;
    FollowHeroThroughTeleporters = false;
    HeadPosCached = false;
    UseMovementInActions = true;
    UpdateMovementVectorThisFrame = true;
    AbortingCurrentAction = false;
    DemonDoor = false;
    Oracle = false;
    HadFacingAngleYZSetByBodyOrientation = false;
    UnderInfluenceOfEpicSpell = false;
    HiddenOnMiniMap = false;
    IsShowingDebugInfo = false;
}