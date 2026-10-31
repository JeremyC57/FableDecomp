// CTCTargetingBase::UpdateNearestInterestingThing() -- every Nth frame: keep the script-override thing when it is near, else search the 8x8 area round the owner for the nearest creature it is aware of and keep the current one while it is still visible inside the area
// Grade: DIFFER(544v544) -- register roles only: retail loads the search result straight into esi and caches &PNearestInterestingThing in ebp, ours loads it via eax into ebp with the cache in esi; retail keeps the first navigation layer in ebp and spills the map reference to [esp+1Ch], ours keeps the map in esi and the layer in [esp+14h] | tried (chunk 616, 9 grades): PDB respelling (protected virtual MAE leaf, CTCBase::GetWorld IBE / GetThingSearchTools IAE, CWorld::PeekWorldMap slot 13 const&, static CWorld::GetFrame, const-reference filters, IsThingInFOV/GetNavigationLayerAt/ContainsPoint const) = same 48 lines; GetWorld().GetFrame() inside the modulo (529: the dead GetWorld call is dropped, so retail's is a separate statement); inline layer compare (554); map as a pointer (=); the search result used directly as best (554); best/layer declaration order (=) | next: lane gap (allocator priority between best and the intelligent-pointer CSE)
// Leaf: ?UpdateNearestInterestingThing@CTCTargetingBase@@MAEXXZ
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
class C3DVector { public: float X, Y, Z; };
class C2DVector {
public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float Length() const { return sqrtf(X * X + Y * Y); }
};
class C2DCoordI {
public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
class C2DBoxI {
public:
    C2DCoordI TopLeft, BottomRight;
    C2DBoxI(long x0, long y0, long x1, long y1) : TopLeft(x0, y0), BottomRight(x1, y1) { }
    bool ContainsPoint(const C2DCoordI& point) const;       // 0x004FAB10
};

class CBaseObject;
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char m_type;                                   // +0x10
    unsigned char _pad11[0x18 - 0x11];
    __int64       UniqueID;                                 // +0x18 (Ego_r ulong64; retail divides signed: __allrem)
    unsigned char _pad20[0x91 - 0x20];
    unsigned char m_flags91;                                // +0x91
    const C3DVector& GetPos() const;                        // 0x004C73D0
};
class CThingCreatureBase : public CThing {
public:
    bool IsThingInFOV(const CThing& thing, bool flag) const;   // 0x00665190
};
class CThingAICreature : public CThingCreatureBase { };

#pragma pack(push, 1)
class CBaseIntelligentPointer {
public:
    void*        __vftable;
    CBaseObject* PItem;                                     // +0x04
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* Get() const { return (T*)GetPItem(); }
};
#pragma pack(pop)

class CThingFilter_IsAICreatureAwareOf {
public:
    const CThingAICreature* Creature;
    CThingFilter_IsAICreatureAwareOf(const CThingAICreature* creature) : Creature(creature) { }
};
class CThingFilter_IsUnconscious {
public:
    const CThingCreatureBase* Creature;
    bool Wanted;
    CThingFilter_IsUnconscious(const CThingCreatureBase* creature, bool wanted) : Creature(creature), Wanted(wanted) { }
};
class CThingFilter_IsAnAlly {
public:
    CThingFilter_IsAnAlly(const CThing&) { }
};
template <class A, class B, class C> class CFilter_Bind3 {
public:
    const A& First;
    const B& Second;
    const C& Third;
    CFilter_Bind3(const A& a, const B& b, const C& c) : First(a), Second(b), Third(c) { }
};
class CThingCompare_Nearest {
public:
    C3DVector Position;
    CThingCompare_Nearest(const C3DVector& position) : Position(position) { }
};
typedef CFilter_Bind3<CThingFilter_IsAICreatureAwareOf, CThingFilter_IsUnconscious, CThingFilter_IsAnAlly> CAwareFilter;
class CThingSearchTools {
public:
    bool GetPBestThingInRoughArea(const C2DBoxI& area, const CAwareFilter& filter, const CThingCompare_Nearest& compare,
                                  CThingCreatureBase** best) const;   // 0x007F5D20
};
class CThingFilter_IsInSameVolumeContextAsThing {
public:
    void* Data[2];
    CThingFilter_IsInSameVolumeContextAsThing(const CThing& thing, bool flag);   // 0x006A8C00
    bool operator()(const CThing& thing) const;            // 0x006A8C60
};
class CWorldMap {
public:
    long GetNavigationLayerAt(const C3DVector& position, const CThing* thing) const;   // 0x0063E590
};
class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual const CWorldMap& PeekWorldMap() const;          // slot 13 (+0x34), 0x004AE8C0
    static long __fastcall GetFrame();                      // 0x0049D870
};
extern long g_NearestThingUpdatePeriod;                     // 0x01375550

