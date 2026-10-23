// CTCCarryable::SetAsCarriedBy(CTCCarrying* carrier) -- attach the thing to a carrier (copy its mesh effects, alpha and
// Grade: DIFFER(522v526) -- retail hoists ParentThing into ebx for the map-number set and evaluates the 0x75 OnThingCarried component lookup before its argument; single-expression variants shrink the frame
// physics frame, persist as the def says) or, with no carrier, drop it back into the world (update the carried position,
// re-persist, recompute the map number from the position and clear the mesh effects)
// Leaf: SetAsCarriedBy
//
// Retail 0x00705D40, 526 bytes (`ret 4`).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined lookups 0x4c CTCWeaponTrail, 0x75 CTCVolumeContainmentTracker),
// CTCWeaponTrail::ClearAllWeaponTrails @ 0x0076E150, CTCCarryable::UpdateCarriedPosition @ 0x00705B60,
// CThing::SetAsGameStatePersisted @ 0x004C9C10, CTCBase::GetWorldMap @ 0x00686D30 (manifest "GetDisplayEngine", propagated),
// CThing::GetPosition @ 0x004C73D0, __ftol2 @ 0x00BFEA70, the world map's slot 17 (+0x44), CThing::SetCurrentMapNumber
// @ 0x004C7580, CTCGraphicAppearance::ClearAllMeshEffects @ 0x004BFF60 / ApplyAllMeshEffects @ 0x004C0430 / SetAlpha
// @ 0x004BFAC0, GetSubDef<CCarryableDef> @ 0x005CC10E, CTCCarrying @ 0x006A23E0 (manifest "COptimisedPrimitive::RemapMorphs",
// wrong), CTCVolumeContainmentTracker::OnThingCarried @ 0x00785C80; physics TC slots 31/32/36/72/80.
// Layout: CTCCarryable PCarryingTC +0x10; CThing bitset +0x20, map +0x44, PhysicsTC +0x60, Graphic +0x64, def +0x70;
// CCarryableDef IncludeInThingScansWhenCarried +0x4c; CTCCarrying BypassFilters +0x1c (retail).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
struct C3DVector { float X, Y, Z; };
struct CIntVector2 { long X, Y; CIntVector2(long x, long y) : X(x), Y(y) { } };
struct CCarryableDef {
    virtual void v00();
    virtual void Destroy();                                  // slot 1
    long RefCount;                                           // +0x04
    unsigned char _pad08[0x4c - 0x08];
    bool IncludeInThingScansWhenCarried;                     // +0x4c
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CCarryableDef>* out);         // 0x005CC10E
};
CCarryableDef* FindCarryableDef(const CDefClassBase* def);   // stand-in: the sub def lookup (not graded here)
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CCarryableDef>* out)
{
    CCarryableDef* def = FindCarryableDef(this);
    out->Object = def;
    return def != 0;
}
#pragma auto_inline(on)
struct CTCPhysicsBase {
    unsigned char _pad00[0x0c];
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30)
    virtual void SetCarriedPosition(const C3DVector& position);        // slot 31 (+0x7c)
    virtual void SetCarriedOrientation(long orientation);              // slot 32 (+0x80)
    V(33) V(34) V(35)
    virtual void SetActive(bool active);                               // slot 36 (+0x90)
    V(37) V(38) V(39) V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49)
    V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59) V(60) V(61) V(62) V(63) V(64) V(65) V(66) V(67) V(68) V(69)
    V(70) V(71)
    virtual long GetOrientation();                                     // slot 72 (+0x120)
    V(73) V(74) V(75) V(76) V(77) V(78) V(79)
    virtual void ResetMovement();                                      // slot 80 (+0x140)
