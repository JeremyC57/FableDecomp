// CTCAssassinRush::GetValidTeleportPos(const CThing* rusher) -- a free spot behind the collided thing for the rush to land on: straight behind it first, then swinging round in 1/36 turns and stepping further out each full turn, giving up after ten tries (back to where the rusher stands), dropped to the ground
// Leaf: GetValidTeleportPos
// Retail 0x007AA9E0, 624 bytes, `ret 8` (hidden result + one argument). Callees: CIntelligentPointer::GetPItem
// 0x00A01B50, CTCBase::GetWorld 0x00686A70 (virtual +0x34), CThing::GetPos 0x004C73D0, the placement object's
// virtual +0x118 (facing, by out pointer), CTCAssassinRush::FindValidTeleportPos 0x007AA880, C3DVector::RotateXY
// 0x00A14040, CThingCreatureBase::GetHeightZBelowAt 0x00666D40. Constants 1.0f 0x0122DED8, 1/36 0x0126C324.
// Layout: Ego_r.pdb CTCAssassinRush (PDef +0x14, CollidedThing +0x39); retail CSpecialAbilitiesAssassinRushDef
// distance +0x3c, size scale +0x40; CThing placement object +0x60 (its radius +0x34); the rusher's radius +0x34.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
public:
    void RotateXY(float);   // 0x00A14040
};
struct CPlacement {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
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
    virtual void v028();
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
    virtual const C3DVector& GetFacing(C3DVector* out);  // +0x118
    unsigned char _pad04[0x34 - 0x04];
    float         Radius;                               // +0x34
};
class CThing { public:
    unsigned char _pad00[0x34];
    float         Radius;                               // +0x34
    unsigned char _pad38[0x60 - 0x38];
    CPlacement*   Placement;                            // +0x60
    const C3DVector& GetPos() const;               // 0x004C73D0
    float GetHeightZBelowAt(const C3DVector& pos, bool a, bool b);   // 0x00666D40
};
class CBaseObject;
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CBaseObject* GetPItem() const;                           // 0x00A01B50
};
struct CWorldMapRef;
class CWorld { public:
    virtual void w00();
    virtual void w01();
    virtual void w02();
    virtual void w03();
    virtual void w04();
    virtual void w05();
    virtual void w06();
    virtual void w07();
    virtual void w08();
    virtual void w09();
    virtual void w10();
    virtual void w11();
    virtual void w12();
    virtual CWorldMapRef* GetWorldMap();               // +0x34
};
struct CAssassinRushDef {
    unsigned char _pad00[0x3c];
    float         TeleportDistance;                     // +0x3c
    float         SizeScale;                            // +0x40
};
#pragma pack(push, 1)
class CTCBase
{
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCAssassinRush : public CTCBase { public:
    void*                   __vftable;                  // +0x00
    CThing*                 ParentThing;                // +0x04
    unsigned char           _pad08[0x14 - 0x08];
    CAssassinRushDef*       PDef;                       // +0x14
    unsigned char           _pad18[0x39 - 0x18];
    CBaseIntelligentPointer CollidedThing;              // +0x39
    bool FindValidTeleportPos(const C3DVector& from, const C3DVector& to, CWorldMapRef* map, const CThing* rusher,
                              float scale);             // 0x007AA880
    C3DVector GetValidTeleportPos(const CThing* rusher);
};
#pragma pack(pop)

C3DVector CTCAssassinRush::GetValidTeleportPos(const CThing* rusher)
{
    CThing* collided = ((CThing*)CollidedThing.GetPItem());
    CWorldMapRef* map = GetWorld().GetWorldMap();
    float scale = PDef->SizeScale * rusher->Radius;
    if (scale < 1.0f)
        scale = 1.0f;
    float distance = PDef->TeleportDistance + collided->Placement->Radius + rusher->Radius;
    C3DVector centre = collided->GetPos();
    C3DVector facing;
    collided->Placement->GetFacing(&facing);
    float step = distance;
    facing *= distance;
    long tries = 0;
    C3DVector candidate = centre - facing;
    C3DVector fallback = ParentThing->GetPos();
    float angle = 0.0f;
    while (!FindValidTeleportPos(centre, candidate, map, rusher, scale)) {
        angle += 1.0f / 36.0f;
        if (angle >= 1.0f) {
            angle = 0.0f;
            distance = step + distance;
        }
        C3DVector temp;
        C3DVector back = -collided->Placement->GetFacing(&temp);
        back.RotateXY(angle);
        candidate = collided->GetPos() + back * distance;
        if (++tries > 10) {
            candidate = fallback;
            break;
        }
    }
    return C3DVector(candidate.X, candidate.Y, collided->GetHeightZBelowAt(candidate, false, true));
}