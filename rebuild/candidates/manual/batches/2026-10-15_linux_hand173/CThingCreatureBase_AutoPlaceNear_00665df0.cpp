// CThingCreatureBase::AutoPlaceNear(const C3DVector& position, float radius) -- find a wander position near the point and, if one exists, move the creature there (navigation reset before and after the physics reset)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00665DF0, 149 bytes (`ret 8`).
// Callees: CInterfaceMap::LowerBound @ 0x0040F020, CTCCreatureNavigation::GetNearbyWanderPosition(const C3DVector*, float,
//   C3DVector*, bool) @ 0x006CB6E0, CTCCreatureNavigation::ResetNavigator @ 0x006CB6B0 (forwards to its +0xc object's
//   slot 18; label "CScriptThing::operator==" propagated), CTCPhysicsBase::ResetPosition slot 31 (+0x7c).
// Layout: retail CThing: interface bitset +0x20 (TC 7 = CTCCreatureNavigation: bit 7 of the byte +0x20), CVectorMap +0x44,
//   physics component +0x60.
// Shape: `T* p = 0; if (HasTC(k)) p = GetTC(k);` (the preloaded `xor esi,esi`), the result vector initialised from the
//   argument through a `__forceinline` memberwise copy ctor (dword copies straight from the argument pointer in eax;
//   the implicit copy goes through `mov ecx,eax`), the `false` pushed first.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CTCCreatureNavigation {
    bool GetNearbyWanderPosition(const C3DVector* from, float radius, C3DVector* result, bool flag);    // 0x006CB6E0
    void ResetNavigator();                      // 0x006CB6B0
};
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void ResetPosition(const C3DVector& position);     // slot 31 (+0x7c)
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
};

struct CThingCreatureBase : CThing {
    bool AutoPlaceNear(const C3DVector& position, float radius);
};

bool CThingCreatureBase::AutoPlaceNear(const C3DVector& position, float radius)
{
    CTCCreatureNavigation* navigation = 0;
    if (HasTC(7))
        navigation = GetTC<CTCCreatureNavigation>(7);
    C3DVector placed = position;
    if (navigation->GetNearbyWanderPosition(&position, radius, &placed, false)) {
        navigation->ResetNavigator();
        PhysicsTC->ResetPosition(placed);
        navigation->ResetNavigator();
        return true;
    }
    return false;
}
