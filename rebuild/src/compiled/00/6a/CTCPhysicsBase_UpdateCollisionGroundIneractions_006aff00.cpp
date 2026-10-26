// CTCPhysicsBase::UpdateCollisionGroundIneractions(const C3DVector& pos) -- tell the parent thing it touched the ground (collision kind 3) at pos, with the world map's ground face normal there
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AFF00, 63 bytes (the misspelt name is the catalog's).
// Callees: 0x00686A70 (this-ignoring member returning g_pMainGameComponent->+0x24, the world; manifest "CViewBase::GetDisplayEngine",
//   propagated), the world's virtual slot 12 (+0x30) = its world map (0x006AF620 is the same chain out of line),
//   CWorldMap::GetGroundFaceNormalAt @ 0x0063AB10 (C3DVector by hidden pointer), the thing's virtual slot 41 (+0xa4).
// Layout: engine/CTCPhysicsBase.h (+0x04 the parent thing).
// Shape: the thing is read into a local BEFORE the world call (esi), the nested call's arguments (0, pos, &result) are pushed
//   before the world-map virtual is called (VC evaluates the object expression after the arguments).
// Leaf: UpdateCollisionGroundIneractions
#define CTCPhysicsBase CTCPhysicsBaseLayout
#include "engine/CTCPhysicsBase.h"
#undef CTCPhysicsBase
class CWorld;

class CTCBase
{
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};
class CTCPhysicsBase : public CTCBase, public CTCPhysicsBaseLayout
{
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
};

class CWorldMap { public:
    C3DVector GetGroundFaceNormalAt(const C3DVector& pos, long flags);   // 0x0063AB10
};

class CWorld { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual CWorldMap* GetWorldMap();       // slot 12 (+0x30)
};

class CThing { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void OnCollision(long kind, const C3DVector& pos, const C3DVector& normal);   // slot 41 (+0xa4)
};

struct CTCPhysicsBase_Methods : CTCPhysicsBase {
    CThing* GetThing() { return reinterpret_cast<CThing*>(sub); }
    void UpdateCollisionGroundIneractions(const C3DVector& pos);
};

void CTCPhysicsBase_Methods::UpdateCollisionGroundIneractions(const C3DVector& pos)
{
    CThing* thing = GetThing();
    thing->OnCollision(3, pos, GetWorld().GetWorldMap()->GetGroundFaceNormalAt(pos, 0));
}