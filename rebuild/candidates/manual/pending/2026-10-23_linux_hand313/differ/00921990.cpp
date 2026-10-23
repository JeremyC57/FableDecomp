// CAIStateGroup_MinionFollowThing::Update() -- follow the followee (stop when within 2 units, walk/run by distance hysteresis) until the hero's wait expression releases the minion or the followee stops following it; with no live followee, go to the nearest (or a random) marker named by the minion's follow component and terminate
// Grade: DIFFER(1797v1651) -- frame 0x78 v 0x34 (each inlined distance gets its own followee-XY copy in retail too, but ours keeps more temporaries), expression-test bools spilled instead of bl, &PFollowee in ebx v ebp; structure otherwise follows retail
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00921990, length 1651 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 (CIntelligentPointer
//          <const CThing> vtable 0x01237F1C), CAIStateGroup_MinionFollowThing::AssignTask @ 0x009215E0 /
//          IsExpressionPerfomedTo @ 0x00921440, CCharString @ 0x0099EBF0 / copy @ 0x0099EC30 / dtor @ 0x0099EAE0,
//          CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xbc = CTCFollowed, 0x7a, 0xaa),
//          CTCFollowed::IsAIFollower @ 0x006D4D70 / RemoveFollower @ 0x006D5180, the 0x7a component's AddDependentThing
//          @ 0x006AEBA0, IsStopping @ 0x008FD010, CThingCreatureBase::GetPosition @ 0x006A4D00, AIAction_DoNothing
//          @ 0x0097E200, AIAction_MoveToThingProximity @ 0x0097E380, PeekThingSearchTools @ 0x008FD000,
//          CThingSearchTools::GetPBestThing<HasDefGlobalIndex, Nearest> @ 0x00922320 / GetAllThings<InMap, !InArea>
//          @ 0x005ED860, GetPWorldSeed @ 0x008FCFD0, GFRandom @ 0x004CDAE0, free @ 0x00BFEA14, Terminate @ 0x008FCF40.
// Globals: 4.0f @ 0x0123088C, 12.25f @ 0x0128FFFC, 56.25f @ 0x0128FFF8, 20.25f @ 0x0128FFF4.
// Layout: engine/CAIStateGroup_MinionFollowThing.h (PFollowee +0x14, FollowDistance +0x1c, AvoidDynamicObstacles +0x20,
//          ExpressionUsed +0x22, FollowerAdded +0x23, CurrentMovementType +0x24).
#include "engine/CAIStateGroup_MinionFollowThing.h"

struct CTCBase;
struct CThing;
struct C3DVector {
    float X, Y, Z;
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector() { }
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    float GetLengthSquared() const { return X * X + Y * Y; }
};
__forceinline C2DVector operator-(const C2DVector& a, const C2DVector& b)
{
    C2DVector r;
    r.X = a.X - b.X;
    r.Y = a.Y - b.Y;
    return r;
}
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[0x1c9 - 0x92];
    unsigned char m_flags1c9;                   // +0x1c9 bit 1 (descriptive)
    const C3DVector& GetPosition() const;       // 0x006A4D00
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
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
    template<class T> __forceinline bool GetInterfaceTX(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return has;
    }
};
struct CThingCreatureBase : CThing { };
struct CThingAICreature : CThingCreatureBase { };
struct CTCFollowed {                            // interface 0xbc
    bool IsAIFollower(CThingAICreature* creature);   // 0x006D4D70
    void RemoveFollower(CThing* follower);      // 0x006D5180
};
struct CTCDependents {                          // interface 0x7a (descriptive)
    void AddDependentThing(CThing* thing);      // 0x006AEBA0
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};
struct CTCMinionFollowMarker {                  // interface 0xaa (descriptive)
    unsigned char _pad[0x38];
    long MarkerMode;                            // +0x38 (6/7: go to the marker; 7: the nearest one)
    unsigned char _pad3c[0x50 - 0x3c];
    CCharString MarkerDefName;                  // +0x50
    float MarkerProximity;                      // +0x54
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item) { SetPItem(item); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CThingFilter_HasDefGlobalIndex {
    CCharString Name;
    CThingFilter_HasDefGlobalIndex(const CCharString& name) : Name(name) { }
};
struct CThingFilter_InMapNotInArea {            // CFilter_Bind2<CThingFilter_InMap, unary_negate<CThingFilter_InArea> >
    CCharString Name;
    CThingFilter_InMapNotInArea(const CCharString& name) : Name(name) { }
};
struct CThingCompare_Nearest {
    C3DVector Position;
    CThingCompare_Nearest(const C3DVector& position) : Position(position) { }
};
extern "C" void __cdecl free(void* p);          // 0x00BFEA14
struct CThingVector {                           // std::vector<CThing*> (STLport, 12 bytes)
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingVector() { if (_M_start) free(_M_start); }
};
struct CThingSearchTools {
    bool GetPBestThing(const CThingFilter_HasDefGlobalIndex& filter, const CThingCompare_Nearest& compare, CThing** best);   // 0x00922320
    void GetAllThings(const CThingFilter_InMapNotInArea& filter, CThingVector* things);   // 0x005ED860
};
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};

