// CAIStateGroup_MinionWorkInInn::CleanBench() -- take a free action point on a free bench, walk to it, face along the point's forward axis and play ST_WIPE_TABLE; free the action point afterwards (or when a step fails)
// Leaf: ?CleanBench@CAIStateGroup_MinionWorkInInn@@AAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090BD40, 399 bytes (+ int3 padding), plain `ret`.
// Name: Ego_r.pdb ?CleanBench@CAIStateGroup_MinionWorkInInn@@AAEXXZ (private; the parked file had it public).
// Callees (Ego_r.pdb spellings): CAIStateGroup_MinionWorkInInn::GetFreeBench() 0x0090BC60 (AAEPAVCThing@@XZ: the class's only
//   other non-virtual member, no arguments, returns the bench; catalog "CleanBench_PickRandomMarker" is descriptive),
//   CTCUserActionPointBase::GetFreeActionPoint(CThing&, C3DVector&, CRightHandedSet&, long&) 0x007FEDE0 and
//   SetActionPointAsFree(long) 0x007FEF40, CAIStateGroupBase::AIAction_MoveToPos 0x0097EC00 / AIAction_LookAtPos 0x0097D580 /
//   AIAction_PerformAction(const CCreatureActionBase&) 0x0097D8B0 (protected, EAIActionReturn), CThing::GetPos() const
//   0x004C73D0, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("ST_WIPE_TABLE" 0x0128F120),
//   CCreatureAction_PlayAnimation(CThingCreatureBase&, const CCharString&, const CSoundPair&, bool, bool, bool, long, long)
//   0x008424F0, CCreatureActionBase::~CCreatureActionBase (virtual) 0x00693EF0; the inlined GetInterface lookup 0x0040F020
//   keeps the fleet's descriptive CInterfaceMap::LowerBound spelling.
// Shape: the parked 399v399 body (named look target, three-float ctor) plus ONE lever: `reached` lives in its own block, so
//   VC7.1 packs it into the same frame byte as the inlined GetInterface presence bool (both +3), which moves the points and
//   key/index slots to retail's +4 / +8. The action temporary's compiler-generated destructor calls ~CCreatureActionBase
//   directly (no vtable re-store).
// Layout: CAIStateGroupBase (engine/CAIStateGroupBase.h names: PBrain +4, ScriptAIPriority +8, Name +0xc, flags +0x10..+0x12),
//   CAIBrain::Creature +0x18 (engine/CAIBrain.h); the creature accessor GetCreature() is descriptive (as in the landed
//   CAIStateGroup_MinionSitAtHome::IsRunnable 0x00939180). Retail CThing UsingComponents +0x20 / Components +0x44.
//   Local models (not the shared headers) because the headers flatten the vptr; see NOTES.md header debt.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) { X = x; Y = y; Z = z; }
    C3DVector(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};
class CRightHandedSet {
public:
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0c
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 UsingComponents
    CInterfaceMap m_interfaces;                 // +0x44 Components
    const C3DVector& GetPos() const;            // 0x004C73D0
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
        }
        return has;
    }
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CThingCreatureBase : public CThingGameObject { };
class CThingAICreature : public CThingCreatureBase { };
class CTCUserActionPointBase {                  // interface 0x2d
public:
    bool GetFreeActionPoint(CThing& user, C3DVector& pos, CRightHandedSet& orientation, long& index);   // 0x007FEDE0
    void SetActionPointAsFree(long index);      // 0x007FEF40
};
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CSoundPair {
public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();             // 0x00693EF0
    unsigned char _body[0xa4];                  // +0x04..+0xa7
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase {
public:
    CCreatureAction_PlayAnimation(CThingCreatureBase& creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
    unsigned char _anim[0x0c];                  // +0xa8..+0xb3
};
class CAIBrain {
public:
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
};
enum EAIActionReturn { AI_ACTION_RETURN_2 = 2 };
enum EAIActionRepeat { AI_ACTION_REPEAT_1 = 1 };
enum EMovementType { MOVEMENT_TYPE_3 = 3 };
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    bool Terminated;                            // +0x10
    unsigned char Priority;                     // +0x11
    bool Enabled;                               // +0x12
protected:
    CThingAICreature& GetCreature() const { return *PBrain->Creature; }
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector pos, bool& reached, EMovementType movement,
        bool a, bool b, const CThing* avoid, bool c, float speed);    // 0x0097EC00
    EAIActionReturn AIAction_LookAtPos(EAIActionRepeat repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
    EAIActionReturn AIAction_PerformAction(const CCreatureActionBase& action);   // 0x0097D8B0
};
class CAIStateGroup_MinionWorkInInn : public CAIStateGroupBase {
private:
    CThing* GetFreeBench();                     // 0x0090BC60
    void CleanBench();
};

void CAIStateGroup_MinionWorkInInn::CleanBench()
{
    CThing* bench = GetFreeBench();
    if (!bench)
        return;
    CTCUserActionPointBase* points;
    bench->GetInterfaceT(&points, 0x2d);
    long index;
    C3DVector pos;
    CRightHandedSet orientation;
    if (!points->GetFreeActionPoint(GetCreature(), pos, orientation, index))
        return;
    {
        bool reached;
        if (AIAction_MoveToPos(AI_ACTION_REPEAT_1, pos, reached, MOVEMENT_TYPE_3, true, true, 0, true, 0.0f) != AI_ACTION_RETURN_2) {
            points->SetActionPointAsFree(index);
            return;
        }
    }
    C3DVector target = orientation.Forward + GetCreature().GetPos();
    if (AIAction_LookAtPos(AI_ACTION_REPEAT_1, target, 0x11, 0.0027777f) != AI_ACTION_RETURN_2) {
        points->SetActionPointAsFree(index);
        return;
    }
    {
        CCharString animation("ST_WIPE_TABLE");
        AIAction_PerformAction(CCreatureAction_PlayAnimation(GetCreature(), animation, CSoundPair(),
            false, true, false, 0, 0));
    }
    points->SetActionPointAsFree(index);
}