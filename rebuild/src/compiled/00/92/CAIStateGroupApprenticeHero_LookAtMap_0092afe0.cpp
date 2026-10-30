// CAIStateGroup_ApprenticeHero::LookAtMap() -- walk to the guild map, turn to it and play the trainee look-at-map animation; true when there is no map
// Grade: RELOCATION_MATCH -- retry wave 2 (sec4): the remembered map radius is a function-local static (its store
//   schedules after the radius argument push, an extern global's before) and the tail is if (... != 2) return false;
//   return true; (setne bl / sete al)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092AFE0, 304 bytes (`ret`). Ego_r.pdb: ?LookAtMap@CAIStateGroup_ApprenticeHero@@IAE_NXZ.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("MainGuildMap" 0x01291BBC,
// "ST_TRAINEE_LOOK_AT_MAP_TABLE" 0x01291B7C), CAIStateGroup_ApprenticeHero::GetObjectWithScriptName(const CCharString&)
// 0x00929FC0 (manifest "CAIStateGroupBase_SelectRandomElement" is propagated), CTCPhysicsBase::GetBoundingInfo
// (C3DVector*, float*) 0x006AF640, AIAction_MoveToPosProximity 0x0097E890, AIAction_LookAtThing 0x0097D6F0 (by-value
// CIntelligentPointer<const CThing> built in its argument slot: ctor 0x00A01B10, vtable 0x01237F1C, SetPItem 0x00A01B90),
// CCreatureAction_PlayCombatAnimation(CThingCreatureBase*, const CCharString&, bool) 0x008B7FF0 / ~CCombatActionBase
// 0x0062E900, AIAction_PerformActionAndWait 0x0097DB70; globals: the last map radius 0x013BB6B4, the look-at turn
// speed 0x0138CBA8 (0.01f, read as a variable).
// Shape: implicit const char* -> CCharString temporaries (re-addressed with `lea`), `bool reached;` uninitialised in
// the high byte of a frame word, the move with the as-landed AIAction_MoveToPosProximity arguments, the look-at,
// then the animation action as an address-of-temporary (as the landed 0x009574D0) and `== 2` returned after the dtors.
// Leaf: LookAtMap@CAIStateGroup_ApprenticeHero
class CThing;
class CThingCreatureBase;

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                               // +0x04
    CBaseIntelligentPointer();                     // 0x00A01B10
    void SetPItem(CThing* item);                   // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // vtable 0x01237F1C
    CIntelligentPointerConstThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

class CTCPhysicsBase { public:
    void GetBoundingInfo(C3DVector* centre, float* radius);   // 0x006AF640
};
class CThingGameObject { public:
    unsigned char _pad[0x60];
    CTCPhysicsBase* Physics;                       // +0x60
};

class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0x110];
};
class CCombatActionBase : public CCreatureActionBase { public:
    ~CCombatActionBase();                          // 0x0062E900
};
class CCreatureAction_PlayCombatAnimation : public CCombatActionBase { public:   // retail 0x118 bytes (Ego_r.pdb 0x130; the frame fixes it)
    unsigned char _own[4];
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* creature, const CCharString& animation, bool loop);   // 0x008B7FF0
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                 // +0x18
};

extern float g_LookAtTurnSpeed;                    // 0x0138CBA8 (0.01f)

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                              // +0x04
    unsigned char _pad[0x0c];                      // +0x08..+0x13
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* reached, long movementType,
                                     bool a, bool b, bool c, CThing* leader, bool d, float e);   // 0x0097E890
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long flags, float turnSpeed);   // 0x0097D6F0
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool flag);   // 0x0097DB70
};

struct CAIStateGroup_ApprenticeHero : CAIStateGroupBase {
    CThingGameObject* GetObjectWithScriptName(const CCharString& name);   // 0x00929FC0
    bool LookAtMap();
};

bool CAIStateGroup_ApprenticeHero::LookAtMap()
{
    CThingGameObject* map = GetObjectWithScriptName("MainGuildMap");
    if (!map)
        return true;
    C3DVector centre;
    float radius;
    CTCPhysicsBase* physics = map->Physics;
    physics->GetBoundingInfo(&centre, &radius);
    bool reached;
    static float lastMapRadius;                    // 0x013BB6B4 (.bss; descriptive name, no PDB symbol found)
    lastMapRadius = radius;
    if (AIAction_MoveToPosProximity(1, centre, radius, &reached, 3, true, false, true, 0, true, 0.0f) != 2)
        return false;
    if (AIAction_LookAtThing(1, CIntelligentPointerConstThing((CThing*)map), 0xb, g_LookAtTurnSpeed) != 2)
        return false;
    if (AIAction_PerformActionAndWait(&CCreatureAction_PlayCombatAnimation(GetCreature(), "ST_TRAINEE_LOOK_AT_MAP_TABLE", true), false) != 2)
        return false;
    return true;
}