#undef V
};
struct CTCGraphicAppearance {
    unsigned char _pad00[0x59];
    unsigned char Flags59;                                   // +0x59
    unsigned char Alpha;                                     // +0x5a
    void ClearAllMeshEffects();                              // 0x004BFF60
    void ApplyAllMeshEffects(CTCGraphicAppearance* source);  // 0x004C0430
    void SetAlpha(unsigned char alpha);                      // 0x004BFAC0
};
struct CTCWeaponTrail { void ClearAllWeaponTrails(); };      // 0x0076E150
struct CThing;
struct CTCVolumeContainmentTracker { void OnThingCarried(CThing& thing); };   // 0x00785C80
struct CThing {
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];                         // +0x20
    CInterfaceMap  m_interfaces;                             // +0x44
    unsigned char  _pad4c[0x60 - 0x4c];
    CTCPhysicsBase*       PhysicsTC;                         // +0x60
    CTCGraphicAppearance* Graphic;                           // +0x64
    unsigned char  _pad68[0x70 - 0x68];
    CDefClassBase* m_pDef;                                   // +0x70
    void SetAsGameStatePersisted(bool persisted);            // 0x004C9C10
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    void SetCurrentMapNumber(long map);            // 0x004C7580
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
};
struct CWorldMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual long GetMapNumberAt(const CIntVector2& coord);  // slot 17 (+0x44)
};
struct CTCCarrying {
    virtual void v00();
    CThing* ParentThing;                                     // +0x04
    unsigned char _pad08[0x1c - 0x08];
    bool BypassFilters;                                      // +0x1c
    void RefreshFilters();                                   // 0x006A23E0
};
struct CTCCarryable {
    virtual void v00();
    CThing* ParentThing;                                     // +0x04
    bool ToBeRemoved;                                        // +0x08
    bool IsInGlobalUpdateTCs;                                // +0x09
    unsigned char _pad0a[2];
    bool ForceFragility;                                     // +0x0c
    CTCCarrying* PCarryingTC;                                // +0x10
    CWorldMap& GetWorldMap();                                // 0x00686D30 (CTCBase)
    void UpdateCarriedPosition(bool forceUpdate);            // 0x00705B60
    void SetAsCarriedBy(CTCCarrying* carrier);
};

void CTCCarryable::SetAsCarriedBy(CTCCarrying* carrier)
{
    if (ParentThing->HasTC(0x4c))
        ParentThing->GetTC<CTCWeaponTrail>(0x4c)->ClearAllWeaponTrails();
    if (carrier == 0) {
        UpdateCarriedPosition(true);
        ParentThing->PhysicsTC->ResetMovement();
        PCarryingTC = 0;
        ParentThing->PhysicsTC->SetActive(true);
        ParentThing->SetAsGameStatePersisted(true);
        CWorldMap& map = GetWorldMap();
        const C3DVector& pos = ParentThing->GetPosition();
        CIntVector2 coord((long)pos.X, (long)pos.Y);
        ParentThing->SetCurrentMapNumber(map.GetMapNumberAt(coord));
        if (ParentThing->Graphic)
            ParentThing->Graphic->ClearAllMeshEffects();
        return;
    }
    bool persist = false;
    CDefPointer<CCarryableDef> def;
    if (ParentThing->m_pDef->GetSubDef(&def))
        persist = def->IncludeInThingScansWhenCarried;
    ParentThing->PhysicsTC->SetActive(false);
    ParentThing->SetAsGameStatePersisted(persist);
    ParentThing->Graphic->Flags59 |= 2;
    PCarryingTC = carrier;
    if (carrier->BypassFilters)
        carrier->RefreshFilters();
    CTCGraphicAppearance* carrierGraphic = carrier->ParentThing->Graphic;
    CTCGraphicAppearance* graphic = ParentThing->Graphic;
    if (carrierGraphic && graphic)
        graphic->ApplyAllMeshEffects(carrierGraphic);
    if (carrier->ParentThing->HasTC(0x75))
        carrier->ParentThing->GetTC<CTCVolumeContainmentTracker>(0x75)->OnThingCarried(*ParentThing);
    if (carrier->ParentThing->Graphic)
        ParentThing->Graphic->SetAlpha(carrier->ParentThing->Graphic->Alpha);
    CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    physics->SetCarriedPosition(*(const C3DVector*)((char*)carrier->ParentThing->PhysicsTC + 0x0c));
    physics->SetCarriedOrientation(carrier->ParentThing->PhysicsTC->GetOrientation());
    physics->ResetMovement();
}
