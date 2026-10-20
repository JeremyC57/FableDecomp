// CTCPhysicsStandard::GetPossibleInteractedWithList(const C2DBoxI& box, vector<CThing*>& things, float extraRadius) const -- when the body interacts or collides with things at all, gather the things in the box that it can collide or interact with, that overlap its sphere (radius + extraRadius) and that have physics (interface 2)
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E57D0, 164 bytes (`ret 0Ch`; PDB ?GetPossibleInteractedWithList@CTCPhysicsStandard@@IBEXABVC2DBoxI@@AAV?$vector@PAVCThing@@...@@M@Z).
// Callees: virtual GetInteractsWithThings slot 60 (+0xf0), GetCollidesWithThings 57 (+0xe4), GetCollidesWithThingsOneWay 58
//   (+0xe8), IsExcludingCreatures 61 (+0xf4); CTCBase::PeekThingSearchTools() const @ 0x006AFD40 (GetWorld() const ->
//   CWorld::PThingSearchTools +0x20; manifest "CThingCreatureBase::GetCombatManager" is propagated), CThingSearchTools::
//   GetAllThingsInRoughArea<const CThing, vector<CThing*>, CFilter_Bind3<...>> @ 0x008E60D0 (const, box / filter / out).
// Layout: Ego_r.pdb filters: CThingFilter_CanCollideOrInteractWithThing {TCPhysics, ExcludeCreatures}, CThingFilter_
//   OverlappingSphere {SpherePos, Radius}, CThingFilter_HasTC {Type}, CFilter_Bind3 {F1, F2, F3} (the Bind3 combination is in
//   the PDB symbol list); CTCPhysicsBase Position +0x0c, Radius +0x34.
// Shape: the three filters are named locals (frame: HasTC +4, CanCollide +8, Bind3 +0x10, sphere +0x1c) and the Bind3 a
//   temporary in the call. C3DVector needs its memberwise user copy constructor: the implicit copy forms `lea eax,[esi+0Ch]`
//   and reads through it, the user copy reads [esi+0Ch..14h] directly like retail.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct C2DBoxI { long TLX, TLY, BRX, BRY; };
struct CThing;
template <class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };
struct CTCPhysicsBase;
struct CThingFilter_CanCollideOrInteractWithThing {
    CTCPhysicsBase* TCPhysics;                          // +0x00
    bool            ExcludeCreatures;                   // +0x04
    CThingFilter_CanCollideOrInteractWithThing(CTCPhysicsBase* physics, bool excludeCreatures)
        : TCPhysics(physics), ExcludeCreatures(excludeCreatures) { }
};
struct CThingFilter_OverlappingSphere {
    C3DVector SpherePos;                                // +0x00
    float     Radius;                                   // +0x0c
    CThingFilter_OverlappingSphere(const C3DVector& pos, float radius) : SpherePos(pos), Radius(radius) { }
};
struct CThingFilter_HasTC {
    long Type;                                          // ETCInterfaceType
    CThingFilter_HasTC(long type) : Type(type) { }
};
template <class A, class B, class C> struct CFilter_Bind3 {
    const A* F1;
    const B* F2;
    const C* F3;
    CFilter_Bind3(const A& a, const B& b, const C& c) : F1(&a), F2(&b), F3(&c) { }
};
struct CThingSearchTools {
    template <class T, class TContainer, class TFilter>
    void GetAllThingsInRoughArea(const C2DBoxI& box, const TFilter& filter, TContainer& out) const;   // 0x008E60D0
};
struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056();
    virtual bool GetCollidesWithThings() const;         // slot 57 (+0xe4)
    virtual bool GetCollidesWithThingsOneWay() const;   // slot 58 (+0xe8)
    virtual void v059();
    virtual bool GetInteractsWithThings() const;        // slot 60 (+0xf0)
    virtual bool IsExcludingCreatures() const;          // slot 61 (+0xf4)
    void*     ParentThing;                              // +0x04 (CTCBase)
    unsigned char _pad08[0x4];
    C3DVector Position;                                 // +0x0c
    C3DVector OldPosition;                              // +0x18
    long      OldPositionLastFrameSet;                  // +0x24
    C3DVector Velocity;                                 // +0x28
    float     Radius;                                   // +0x34
    const CThingSearchTools* PeekThingSearchTools() const;   // 0x006AFD40 (GetWorld() const -> PThingSearchTools)
};
struct CTCPhysicsStandard : CTCPhysicsBase {
    void GetPossibleInteractedWithList(const C2DBoxI& box, vector<CThing*>& things, float extraRadius) const;
};

void CTCPhysicsStandard::GetPossibleInteractedWithList(const C2DBoxI& box, vector<CThing*>& things, float extraRadius) const
{
    if (GetInteractsWithThings() || GetCollidesWithThings() || GetCollidesWithThingsOneWay()) {
        CThingFilter_CanCollideOrInteractWithThing canFilter(const_cast<CTCPhysicsStandard*>(this), IsExcludingCreatures());
        CThingFilter_OverlappingSphere sphereFilter(Position, extraRadius + Radius);
        CThingFilter_HasTC physicsFilter(2);
        PeekThingSearchTools()->GetAllThingsInRoughArea<const CThing>(box,
            CFilter_Bind3<CThingFilter_CanCollideOrInteractWithThing, CThingFilter_OverlappingSphere, CThingFilter_HasTC>(
                canFilter, sphereFilter, physicsFilter), things);
    }
}
