// CAIStateGroup_MinionWander::Update() -- wander loop: wander and idle; when the creature has strayed out of its wander circle (the scratchpad centre and max distance for state 4, else its start position and 10) walk back, then idle; repeat while DoNothing keeps running
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092C070, length 491 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xAA = CTCAIScratchpad, bit 10 of +0x34),
// CDefClassBase::GetSubDef<CCreatureNavigationDef> @ 0x006CD2F0 (on the creature's +0x70), CThing::GetPosition @ 0x004C73D0
// (twice), CAIStateGroupBase::AIAction_MinionWander @ 0x0097F870, AIAction_MinionIdle @ 0x0097FA00 (twice),
// AIAction_MoveToPosProximity @ 0x0097E890, AIAction_DoNothing @ 0x0097E200, Terminate @ 0x008FCF40 (twice).
// Shape: the presence bit is bound to a const reference (frame byte, later reused by the `reached` flag); the navigation
// def is copied out of its out slot into a holder (kept in edi, the slot reused for the radius) whose inline dtor releases
// it (no null test on the paths that already read AvoidDynamicObstacles); the state-4 centre comes from a by-value getter
// (temporary copy) chosen against the copied start position; `reached` is a function-scope bool assigned false inside the
// move arm (an initialised block local is stored after the address lea instead of after the first push).
// Layout: no engine header for CTCAIScratchpad or CCreatureNavigationDef; members named from Ego_r.pdb (WanderCentrePoint
// +0x14, WanderMaxDistance +0x24, ScriptingStateGroup +0x38; AvoidDynamicObstacles +0x34).
class CTCBase;

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CCreatureNavigationDef { public:
    virtual void v00();
    virtual void DeleteThis();                  // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0x2c];
    bool AvoidDynamicObstacles;                 // +0x34
    void Release() { if (--RefCount == 0) DeleteThis(); }
};

struct CDefPointerNav {
    CCreatureNavigationDef* m_p;
    CDefPointerNav() : m_p(0) { }
};

struct CNavDefHolder {
    CCreatureNavigationDef* p;
    CNavDefHolder(CCreatureNavigationDef* d) : p(d) { }
    ~CNavDefHolder() { if (p) p->Release(); }
};

class CDefClassBase { public:
    bool GetSubDef(CDefPointerNav* out);        // 0x006CD2F0 (GetSubDef<CCreatureNavigationDef>)
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x24];
    CDefClassBase* PhysicsTC;                   // +0x70
    const C3DVector& GetPosition() const;       // 0x004C73D0
    unsigned char HasTC(long interfaceType) const { return (unsigned char)((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        const unsigned char& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return present != 0;
    }
};

class CTCAIScratchpad { public:                        // interface 0xAA
    unsigned char _pad[0x14];
    C3DVector WanderCentrePoint;                // +0x14
    float WanderMinDistance;                    // +0x20
    float WanderMaxDistance;                    // +0x24
    unsigned char _pad28[0x10];
    long ScriptingStateGroup;                   // +0x38
    C3DVector GetWanderCentrePoint() const { return WanderCentrePoint; }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CThingBuilding;
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    void Terminate();                           // 0x008FCF40
    long AIAction_MinionWander(long movement, CThingBuilding* building);    // 0x0097F870
    long AIAction_MinionIdle();                 // 0x0097FA00
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* reached, long movementType,
                                     bool a, bool b, bool c, CThing* leader, bool d, float e);   // 0x0097E890
};

#pragma auto_inline(off)
long TmplSink(const void* self, long key);      // stand-in: the callee's own work (outside the graded bytes)
// Same-TU stand-ins (template members compiled in the caller's TU): LowerBound reads the key through its pointer; GetSubDef
// reads the old out pointer and writes the new one, like CDefPointer::operator=. Neither lets an argument escape.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    return (SEntry*)TmplSink(this, *type);
}
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointerNav* out)
{
    CCreatureNavigationDef* old = out->m_p;
    out->m_p = (CCreatureNavigationDef*)TmplSink(this, (long)old);
    return out->m_p != 0;
}
#pragma auto_inline(on)

struct CAIStateGroup_MinionWander : CAIStateGroupBase {
    virtual void Update();
};

void CAIStateGroup_MinionWander::Update()
{
    CThing* creature = PBrain->PCreature;
    CTCAIScratchpad* scratchpad;
    creature->GetInterface(&scratchpad, 0xaa);
    CDefPointerNav navDef;
    creature->PhysicsTC->GetSubDef(&navDef);
    C3DVector start = PBrain->PCreature->GetPosition();
    CNavDefHolder def(navDef.m_p);
    bool reached;
    do {
        if (AIAction_MinionWander(3, 0) != 2)
            return;
        if (AIAction_MinionIdle() != 2)
            return;
        C3DVector centre = scratchpad->ScriptingStateGroup == 4 ? scratchpad->GetWanderCentrePoint() : start;
        float radius = scratchpad->ScriptingStateGroup == 4 ? scratchpad->WanderMaxDistance : 10.0f;
        float distSq;
        {
            const C3DVector& pos = PBrain->PCreature->GetPosition();
            float dx = pos.X - centre.X;
            float dy = pos.Y - centre.Y;
            distSq = dy * dy + dx * dx;
        }
        float radiusSq = radius * radius;
        if (!(distSq < radiusSq)) {
            reached = false;
            if (AIAction_MoveToPosProximity(1, centre, radius, &reached, 3, true, false, def.p->AvoidDynamicObstacles, 0, true, 0.0f) != 2) {
                Terminate();
                return;
            }
            if (AIAction_MinionIdle() != 2) {
                Terminate();
                return;
            }
        }
    } while (AIAction_DoNothing() == 4);
}