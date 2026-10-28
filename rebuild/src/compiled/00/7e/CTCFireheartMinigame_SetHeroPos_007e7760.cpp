// CTCFireheartMinigame::SetHeroPos(long x, long y, bool instantly) -- move the hero to grid marker [x][y]: teleport through the physics component, or (after putting the hero under scripted control) queue a CActionMoveTo to the marker
// Retry 10b (accessor sweep, FIXES B-4/B-5): the parent thing is read through an inline accessor; the plain member read gives the parked register difference.
// Leaf: SetHeroPos
//
// Retail 0x007E7760, 288 bytes incl. padding (`ret 0Ch`). Ego_r.pdb `?SetHeroPos@CTCFireheartMinigame@@...`.
// Callees: CThing::GetPos @ 0x004C73D0 (twice), the physics component's SetPosition(const C3DVector&) (slot 30, +0x78),
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for CTCScriptedControl, type 0x1F =
// bit 31 of the bitset word +0x20, `test ecx,ecx ; jns`; unchecked: the miss path uses the never-written slot),
// CTCBase::GetWorld @ 0x00686A70, CThing::GetScriptThing @ 0x004C7E50 (by value), CGameScriptInterface::StartScriptingEntity
// (slot 8, +0x20), ~CScriptThing @ 0x004AA840, operator new @ 0x00BFEA1A, CActionMoveTo::CActionMoveTo(const C3DVector&,
// EMovementType, float, bool, bool) @ 0x007E71D0, CTCScriptedControl::AddAction(CActionBase*) @ 0x007137D0.
// Global: the script runner @ 0x013D2838 (only tested for null).
// Layout = Ego_r.pdb CTCFireheartMinigame: Markers[4][4] +0x0C, XPos +0x68, YPos +0x6C, Moving +0x70, ScriptData +0x78
// (CScriptGameResourceObjectScriptedThingBase); CWorld PGameScriptInterface +0x38 (scoped_ptr); retail CThing PhysicsTC +0x60.
// Shape: the physics component is a named local whose vtable VC parks in the dead `instantly` slot across GetPos; the
// interface key and the out pointer also live in that slot; the CGameScriptInterface pointer is a named local (fetched with
// its vtable before the pushes), the CScriptThing an unnamed temporary; `new` keeps its null test.
class CThing;
class CTCBase;
class C3DVector;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CTCScriptedControl;
class CScriptThing { public:
    unsigned char _body[12 - 4];
    virtual ~CScriptThing();                            // 0x004AA840
};

class CTCPhysicsBase {
public:
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29)
#undef V
    virtual void SetPosition(const C3DVector& position);   // slot 30 (+0x78)
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    const C3DVector& GetPos() const;       // 0x004C73D0
    CScriptThing GetScriptThing();              // 0x004C7E50
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    __forceinline CTCScriptedControl* GetScriptedControl()
    {
        CTCScriptedControl* control;
        GetInterface(&control, 0x1f);
        return control;
    }
};

class CScriptGameResourceObjectScriptedThingBase { public:
    unsigned char _body[0x10];
};

class CGameScriptInterface {
public:
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07)
#undef V
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& data,
                                      long priority);   // slot 8 (+0x20)
};

class CWorld { public:
    unsigned char _pad00[0x38];
    CGameScriptInterface* PGameScriptInterface; // +0x38 (scoped_ptr<CGameScriptInterface>: its one pointer)
};

struct CActionBase;
struct CActionMoveTo {                          // 0x50 bytes
    unsigned char _body[0x50];
    CActionMoveTo(const C3DVector& position, long movementType, float distance, bool a, bool b);   // 0x007E71D0
};
class CTCScriptedControl { public:
    void AddAction(CActionBase* action);        // 0x007137D0
};

struct CScriptRunner;
extern CScriptRunner* g_pScriptRunner;          // 0x013D2838

class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    CThing* GetParentThing() const { return ParentThing; }
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCFireheartMinigame : public CTCBase {
public:
    CThing* Markers[4][4];                      // +0x0C
    bool InSession;                             // +0x4C
    bool ForceQuit;                             // +0x4D
    CThing* CurrentMarker;                      // +0x50
    bool Initialised;                           // +0x54
    bool BlocksActive[4][4];                    // +0x55
    long XPos;                                  // +0x68
    long YPos;                                  // +0x6C
    bool Moving;                                // +0x70
    CTCBase* PTCOldPhysics;                     // +0x74
    CScriptGameResourceObjectScriptedThingBase ScriptData;   // +0x78
    bool QuitWithDelay;                         // +0x88
    void SetHeroPos(long x, long y, bool instantly);
};

void CTCFireheartMinigame::SetHeroPos(long x, long y, bool instantly)
{
    if (XPos == x && YPos == y)
        return;
    XPos = x;
    YPos = y;
    if (instantly) {
        CTCPhysicsBase* physics = GetParentThing()->PhysicsTC;
        physics->SetPosition(Markers[x][y]->GetPos());
        return;
    }
    CTCScriptedControl* control = GetParentThing()->GetScriptedControl();
    if (g_pScriptRunner) {
        GetWorld().PGameScriptInterface->StartScriptingEntity(GetParentThing()->GetScriptThing(), ScriptData, 4);
    }
    control->AddAction((CActionBase*)new CActionMoveTo(Markers[x][y]->GetPos(), 3, 0.0f, true, false));
    Moving = true;
}