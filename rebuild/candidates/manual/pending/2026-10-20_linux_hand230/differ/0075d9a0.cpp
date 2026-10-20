// CTCCameraPointBuilding::vfunc_17(CCharString& error) const -- validity check: the camera point must hang off a live building through its owner link, may not be both a start and an exit point, and its inside/outside position must agree with the start/exit flags; writes the reason into `error` and returns false on the first failure
// Grade: DIFFER(255v256) -- callee-saved push placement only: retail pushes esi at entry (push ecx/esi/edi), ours sinks it below the HasTC branch so the not-attached exit pops two registers; block layout matches with the inline out-param GetInterface (probe v1: plain HasTC moves that exit up, v2: if/else moves it to the end, single-exit else-if chain adds a bl flag)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075D9A0, 254 bytes (+ int3 padding), `ret 4`. Vtable 0x0123ECBC
//   (CTCCameraPointBuilding) slot 17, the slot the landed CTCCameraPointScripted::vfunc_17 0x0075CDF0 fills.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (inlined GetInterface for type 0x33 =
//   word 1 bit 19), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPosition @ 0x004C73D0,
//   CThingBuilding::IsPointInside(const C3DVector&) @ 0x0082D000, CCharString::operator=(const char*) @ 0x0099EFE0.
// Strings: 0x01269FF0 "CAMERA_POINT_BUILDING is not attached to a building!", 0x0126A028 "CAMERA_POINT_BUILDING is both
//   start and end point!", 0x01269F30 "CAMERA_POINT_BUILDING is attached to something other than a building!",
//   0x01269FB8 "point is inside building but flagged as an exit point!", 0x01269F78 "point is not inside building but
//   is flagged as a start point!".
// Layout = Ego_r.pdb CTCCameraPointBuilding (0x44): InMyBuilding +0x40, ExitPoint +0x41, StartPoint +0x42; the
//   interface-0x33 component's owner link at the odd +0x0d (landed GetOwnerBuilding 0x0075D940); CThing ThingTypeFlags
//   +0x6c (0x40 = building), Dead bit 0 of +0x91.
// Shape: early-return ifs, each failure its own `error = "..."; return false;` tail; the component is fetched (ternary
//   GetTC) before the start/exit test so its load is scheduled between the flag test and the branch; IsPointInside's
//   result is a named bool tested in two ifs.
struct CCharString {
    void* PStringData;
    CCharString& operator=(const char* s);      // 0x0099EFE0
};
struct C3DVector { float X, Y, Z; };
struct CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
struct CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;               // +0x6c (0x40 = building)
    unsigned char _pad70[0x91 - 0x70];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    const C3DVector& GetPosition() const;       // 0x004C73D0
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            *ppOut = GetTC<T>(interfaceType);
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long wanted) const
    {
        long type = wanted;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
struct CThingBuilding : CThing {
    bool IsPointInside(const C3DVector& point) const;   // 0x0082D000
};
#pragma pack(push, 1)
struct CTCOwnedEntity {                         // the interface-0x33 component (owner link at +0x0d)
    unsigned char _pad00[0x0d];
    CBaseIntelligentPointer POwner;             // +0x0d
};
#pragma pack(pop)
struct CTCCameraPointBuilding {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x40 - 0x08];          // CTCCameraPointDefinitionBase members
    bool InMyBuilding;                          // +0x40
    bool ExitPoint;                             // +0x41
    bool StartPoint;                            // +0x42
    bool vfunc_17(CCharString& error) const;
};

bool CTCCameraPointBuilding::vfunc_17(CCharString& error) const
{
    CTCOwnedEntity* owned;
    if (!ParentThing->GetInterface(&owned, 0x33)) {
        error = "CAMERA_POINT_BUILDING is not attached to a building!";
        return false;
    }
    if (StartPoint && ExitPoint) {
        error = "CAMERA_POINT_BUILDING is both start and end point!";
        return false;
    }
    CThing* owner = owned->POwner.GetPItem();
    if (!owner || owner->IsDead() || !(owner->ThingTypeFlags & 0x40)) {
        error = "CAMERA_POINT_BUILDING is attached to something other than a building!";
        return false;
    }
    bool inside = ((CThingBuilding*)owner)->IsPointInside(ParentThing->GetPosition());
    if (ExitPoint && inside) {
        error = "point is inside building but flagged as an exit point!";
        return false;
    }
    if (StartPoint && !inside) {
        error = "point is not inside building but is flagged as a start point!";
        return false;
    }
    return true;
}
