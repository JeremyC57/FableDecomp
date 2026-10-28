// CGroundNavigatorExternalInterface::PeekNavigatorRadius(bool physical) const -- the navigator's radius: the stored def radius, or for a physical query the creature's physics radius, padded by 0.1 for the player
// Leaf: ?PeekNavigatorRadius@CGroundNavigatorExternalInterface@@UBEM_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CDA10, 49 bytes, thiscall, `ret 4`.
// Identity (proven): vtable 0x0125EC9C (RTTI CGroundNavigatorExternalInterface) slot 6; Ego_r.pdb donor order
//   PeekNavigatorRadius / PeekNavigatorOrientation / PeekNavigatorPosition2D / PeekNavigatorPosition3D / IsDead /
//   GetMinPathCost / GetMaxPathCost matches slots 6, 7 (landed 0x006CDA50), 8, 9 (landed 0x006CDAB0), 21 (landed
//   0x006CDB20), 18, 19; Ego_r.pdb ?PeekNavigatorRadius@CGroundNavigatorExternalInterface@@UBEM_N@Z.
// Callees: the interface's own virtual slot 20 (+0x50), IsPlayer() const (0x006CB5F0, landed). Float pool 0x0122EE58 = 0.1f.
// Layout: Creature +0x04, Radius +0x08 (engine/CGroundNavigatorExternalInterface.h, modelled locally: the class is
//   polymorphic); the creature's physics body +0x60, its radius +0x34 (landed 0x006CDD00 model).
// Shape: the physics radius is read into a named float before the virtual call (it lives in the dead bool slot) and
//   padded in place, so the reload sits before the bool test.
class CPhysicsBody {                               // the creature's physics body at +0x60
public:
    unsigned char _pad00[0x34];
    float         Radius;                          // +0x34
};
class CThingCreatureBase { public:
    unsigned char  _pad00[0x60];
    CPhysicsBody*  PBody;                          // +0x60
};
class CGroundNavigatorExternalInterface { public:  // vtable 0x0125EC9C
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual float PeekNavigatorRadius(bool physical) const;   // slot 6
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19();
    virtual bool IsPlayer() const;                 // slot 20 (+0x50), 0x006CB5F0
    CThingCreatureBase* Creature;                  // +0x04
    float               Radius;                    // +0x08
};

float CGroundNavigatorExternalInterface::PeekNavigatorRadius(bool physical) const
{
    if (physical) {
        float radius = Creature->PBody->Radius;
        if (IsPlayer())
            radius += 0.1f;
        return radius;
    }
    return Radius;
}