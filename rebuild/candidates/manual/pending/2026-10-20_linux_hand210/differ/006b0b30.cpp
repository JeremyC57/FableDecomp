// CTCPhysicsBase::InitialActivate() -- on activation of a thing that belongs in the map: if its mapwho component accepts the offset position, place it in the mapwho there
// Grade: DIFFER(217v217) -- one x87 operand order: in the PlaceInMapwho argument retail loads the offset's Y first (`fld [eax+4] ; fadd [esi+10h]`), this source loads Position.Y first
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B0B30, 217 bytes (plain `ret`). CTCPhysicsBase vtable 0x0125D2C4 slot 5 (PDB ?InitialActivate@CTCPhysicsBase@@UAEXXZ).
// Callees: virtual CThing::IsToBePlacedInMap() const slot 17 (+0x44, 0x004C7B80; PDB ?IsToBePlacedInMap@CThing@@UBE_NXZ),
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (interface 0x3a = CTCMapwho), virtual GetOffsetVector()
//   const slot 28 (+0x70, PDB ?GetOffsetVector@CTCPhysicsBase@@MBE?AVC2DVector@@XZ, hidden result), CTCMapwho::
//   IsToBePlacedInMapwho(const C3DVector&) @ 0x0071A6D0, CTCMapwho::PlaceInMapwho(const C3DVector&) @ 0x0071A710.
// Shape: `CTCMapwho* mapwho = 0; if (HasTC) mapwho = GetTC;` (preloaded `xor edi,edi`); the first GetOffsetVector() result is
//   discarded (retail calls it once more than it uses it); the offset position is a C2DVector + C3DVector operator building a
//   NAMED result (frame 0x20: the inner result keeps only Z in memory, X/Y are forwarded on the x87 stack) copied by the
//   memberwise user copy ctor into the argument temporary. In the first use the lane keeps the written operand order, in the
//   second it swaps the operands of the X and Y sums. Tried: inline GetMapwhoPosition with a const& offset (reads the frame,
//   0x14 frame), operator+(C2DVector, C3DVector) with a named result (only PlaceInMapwho's X swapped), the same with a returned
//   temporary (0x14 frame), a second operator+(C3DVector, C2DVector) for PlaceInMapwho written `position.X + offset.X` (X fixed,
//   Y now swapped). Next: write that second operator's Y as `position.Y + offset.Y` too (one-try).
struct C2DVector { float X, Y; };
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
inline C3DVector operator+(const C2DVector& offset, const C3DVector& position)
{
    C3DVector result(offset.X + position.X, offset.Y + position.Y, position.Z);
    return result;
}
inline C3DVector operator+(const C3DVector& position, const C2DVector& offset)
{
    C3DVector result(position.X + offset.X, offset.Y + position.Y, position.Z);
    return result;
}
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CTCMapwho {
    bool IsToBePlacedInMapwho(const C3DVector& position);   // 0x0071A6D0
    void PlaceInMapwho(const C3DVector& position);          // 0x0071A710
};
struct CThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual bool IsToBePlacedInMap() const;             // slot 17 (+0x44) 0x004C7B80
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                     // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return it->component;
    }
};
struct CThingPhysicsMeshInfo;
struct CTCPhysicsBase {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void OnCreate();                                // slot 4
    virtual void InitialActivate();                         // slot 5
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual C2DVector GetOffsetVector() const;              // slot 28 (+0x70)
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void v092();
    virtual CThingPhysicsMeshInfo* GetPhysicsMeshInfo();    // slot 93 (+0x174)
    CThing*   ParentThing;                              // +0x04 (CTCBase)
    bool      ToBeRemoved;                              // +0x08
    bool      IsInGlobalUpdateTCs;                      // +0x09
    unsigned char _pad0a[2];
    C3DVector Position;                                 // +0x0c
};

void CTCPhysicsBase::InitialActivate()
{
    if (ParentThing->IsToBePlacedInMap()) {
        CThing* thing = ParentThing;
        CTCMapwho* mapwho = 0;
        if (thing->HasTC(0x3a))
            mapwho = (CTCMapwho*)thing->GetTC(0x3a);
        GetOffsetVector();
        if (mapwho->IsToBePlacedInMapwho(GetOffsetVector() + Position))
            mapwho->PlaceInMapwho(Position + GetOffsetVector());
    }
}
