// CTCPhysicsRigidBody::GetPossibleCollidedWithArea() -- the integer box around the position that the body can touch: the position's XY plus/minus three times the mesh's bounding-circle radius, orientation-validated
// Leaf: GetPossibleCollidedWithArea
// Retail: Fable.exe (TLC, VC7.1) @ 0x00759870, 124 bytes. CTCPhysicsRigidBody vtable 0x0126996C slot 97.
// Callees: virtual slot 93 (+0x174, GetPhysicsMeshInfo), _ftol @ 0x00BFEA70 (four times), C2DBoxI::ValidateOrientationAndFlip()
//   @ 0x00A3A150. 3.0f from the pool 0x0122DED4.
// Layout: CTCPhysicsBase Position +0x0c; CThingPhysicsMeshInfo (PDB) BoundingCircleRadius +0x18; C2DBoxI = two C2DCoordI
//   (TopLeft x,y / BottomRight x,y).
// Shape: a C2DBoxI local filled corner by corner in order (x0, y0, x1, y1: the radius stays on the FPU stack), validated, and
//   returned by value through the hidden pointer (copied out by the __forceinline memberwise copy ctor:
//   no NRVO; the implicit copy goes through ecx with `mov ecx,eax`).
class C3DVector { public: float X, Y, Z; };
class C2DCoordI { public: long X, Y; };
class C2DBoxI { public:
    C2DCoordI TopLeft;
    C2DCoordI BottomRight;
    C2DBoxI() {}
    __forceinline C2DBoxI(const C2DBoxI& o)
    {
        TopLeft.X = o.TopLeft.X;
        TopLeft.Y = o.TopLeft.Y;
        BottomRight.X = o.BottomRight.X;
        BottomRight.Y = o.BottomRight.Y;
    }
    void ValidateOrientationAndFlip();          // 0x00A3A150
};
class CThingPhysicsMeshInfo { public:
    unsigned short BankIndex;                   // +0x00
    float          RenderSizeX;                 // +0x04
    C3DVector      ToCentre;                    // +0x08
    float          BoundingRadius;              // +0x14
    float          BoundingCircleRadius;        // +0x18
};
struct CTCPhysicsRigidBody {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092();
    virtual const CThingPhysicsMeshInfo* GetPhysicsMeshInfo();   // slot 93 (+0x174)
    virtual void v094(); virtual void v095(); virtual void v096();
    virtual C2DBoxI GetPossibleCollidedWithArea();   // slot 97 (+0x184)
    void*     ParentThing;                      // +0x04
    unsigned char _pad08[0x4];
    C3DVector Position;                         // +0x0c
};

C2DBoxI CTCPhysicsRigidBody::GetPossibleCollidedWithArea()
{
    float radius = GetPhysicsMeshInfo()->BoundingCircleRadius * 3.0f;
    C2DBoxI box;
    box.TopLeft.X = (long)(Position.X - radius);
    box.TopLeft.Y = (long)(Position.Y - radius);
    box.BottomRight.X = (long)(radius + Position.X);
    box.BottomRight.Y = (long)(radius + Position.Y);
    box.ValidateOrientationAndFlip();
    return box;
}