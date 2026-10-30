// CTCTargetingBase::UpdateNearestInterestingThing() -- every Nth frame: keep the script-override thing when it is near, else search the 8x8 area round the owner for the nearest creature it is aware of and keep the current one while it is still visible inside the area
// Leaf: UpdateNearestInterestingThing
// Grade: DIFFER(544v544) -- this sweep: creature (= ParentThing) declared before the pos.X/pos.Y ftol fixes the creature=ebx load point and the box registers (diff 35 -> 26 lines); left: best loaded via eax and copied to ebp (retail straight into esi), map/layer register rotation; tried inline layer compare (554), const locals
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F5AE0, 544 bytes, plain `ret`.
// Callees: CTCBase world accessor 0x00686A70 (world singleton 0x013B8A1C +0x24; catalog "CViewBase::GetDisplayEngine"),
//   the frame counter getter 0x0049D870 (returns 0x013B89BC; catalog "GFGetPreMainMemoryUsed"), __allrem 0x00BFEC30,
//   CThing::GetPosition 0x004C73D0, CBaseIntelligentPointer::GetPItem 0x00A01B50, __ftol2 0x00BFEA70,
//   CTCBase::GetThingSearchTools 0x00686D90 (catalog "_Cons_val<...>" propagated), CThingSearchTools::GetPBestThingInRoughArea
//   <CFilter_Bind3<IsAICreatureAwareOf, IsUnconscious, IsAnAlly>, CThingCompare_Nearest, CThingCreatureBase> 0x007F5D20,
//   CThingFilter_IsInSameVolumeContextAsThing ctor 0x006A8C00 / operator() 0x006A8C60, CWorldMap::GetNavigationLayerAt
//   0x0063E590, CThingCreatureBase::IsThingInFOV 0x00665190, C2DBoxI::ContainsPoint 0x004FAB10,
//   CTCTargetingBase::SetPNearestInterestingThing 0x007F5810 (catalog "CVillageTaskCarryCrate::AssignCreatureToTask").
// Globals: the update period 0x01375550.
// Layout: Ego_r.pdb CTCTargetingBase (packed intelligent pointers at +0x0d / +0x15), as in the landed 0x007F5860.
#include <math.h>
struct C3DVector { float X, Y, Z; };
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float Length() const { return sqrtf(X * X + Y * Y); }
};
struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
struct C2DBoxI {
    C2DCoordI TopLeft, BottomRight;
    C2DBoxI(long x0, long y0, long x1, long y1) : TopLeft(x0, y0), BottomRight(x1, y1) { }
    bool ContainsPoint(const C2DCoordI& point) const;       // 0x004FAB10
};

struct CThing {
    unsigned char _pad00[0x10];
    unsigned char m_type;                                   // +0x10
    unsigned char _pad11[0x18 - 0x11];
    __int64       m_updateOffset;                           // +0x18
    unsigned char _pad20[0x91 - 0x20];
    unsigned char m_flags91;                                // +0x91
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};
struct CThingCreatureBase : CThing {
    bool IsThingInFOV(CThing* thing, bool flag);            // 0x00665190
};

struct CBaseObject;
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    void*        __vftable;
    CBaseObject* m_pItem;                                   // +0x04
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* Get() const { return (T*)GetPItem(); }
};
#pragma pack(pop)

