// NThingShot::CTCShotDefinitionTearInGround::UpdateMovement() -- moves the shot along the ground: pokes the owner's placement object (slot 38 with 0), then snaps its position to the ground height of the world map and sets it back
// Leaf: UpdateMovement
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BBDCA, length 90 bytes; vtable NThingShot::CTCShotDefinitionTearInGround
// 0x0123CF9C slot 35.
// Callees: the owner's +0x60 object virtual slot 38 (+0x98, one 0 argument) and slot 30 (+0x78, the new position by
// reference), CTCBase::GetWorldMap @ 0x00686D30 (world singleton +0x28; the manifest label "CViewBase::GetDisplayEngine" is
// propagated), CWorldMap::GetGroundSizeZAt(const C3DVector&) @ 0x0063A300 (float).
// Shape: a frame-pointer body ending in `leave` = `#pragma optimize("y", off)` + `("s", on)`; the 12-byte position
// local is copied from the object's +0xc vector through a memberwise USER copy constructor (`add eax,0Ch` then three
// dword moves through ecx; a POD copy becomes `movsd` x3 under optimize("s")); its address is pushed before
// GetWorldMap is called (arguments first); the ground height lands straight in Z (`fstp [ebp-4]`); the owner and its
// +0x60 object are re-read for each use.
#pragma optimize("y", off)
#pragma optimize("s", on)
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

struct CThingPlacement {                     // the owner's +0x60 object (class not established)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void SetPosition(const C3DVector& position);   // slot 30 (+0x78)
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37();
    virtual void Update(long flags);                       // slot 38 (+0x98)
    unsigned char _pad04[8];
    C3DVector     m_position;                // +0x0c
};

class CThing { public:
    unsigned char    _pad[0x60];
    CThingPlacement* m_pPlacement;           // +0x60
};

class CWorldMap { public:
public:
    float GetGroundSizeZAt(const C3DVector&) const;   // 0x0063A300
};

class CTCBase
{
protected:
    CWorldMap& GetWorldMap();   // 0x00686D30
};
namespace NThingShot {
class CTCShotDefinitionBase : public CTCBase
{
public:
};
}

namespace NThingShot {

struct CTCShotDefinitionTearInGround : public NThingShot::CTCShotDefinitionBase {
    void*   m_vtable;                        // +0x00
    CThing* ParentThing;                     // +0x04 (CTCBase)
    void UpdateMovement();
};

void CTCShotDefinitionTearInGround::UpdateMovement()
{
    ParentThing->m_pPlacement->Update(0);
    C3DVector position = ParentThing->m_pPlacement->m_position;
    position.Z = GetWorldMap().GetGroundSizeZAt(position);
    ParentThing->m_pPlacement->SetPosition(position);
}

}