#pragma pack(push, 1)
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    unsigned char _pad_0x0a[0x2];
protected:
    CWorld& GetWorld() const;                               // 0x00686A70
    CThingSearchTools& GetThingSearchTools();               // 0x00686D90
};
class CTCTargetingBase : public CTCBase {
public:
    bool          ScriptOverrideIgnoreEverything;           // +0x0c
    CIntelligentPointer<CThing> PNearestInterestingThing;   // +0x0d
    CIntelligentPointer<CThing> PScriptOverrideInterestingThing;   // +0x15
protected:
    void SetPNearestInterestingThing(CThing* thing);        // 0x007F5810
    virtual void UpdateNearestInterestingThing();
};
#pragma pack(pop)

void CTCTargetingBase::UpdateNearestInterestingThing()
{
    GetWorld();
    __int64 offset = ParentThing->UniqueID;
    long period = g_NearestThingUpdatePeriod;
    if ((offset + CWorld::GetFrame()) % period != 0)
        return;
    if (ScriptOverrideIgnoreEverything)
        return;
    const C3DVector& pos = ParentThing->GetPos();
    if (PScriptOverrideInterestingThing.Get()) {
        const C3DVector& target = PScriptOverrideInterestingThing.Get()->GetPos();
        float dx = target.X - pos.X;
        float dy = target.Y - pos.Y;
        dx *= dx;
        if (sqrtf(dx + dy * dy) < 4.0f) {
            SetPNearestInterestingThing(PScriptOverrideInterestingThing.Get());
            return;
        }
    }
    CThingAICreature* creature = (CThingAICreature*)ParentThing;
    long x = (long)pos.X;
    long y = (long)pos.Y;
    C2DBoxI area(x - 4, y - 4, x + 4, y + 4);
    CThingCreatureBase* found;
    GetThingSearchTools().GetPBestThingInRoughArea(area,
        CAwareFilter(CThingFilter_IsAICreatureAwareOf(creature), CThingFilter_IsUnconscious(creature, false),
                     CThingFilter_IsAnAlly(*creature)),
        CThingCompare_Nearest(pos), &found);
    CThingCreatureBase* best = found;
    if (best && !(best->m_flags91 & 1)) {
        if (!CThingFilter_IsInSameVolumeContextAsThing(*creature, false)(*best))
            best = 0;
        else {
            const CWorldMap& map = GetWorld().PeekWorldMap();
            long layer = map.GetNavigationLayerAt(best->GetPos(), 0);
            if (layer != map.GetNavigationLayerAt(creature->GetPos(), 0))
                best = 0;
        }
    }
    if (PNearestInterestingThing.Get() && (!best || best->m_type != 2) &&
        creature->IsThingInFOV(*PNearestInterestingThing.Get(), false)) {
        const C3DVector& current = PNearestInterestingThing.Get()->GetPos();
        if (area.ContainsPoint(C2DCoordI((long)current.X, (long)current.Y)))
            return;
    }
    SetPNearestInterestingThing(best);
}