struct CAIStateGroupMinionFollowThingMethods : CAIStateGroup_MinionFollowThing {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThingAICreature* GetCreature() const { return Brain()->PCreature; }
    CBaseIntelligentPointer& Followee() { return *(CBaseIntelligentPointer*)PFollowee; }
    long& MovementType() { return *(long*)CurrentMovementType; }
    __forceinline float DistanceSquaredToFollowee()
    {
        C2DVector followeePos(Followee().GetPItem()->GetPosition());
        C2DVector myPos(GetCreature()->GetPosition());
        return (followeePos - myPos).GetLengthSquared();
    }
    bool IsStopping();                          // 0x008FD010
    void Terminate();                           // 0x008FCF40
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    void AssignTask(CThing* followee);          // 0x009215E0
    bool IsExpressionPerfomedTo(const CCharString& expression);   // 0x00921440
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing thing, float radius, bool* reached,
                                       long movement, bool a, bool b, bool c, bool d, float speed);   // 0x0097E380
    void Update();
};

void CAIStateGroupMinionFollowThingMethods::Update()
{
    if (IS_THING_ALIVE(Followee())) {
        AssignTask(Followee().GetPItem());
        long result;
        do {
            if (ExpressionUsed) {
                bool waited;
            {
                CCharString expression("EXPRESSION_WAIT");
                waited = IsExpressionPerfomedTo(expression);
            }
            if (waited) {
                    ExpressionUsed = false;
                    if (FollowerAdded && Followee().GetPItem() != 0) {
                        CTCFollowed* followed;
                        if (Followee().GetPItem()->GetInterface(&followed, 0xbc))
                            followed->RemoveFollower(GetCreature());
                        if (!(GetCreature()->m_flags1c9 & 2)) {
                            CTCDependents* dependents;
                            if (Followee().GetPItem()->GetInterface(&dependents, 0x7a))
                                dependents->AddDependentThing(GetCreature());
                        }
                    }
                    Terminate();
                    return;
                }
                CTCFollowed* followed;
                if (IS_THING_ALIVE(Followee()) && Followee().GetPItem()->GetInterfaceTX(&followed, 0xbc) &&
                    !followed->IsAIFollower(GetCreature())) {
                    ExpressionUsed = false;
                    Terminate();
                    return;
                }
            }
            if (IsExpressionPerfomedTo(CCharString("EXPRESSION_FOLLOW")))
                ExpressionUsed = true;
            if (!IS_THING_ALIVE(Followee()) || IsStopping()) {
                Terminate();
                return;
            }
            if (DistanceSquaredToFollowee() < 4.0f) {
                MovementType() = 0;
                result = AIAction_DoNothing();
            } else if (MovementType() == 0 && DistanceSquaredToFollowee() < 12.25f) {
                result = AIAction_DoNothing();
            } else {
                if (MovementType() == 0)
                    MovementType() = 3;
                if (MovementType() == 3) {
                    if (DistanceSquaredToFollowee() > 56.25f)
                        MovementType() = 5;
                } else if (DistanceSquaredToFollowee() < 20.25f)
                    MovementType() = 3;
                bool reached;
                result = AIAction_MoveToThingProximity(2, CIntelligentPointerConstThing(Followee().GetPItem()), FollowDistance,
                                                       &reached, MovementType(), true, false, AvoidDynamicObstacles, true, 0.0f);
            }
        } while (result == 4);
        return;
    }
    CThingAICreature* creature = GetCreature();
    CTCMinionFollowMarker* marker;
    creature->GetInterfaceT(&marker, 0xaa);
    CThing* target = 0;
    PeekThingSearchTools()->GetPBestThing(CThingFilter_HasDefGlobalIndex(CCharString(marker->MarkerDefName)),
                                          CThingCompare_Nearest(creature->GetPosition()), &target);
    if (marker->MarkerMode == 7) {
        PeekThingSearchTools()->GetPBestThing(CThingFilter_HasDefGlobalIndex(CCharString(marker->MarkerDefName)),
                                              CThingCompare_Nearest(GetCreature()->GetPosition()), &target);
    } else {
        CThingVector things;
        PeekThingSearchTools()->GetAllThings(CThingFilter_InMapNotInArea(CCharString(marker->MarkerDefName)), &things);
        if (things._M_start == things._M_finish) {
            Terminate();
            return;
        }
        target = things._M_start[GFRandom(things._M_finish - things._M_start, GetPWorldSeed())];
    }
    while (marker->MarkerMode == 6 || marker->MarkerMode == 7) {
        bool reached;
        AIAction_MoveToThingProximity(2, CIntelligentPointerConstThing(target), marker->MarkerProximity, &reached, 3,
                                      true, false, AvoidDynamicObstacles, true, 0.0f);
        if (IsStopping())
            break;
    }
    Terminate();
}