struct CThingFilter_IsAICreatureAwareOf {
    CThingCreatureBase* Creature;
    CThingFilter_IsAICreatureAwareOf(CThingCreatureBase* creature) : Creature(creature) { }
};
struct CThingFilter_IsUnconscious {
    CThingCreatureBase* Creature;
    bool Wanted;
    CThingFilter_IsUnconscious(CThingCreatureBase* creature, bool wanted) : Creature(creature), Wanted(wanted) { }
};
struct CThingFilter_IsAnAlly {
    CThingFilter_IsAnAlly() { }
};
template <class A, class B, class C> struct CFilter_Bind3 {
    const A& First;
    const B& Second;
    const C& Third;
    CFilter_Bind3(const A& a, const B& b, const C& c) : First(a), Second(b), Third(c) { }
};
struct CThingCompare_Nearest {
    C3DVector Position;
    CThingCompare_Nearest(const C3DVector& position) : Position(position) { }
};
typedef CFilter_Bind3<CThingFilter_IsAICreatureAwareOf, CThingFilter_IsUnconscious, CThingFilter_IsAnAlly> CAwareFilter;
struct CThingSearchTools {
    bool GetPBestThingInRoughArea(const C2DBoxI& area, const CAwareFilter& filter, const CThingCompare_Nearest& compare,
                                  CThingCreatureBase** best);   // 0x007F5D20
};
struct CThingFilter_IsInSameVolumeContextAsThing {
    void* m_data[2];
    CThingFilter_IsInSameVolumeContextAsThing(CThing* thing, bool flag);   // 0x006A8C00
    bool operator()(CThing* thing) const;                   // 0x006A8C60
};
struct CWorldMap {
    long GetNavigationLayerAt(const C3DVector& position, CThing* thing);   // 0x0063E590
};
struct CWorldState {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual CWorldMap* GetWorldMap();                       // +0x34
};
long GetFrameNumber();                                      // 0x0049D870
extern long g_NearestThingUpdatePeriod;                     // 0x01375550

#pragma pack(push, 1)
struct CTCTargetingBase {
    void*         __vftable;                                // +0x00
    CThingCreatureBase* ParentThing;                        // +0x04
    unsigned char _pad08[4];
    bool          ScriptOverrideIgnoreEverything;           // +0x0c
    CIntelligentPointer<CThing> PNearestInterestingThing;   // +0x0d
    CIntelligentPointer<CThing> PScriptOverrideInterestingThing;   // +0x15
    CWorldState* GetWorldState();                           // 0x00686A70
    CThingSearchTools* GetThingSearchTools();               // 0x00686D90
    void SetPNearestInterestingThing(CThing* thing);        // 0x007F5810
    void UpdateNearestInterestingThing();
};
#pragma pack(pop)

void CTCTargetingBase::UpdateNearestInterestingThing()
{
    GetWorldState();
    __int64 offset = ParentThing->m_updateOffset;
    long period = g_NearestThingUpdatePeriod;
    if ((offset + GetFrameNumber()) % period != 0)
        return;
    if (ScriptOverrideIgnoreEverything)
        return;
    const C3DVector& pos = ParentThing->GetPosition();
    if (PScriptOverrideInterestingThing.Get()) {
        const C3DVector& target = PScriptOverrideInterestingThing.Get()->GetPosition();
        float dx = target.X - pos.X;
        float dy = target.Y - pos.Y;
        dx *= dx;
        if (sqrtf(dx + dy * dy) < 4.0f) {
            SetPNearestInterestingThing(PScriptOverrideInterestingThing.Get());
            return;
        }
    }
    CThingCreatureBase* creature = ParentThing;
    long x = (long)pos.X;
    long y = (long)pos.Y;
    C2DBoxI area(x - 4, y - 4, x + 4, y + 4);
    CThingCreatureBase* found;
    GetThingSearchTools()->GetPBestThingInRoughArea(area,
        CAwareFilter(CThingFilter_IsAICreatureAwareOf(creature), CThingFilter_IsUnconscious(creature, false),
                     CThingFilter_IsAnAlly()),
        CThingCompare_Nearest(pos), &found);
    CThingCreatureBase* best = found;
    if (best && !(best->m_flags91 & 1)) {
        if (!CThingFilter_IsInSameVolumeContextAsThing(creature, false)(best))
            best = 0;
        else {
            CWorldMap* map = GetWorldState()->GetWorldMap();
            long layer = map->GetNavigationLayerAt(best->GetPosition(), 0);
            if (layer != map->GetNavigationLayerAt(creature->GetPosition(), 0))
                best = 0;
        }
    }
    if (PNearestInterestingThing.Get() && (!best || best->m_type != 2) &&
        creature->IsThingInFOV(PNearestInterestingThing.Get(), false)) {
        const C3DVector& current = PNearestInterestingThing.Get()->GetPosition();
        if (area.ContainsPoint(C2DCoordI((long)current.X, (long)current.Y)))
            return;
    }
    SetPNearestInterestingThing(best);
}
