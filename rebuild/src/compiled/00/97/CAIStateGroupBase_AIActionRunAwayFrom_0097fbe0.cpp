// CAIStateGroupBase::AIAction_RunAwayFrom(CIntelligentPointer<const CThing>, CCreatureActionBase*) -- while the creature is aware of the living creature threat, move to a nearby position directed away from it (or wander off when it is inside personal space), otherwise idle
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097FBE0, 704 bytes (ret 0Ch; aligned frame from the 8-byte intelligent pointer local).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10,
//   IsAwareOfThingImplementation 0x00864700 (__fastcall), CThing::GetPos 0x004C73D0, CVectorMap::LowerBound 0x0040F020
//   (same-TU stand-in; inlined GetInterface 7 = CTCCreatureNavigation), CTCCreatureNavigation::GetNearbyDirectedPosition
//   0x006CB780 / GetNearbyWanderPosition 0x006CB6E0, CThingCreatureBase::GetPersonalSpaceRadius 0x00661CC0 (static),
//   CAIStateGroupBase::AIAction_MoveToPos 0x0097EC00 / AIAction_DoNothing 0x0097E200.
// Layout: the state group follows engine/CAIStateGroupBase.h (PBrain +4), modelled locally to declare the members; the
// brain's creature at +0x18; the threat's flag byte at +0x6c (bit 3) and dead bit at +0x91 (retail CThing).
// Shape: the found flag of the inlined navigation lookup is written through the bool that MoveToPos later fills (one
// frame byte); the 2D personal-space test compares a named distance against a named squared radius.
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
    const C3DVector& GetPos() const;       // 0x004C73D0
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned char m_flags6c;                    // +0x6c bit 3 tested
    unsigned char _pad6d[0x91 - 0x6d];
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
class CThingCreatureBase : public CThing { public:
    static float __fastcall GetPersonalSpaceRadius();   // 0x00661CC0 ?GetPersonalSpaceRadius@CThingCreatureBase@@SIMXZ (Ego_r.pdb; donor 0x0053CA80 in the CThingCreatureBase run)
};

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseIntelligentPointer();                  // 0x00A01B10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CIntelligentPointerConstCreature : CBaseIntelligentPointer {   // CIntelligentPointer<const CThingCreatureBase>, vtable 0x0125AAEC
    virtual void v00();
    CIntelligentPointerConstCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
bool __fastcall IsAwareOfThingImplementation(CThingCreatureBase* creature, CThing* thing, bool flag);   // 0x00864700

class CAIBrain { public:
    unsigned char _pad00[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 0, AI_ACTION_REPEAT_UNTIL_DONE = 1, AI_ACTION_REPEAT_POLL = 2 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
enum EMovementType { MOVEMENT_DEFAULT, MOVEMENT_RUN = 5 };
class CCreatureActionBase;

class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector pos, bool& moving, EMovementType anim, bool b1,
                                       bool b2, const CThing* target, bool b3, float f);   // 0x0097EC00
    EAIActionReturn AIAction_DoNothing();       // 0x0097E200
    EAIActionReturn AIAction_RunAwayFrom(CIntelligentPointerConstThing threat, CCreatureActionBase* action);
};

EAIActionReturn CAIStateGroupBase::AIAction_RunAwayFrom(CIntelligentPointerConstThing threat, CCreatureActionBase* action)
{
    if (!IS_THING_ALIVE(threat) || !(((CThing*)threat.GetPItem())->m_flags6c & 8))
        return AI_ACTION_ABORTED;
    CIntelligentPointerConstCreature enemy(((CThing*)threat.GetPItem()));
    for (;;) {
        if (!IS_THING_ALIVE(enemy) || !IsAwareOfThingImplementation(GetCreature(), ((CThing*)enemy.GetPItem()), false))
            return AI_ACTION_DONE;
        C3DVector away = GetCreature()->GetPos() - ((CThing*)enemy.GetPItem())->GetPos();
        CThingCreatureBase* creature = GetCreature();
        CTCCreatureNavigation* nav;
        bool moving;
        creature->GetInterfaceR(&nav, 7, moving);
        C3DVector dest;
        EAIActionReturn result;
        if (nav->GetNearbyDirectedPosition(creature->GetPos(), 5.0f, away, 0.25277778f, &dest)) {
            result = AIAction_MoveToPos(AI_ACTION_REPEAT_POLL, dest, moving, MOVEMENT_RUN, true, true, 0, true, 0.0f);
        } else {
            const CThing* threatThing = ((CThing*)enemy.GetPItem());
            CThingCreatureBase* me = GetCreature();
            float radius = CThingCreatureBase::GetPersonalSpaceRadius();
            const C3DVector& from = threatThing->GetPos();
            const C3DVector& to = me->GetPos();
            float dx = to.X - from.X;
            float dy = to.Y - from.Y;
            float distanceSquared = dy * dy + dx * dx;
            float radiusSquared = radius * radius;
            if (distanceSquared < radiusSquared
                && nav->GetNearbyWanderPosition(((CThing*)enemy.GetPItem())->GetPos(), 5.0f, &dest, false)) {
                    result = AIAction_MoveToPos(AI_ACTION_REPEAT_UNTIL_DONE, dest, moving, MOVEMENT_RUN, true, true, 0, true, 0.0f);
                if (result == AI_ACTION_DONE)
                    continue;
                return result;
            }
            result = AIAction_DoNothing();
        }
        if (result == AI_ACTION_RUNNING)
            continue;
        return result;
    }
}