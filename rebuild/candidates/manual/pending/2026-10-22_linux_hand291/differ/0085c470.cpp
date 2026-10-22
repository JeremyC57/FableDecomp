// Grade: DIFFER(857v857) -- frame and slots match (speed as a C2DVector copy, the first section in its own block); residue is register choice: the hit-locations TC merge lands in eax (+mov ecx,eax) not ecx, one fstp/mov ecx,esi swap, and the edx/eax/ecx rotation in the ground add and LookAt copy
// NCameraModes::CMoveOnCylinder::Update() -- probe
// Leaf: ?Update@CMoveOnCylinder
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector& operator=(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; return *this; }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    void Rotate(const C3DVector& axis, float angle);                // 0x00A13C60
};
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}
};
struct CTCHitLocations {
    void GetBoundingSphere(C3DVector& centre, float& radius, bool exact) const;   // 0x00815420
};
struct CTCBase;
struct CInterfaceMap {                                   // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* key) const;           // 0x004365B0
};
struct CThingAppearance {
    unsigned char _pad00[0x34];
    float         Radius;                                // +0x34
};
struct CThing {
    unsigned char     _pad00[0x20];
    unsigned long     UsingComponents[4];                // +0x20
    unsigned char     _pad30[0x44 - 0x30];
    CInterfaceMap     Components;                        // +0x44
    unsigned char     _pad4c[0x60 - 0x4c];
    CThingAppearance* Appearance;                        // +0x60
    unsigned char     _pad64[0x91 - 0x64];
    unsigned char     Flags91;                           // +0x91 (bit 0: dead)
    const C3DVector& GetPosition() const;                // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline const T* GetConstTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (const T*)it->component;
    }
};
struct CUserProfileManager {
    unsigned char _pad00[0x0c];
    bool          UseSecondProfile;                      // +0x0c
    unsigned char _pad0d[0x15 - 0x0d];
    bool          InvertX1;                              // +0x15
    bool          InvertY1;                              // +0x16
    unsigned char _pad17[0x7c - 0x17];
    float         Sensitivity1;                          // +0x7c
    unsigned char _pad80[0xd5 - 0x80];
    bool          InvertX2;                              // +0xd5
    bool          InvertY2;                              // +0xd6
    unsigned char _padd7[0x130 - 0xd7];
    float         Sensitivity2;                          // +0x130
    static CUserProfileManager& __fastcall Get();        // 0x0040D2A0
    bool  GetInvertX() const { return UseSecondProfile ? InvertX2 : InvertX1; }
    bool  GetInvertY() const { return UseSecondProfile ? InvertY2 : InvertY1; }
    float GetSensitivity() const { return UseSecondProfile ? Sensitivity2 : Sensitivity1; }
};
struct CGameCameraManager {
    unsigned char _pad00[0xc8];
    float         DistanceScale;                         // +0xc8
    float         RotateSpeed;                           // +0xcc
    float         HeightSpeed;                           // +0xd0
    float         MinHeightAboveGround;                  // +0xd4
};
struct CWorldMapAccess {
    float GetHeight() const;                             // 0x0063E480
};
struct CCamWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual const CWorldMapAccess* GetMapAt(const C3DVector& pos) const;   // slot 13 (+0x34)
};
namespace NCameraModes {
class CBase {
public:
    virtual void v00();
    unsigned char       _pad04[0x10 - 0x04];
    C3DVector           LookAtPosition;                  // +0x10
    unsigned char       _pad1c[0x44 - 0x1c];
    CGameCameraManager* CameraManager;                   // +0x44
    unsigned char       _pad48[0x98 - 0x48];
    const CThing* GetObservedThing() const;              // 0x0084F1B0
    const CCamWorld* GetWorld() const;                   // 0x0084D050
    void SetGoToPosition(const C3DVector& pos, float rate);   // 0x0084E1A0
};
class CMoveOnCylinder : public CBase {
public:
    C2DVector Speed;                                     // +0x98
    unsigned char _pada0[4];
    C3DVector Direction;                                 // +0xa4
    unsigned char _padb0[0xbc - 0xb0];
    C3DVector Up;                                        // +0xbc
    float     Angle;                                     // +0xc8
    float     Height;                                    // +0xcc
    void Update();
};

void CMoveOnCylinder::Update()
{
    const CThing* thing = GetObservedThing();
    if (!thing || (thing->Flags91 & 1))
        return;
    {
        C2DVector speed = Speed;
        if (CUserProfileManager::Get().GetInvertX())
            speed.X *= -1.0f;
        if (CUserProfileManager::Get().GetInvertY())
            speed.Y *= -1.0f;
        float rotateSpeed = CameraManager->RotateSpeed;
        float heightSpeed = CameraManager->HeightSpeed;
        float sensitivity = CUserProfileManager::Get().GetSensitivity();
        if (sensitivity < 0.0f)
            sensitivity = 0.0f;
        else if (sensitivity > 1.0f)
            sensitivity = 1.0f;
        sensitivity += 0.5f;
        Angle += rotateSpeed * sensitivity * speed.X;
        Height += sensitivity * heightSpeed * speed.Y;
        if (Height < 0.0)
            Height = 0.0f;
        else if (Height > 1.0f)
            Height = 1.0f;
    }

    float radius = GetObservedThing()->Appearance->Radius;
    float scale = CameraManager->DistanceScale;
    const CThing* observed = GetObservedThing();
    const CTCHitLocations* hitLocations;
    if (observed->HasTC(0x7f))
        hitLocations = observed->GetConstTC<CTCHitLocations>(0x7f);
    C3DVector centre;
    float sphereRadius;
    hitLocations->GetBoundingSphere(centre, sphereRadius, false);
    sphereRadius *= 2.0f;
    C3DVector dir = Direction;
    dir.Rotate(Up, Angle);
    C3DVector pos = GetObservedThing()->GetPosition();
    C3DVector offset = dir * radius * scale;
    C3DVector probe = pos + offset + Up;
    C3DVector lookAt = pos + Up * sphereRadius * Height;
    C3DVector goTo = lookAt + offset;
    float ground = GetWorld()->GetMapAt(probe)->GetHeight() + CameraManager->MinHeightAboveGround;
    if (goTo.Z < ground)
        goTo.Z = ground;
    SetGoToPosition(goTo, 1.0f);
    LookAtPosition = lookAt;
}
}
