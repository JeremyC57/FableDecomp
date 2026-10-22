// Grade: DIFFER(782v782) -- one interleave left: retail schedules the diveTo.Z integer copy after the fld of the 2D-length square, the lane before it (plus the const-pointer vtable relocation); 13 tries
// CAIStateGroup_HoverHornetQueen::DiveBomb() -- mark the timing critical; fly to the next circle marker in the route direction (or chase the target when there are no markers); then, when the target is within 12 units on the ground plane and the point 0.75 short of it is in navigable line of sight, perform the dive action and return to the current circle marker, flipping the route direction
// Layout: retail STLport vectors are 12 bytes, so CurrentCirclePoint +0x4c, CircleMarkers +0x50, TimingCritical +0x60, RouteDirection +0x64 (PDB header +0x50/+0x54/+0x68/+0x6c).
#include <math.h>
#pragma intrinsic(sqrt)
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    float GetXYLength() const { return (float)sqrt(X * X + Y * Y); }
    __forceinline C3DVector& operator*=(const float& s) { X *= s; Y *= s; Z *= s; return *this; }
    __forceinline void Normalise() { *this *= FastInvSqrt(Z * Z + X * X + Y * Y); }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
__forceinline C3DVector operator*(const C3DVector& a, float s) { return C3DVector(a.X * s, a.Y * s, a.Z * s); }
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing { };
__forceinline C3DVector Direction(const CThing* to, const CThing* from) { return to->GetPosition() - from->GetPosition(); }
struct CTCCreatureNavigation {
    bool IsPosNavLineOfSightTo(const C3DVector& pos);   // 0x006CBA20
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    virtual void v00();
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {     // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
template <class T> struct CVec {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x110];
};
struct CCombatActionBase : CCreatureActionBase {
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCreatureAction_HornetQueenDive : CCombatActionBase {
    CCreatureAction_HornetQueenDive(CThingCreatureBase* creature, CThing* target);   // 0x008C5410
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    void Terminate();                           // 0x008FCF40
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing target, float proximity,
                                       bool* arrived, long movement, bool a, bool b, bool c, bool d, float e);   // 0x0097E380
    long AIAction_CombatPerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DA40
};
struct CAIStateGroup_HoverMovementBase : CAIStateGroupBase { };
struct CAIStateGroup_HoverHornetQueen : CAIStateGroup_HoverMovementBase {
    CIntelligentPointerThing PTarget;           // +0x14
    unsigned char _pad1c[0x40 - 0x1c];
    CVec<CIntelligentPointerThing> Things;      // +0x40
    long CurrentCirclePoint;                    // +0x4c
    CVec<CIntelligentPointerThing> CircleMarkers;   // +0x50
    long EnemyKnockDownFrame;                   // +0x5c
    bool TimingCritical;                        // +0x60
    long RouteDirection;                        // +0x64
    long GetNextCirclePoint() { long i = CurrentCirclePoint + 1; if (i == (long)CircleMarkers.size()) i = 0; return i; }
    long GetPrevCirclePoint() { long i = CurrentCirclePoint - 1; if (i == -1) i = CircleMarkers.size() - 1; return i; }
    bool IsStillRunnable();                     // 0x0095A420
    long GotoCirclePoint(long index, long movementType);   // 0x0095A6C0
    void DiveBomb();
};

void CAIStateGroup_HoverHornetQueen::DiveBomb()
{
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    TimingCritical = true;
    if (CircleMarkers.size() > 0) {
        long result;
        if (RouteDirection == 0)
            result = GotoCirclePoint(GetNextCirclePoint(), 9);
        else
            result = GotoCirclePoint(GetPrevCirclePoint(), 9);
        if (result == 3)
            RouteDirection = RouteDirection == 0;
        if (!IsStillRunnable()) {
            Terminate();
            TimingCritical = false;
            return;
        }
    } else {
        bool arrived;
        long result;
        do
            result = AIAction_MoveToThingProximity(2, PTarget, 10.0f, &arrived, 9, true, false, true, true, 0.0f);
        while (result == 4 && IsStillRunnable());
    }
    if (IsStillRunnable()) {
        CThingCreatureBase* creature = GetCreature();
        CTCCreatureNavigation* nav;
        C3DVector diveTo;
        creature->GetInterface(&nav, 7);
        C3DVector offset = Direction(PTarget.GetPItem(), creature);
        C3DVector direction = offset;
        direction.Normalise();
        diveTo = PTarget.GetPItem()->GetPosition() - direction * 0.75f;
        if (offset.GetXYLength() < 12.0f && nav->IsPosNavLineOfSightTo(diveTo)) {
            AIAction_CombatPerformActionAndWait(&CCreatureAction_HornetQueenDive(GetCreature(), PTarget.GetPItem()), false);
            if (!IsStillRunnable()) {
                TimingCritical = false;
                Terminate();
                return;
            }
            GotoCirclePoint(CurrentCirclePoint, 9);
            RouteDirection = RouteDirection == 0;
        }
    }
    TimingCritical = false;
    if (!IsStillRunnable())
        Terminate();
}
