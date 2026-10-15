// CThingPhysical::ValidateInitialisation() -- when virtual slot 17 holds and the thing has a physics component, the component's position must lie inside the world map; true otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A4D60, length 47 bytes (called from 0x006665DA, 0x006AA0D4, 0x00703356, 0x00833A9B).
// Callees: this's virtual slot 17 (+0x44), CThing::GetWorldMap() @ 0x004C79D0 (the definition manager's +0xd0
// world map; manifest "CViewBase::GetDisplayEngine" is propagated), CWorldMap::IsPosInMap(const C3DVector&) @ 0x004FC600.
// Layout: PhysicsTC +0x60 (PDB +0x70), its position (C3DVector) at +0xc.
// Shape: the position argument is pushed before the world-map fetch; the failing test returns the
// callee's own zero `al` (`if (!x) return false;` lets VC reuse it), the shared `mov al,1` tail
// serves both skipped tests.
struct C3DVector {
    float X, Y, Z;
};
struct CTCPhysicsBase {
    unsigned char _pad00[0x0c];
    C3DVector     Position;                                  // +0x0c
};
struct CWorldMap {
    bool IsPosInMap(const C3DVector& position);              // 0x004FC600
};
struct CThingPhysical {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16();
    virtual bool IsPositionedThing();                        // slot 17 (+0x44)
    unsigned char   _pad04[0x5c];
    CTCPhysicsBase* PhysicsTC;                               // +0x60
    CWorldMap* GetWorldMap();                                // 0x004C79D0
    bool ValidateInitialisation();
};

bool CThingPhysical::ValidateInitialisation()
{
    if (IsPositionedThing() && PhysicsTC != 0) {
        if (!GetWorldMap()->IsPosInMap(PhysicsTC->Position))
            return false;
    }
    return true;
}
