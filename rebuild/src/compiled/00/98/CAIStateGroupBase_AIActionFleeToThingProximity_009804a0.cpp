// CAIStateGroupBase::AIAction_FleeToThingProximity(CIntelligentPointer<const CThingCreatureBase>, float, CCreatureActionBase*, CAIFilterBase*) -- move to within `proximity` of a creature while the filter holds; once there and idle, step to a nearby position toward it (facing the best target) or perform the action
// Retail: Fable.exe (TLC, VC7.1) @ 0x009804A0, 720 bytes (ret 14h).
// Callees: GetPBestTargetToFaceForCreature 0x0097E140 (__fastcall), CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem
//   0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10, CAIStateGroupBase::AIAction_MoveToThingProximity 0x0097E380 /
//   AIAction_LookAtThing 0x0097D6F0 / AIAction_MoveToPos 0x0097EC00 / AIAction_PerformAction 0x0097D8B0,
//   CThingCreatureBase::IsPerformingAnyAction 0x006622A0 / PeekPos 0x006A4D00, CVectorMap::LowerBound 0x0040F020
//   (same-TU stand-in; inlined GetInterface 7 = CTCCreatureNavigation), CTCCreatureNavigation::GetNearbyDirectedPosition
//   0x006CB780, EPSILON 0x0129BA3C.
// Layout: the state group follows engine/CAIStateGroupBase.h (PBrain +4), modelled locally to declare the members; the
// brain's creature at +0x18; dead bit at +0x91 (retail CThing).
// Shape: the found flag of the inlined navigation lookup is written through the bool that MoveToPos later fills; the
// PerformAction result is discarded (the loop re-tests the proximity move result); the creature is named before EPSILON.
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
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

class CTCCreatureNavigation { public:                  // interface type 7
    bool GetNearbyDirectedPosition(const C3DVector& from, float radius, const C3DVector& direction, float spread,
                                   C3DVector* out);     // 0x006CB780
    bool GetNearbyWanderPosition(const C3DVector& from, float radius, C3DVector* out, bool flag);   // 0x006CB6E0
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline void GetInterfaceR(T** ppOut, long interfaceType, bool& has)
    {
        has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return has;
    }
};
class CCreatureActionBase;
class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;       // 0x006A4D00
};
class CThingCreatureBase : public CThingPhysical { public:
    bool IsPerformingAnyAction() const;         // 0x006622A0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerConstCreature;
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item) : CBaseIntelligentPointer() { SetPItem((CThing*)item); }
    CIntelligentPointerConstThing(const CIntelligentPointerConstCreature& other);
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CIntelligentPointerConstCreature : CBaseIntelligentPointer {   // CIntelligentPointer<const CThingCreatureBase>, vtable 0x0125AAEC
    virtual void v00();
    const CThingCreatureBase* GetPCreature() const { return (const CThingCreatureBase*)GetPItem(); }
};
inline CIntelligentPointerConstThing::CIntelligentPointerConstThing(const CIntelligentPointerConstCreature& other)
    : CBaseIntelligentPointer()
{
    SetPItem(other.GetPItem());
}
__forceinline bool IS_THING_ALIVE(const CThing* thing)
{
    if (thing != 0) {
        if (!(thing->m_flags91 & 1))
            return true;
    }
    return false;
}
const CThing* __fastcall GetPBestTargetToFaceForCreature(CThingCreatureBase* creature);   // 0x0097E140
extern const float EPSILON;                     // 0x0129BA3C
struct CAIFilterBase {
    virtual bool Check() const;                 // slot 0
};

class CAIBrain { public:
    unsigned char _pad00[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 0, AI_ACTION_REPEAT_UNTIL_DONE = 1, AI_ACTION_REPEAT_POLL = 2 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
enum EMovementType { MOVEMENT_DEFAULT, MOVEMENT_RUN = 5 };

class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector pos, bool& moving, EMovementType anim, bool b1,
                                       bool b2, const CThing* target, bool b3, float f);   // 0x0097EC00
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat, CIntelligentPointerConstThing thing, float proximity,
                                                 bool& moving, EMovementType anim, bool b1, bool b2, bool b3, bool b4,
                                                 float f);   // 0x0097E380
    EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointerConstThing thing, long frames,
                                         float speed);   // 0x0097D6F0
    EAIActionReturn AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
    EAIActionReturn AIAction_FleeToThingProximity(CIntelligentPointerConstCreature threat, float proximity,
                                                  CCreatureActionBase* action, CAIFilterBase* filter);
};

EAIActionReturn CAIStateGroupBase::AIAction_FleeToThingProximity(CIntelligentPointerConstCreature threat, float proximity,
                                                                 CCreatureActionBase* action, CAIFilterBase* filter)
{
    bool moving = false;
    bool startedMoving = false;
    EAIActionReturn result;
    do {
        const CThing* faceTarget = GetPBestTargetToFaceForCreature(GetCreature());
        if (!IS_THING_ALIVE(threat) || !filter->Check())
            return AI_ACTION_ABORTED;
        result = AIAction_MoveToThingProximity(AI_ACTION_REPEAT_POLL, threat, proximity, moving, MOVEMENT_RUN, true, false,
                                               true, true, 0.0f);
        if (!IS_THING_ALIVE(threat) || result == AI_ACTION_ABORTED)
            return AI_ACTION_ABORTED;
        if (startedMoving || moving) {
            if (moving)
                startedMoving = true;
        } else if (!GetCreature()->IsPerformingAnyAction()) {
            CThingCreatureBase* creature = GetCreature();
            CTCCreatureNavigation* nav;
            bool found;
            creature->GetInterfaceR(&nav, 7, found);
            C3DVector toward = threat.GetPCreature()->PeekPos() - creature->PeekPos();
            C3DVector dest;
            if (nav->GetNearbyDirectedPosition(GetCreature()->PeekPos(), 5.0f, toward, 0.25277778f, &dest)) {
                CThingCreatureBase* me = GetCreature();
                float tolerance = EPSILON;
                const C3DVector& pos = me->PeekPos();
                float dx = pos.X - dest.X;
                float dy = pos.Y - dest.Y;
                float distanceSquared = dy * dy + dx * dx;
                if (!(distanceSquared < tolerance)) {
                    bool notLooking;
                    if (IS_THING_ALIVE(faceTarget)) {
                        EAIActionReturn lookResult = AIAction_LookAtThing(AI_ACTION_REPEAT_POLL,
                                                                          CIntelligentPointerConstThing(faceTarget), 0x27,
                                                                          0.0027777f);
                        if (lookResult != AI_ACTION_RUNNING && lookResult != AI_ACTION_DONE)
                            return lookResult;
                        notLooking = false;
                    } else
                        notLooking = true;
                    result = AIAction_MoveToPos(AI_ACTION_REPEAT_POLL, dest, found, MOVEMENT_RUN, notLooking, true, 0, true,
                                                0.0f);
                    continue;
                }
            }
            AIAction_PerformAction(action);
        }
    } while (result == AI_ACTION_RUNNING);
    return result;
}