// CObstacleNavigatorExternalInterface::PeekNavigatorOrientation() const -- the obstacle's facing as a unit 2D vector: the physics component's 3D direction (virtual slot 70) cut to X/Y, normalised, returned by hidden pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DF3F7, length 61 bytes (`ret 4`). Callees: the physics component's
// virtual slot 70 (+0x118, a C3DVector returned through the hidden pointer [ebp-14h]), C2DVector::Normalise @ 0x00A14540.
// Layout: Obstacle (CThing*) at +4 (PDB); CThing::PhysicsTC at retail +0x60 (CThing::GetPosition @ 0x004C73D0 reads
// `[this+60h]+0Ch`). Shape: a frame-pointer size TU ending in `leave` = #pragma optimize("y", off) + ("s", on); the
// direction is a named C2DVector local converted from the call result (both words moved as integers), normalised in
// place, then returned through C2DVector's memberwise copy constructor, which in a /Os TU copies the first float
// through the x87 (`fld ; fstp`) and the second through an integer register.
// Leaf: PeekNavigatorOrientation
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};

struct C2DVector {
    float X;
    float Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) {}
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) {}
    C2DVector* Normalise();                        // 0x00A14540
};

class CTCPhysicsBase {
public:
    virtual void v000();
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual C3DVector GetForward() const;          // slot 70 (+0x118): the facing direction
};

class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                     // +0x60
};

class CObstacleNavigatorExternalInterface {
public:
    virtual void v000();
    CThing* Obstacle;                              // +0x04
    virtual C2DVector PeekNavigatorOrientation() const;
};

C2DVector CObstacleNavigatorExternalInterface::PeekNavigatorOrientation() const
{
    C2DVector direction(Obstacle->PhysicsTC->GetForward());
    direction.Normalise();
    return direction;
}
