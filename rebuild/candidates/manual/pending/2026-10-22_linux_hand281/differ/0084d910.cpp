// Grade: DIFFER(525v542) -- calls, prologue and the exp/sqrt intrinsics match; retail evaluates the height scale (virtual GetStretchHeight) before the radial scale and keeps it on the x87 stack until the final Z add, and lays out heroPos/flat/offset in a different frame order; the lane hoists the radial scale and the X/Y sums above the virtual call. Forms tried: three float locals, C2DVector flat, inline scale, position.Z += height, rebuilt position.
// NCameraModes::CBase::UpdateCameraStretchAroundHero(C3DVector& position, C3DVector& lookAt) -- when the mode
//   stretches around the hero and the observed thing is the main player's creature, pull the camera towards the hero:
//   the horizontal offset from the hero is scaled towards radius/a and the height towards GetStretchHeight()/c,
//   both blended by exp(-0.01 * d^2) of the horizontal distance beyond the hero's mesh radius; the look-at point
//   keeps its offset from the camera
// Leaf: UpdateCameraStretchAroundHero
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084D910, 540 bytes (544 with padding); `ret 8`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPlayer 0x004C7A10, CPlayer::IsMainPlayer
//   0x00487520, CThing::GetPosition 0x004C73D0, the creature's def-float getters 0x006625C0 (+0x60) / 0x006625E0
//   (+0x5C) (manifest "CTCWeapon::GetAnimationSpeedValue" is propagated), CThingCreatureBase::GetCurrentMeshRadius
//   0x00664BA0, virtual slots 8 (+0x20) and 15 (+0x3c). exp is the x87 intrinsic; -0.01f is 0x01231564.
// Model: the landed NCameraModes::CBase::UpdateCameraOcclusion 0x0084DB30 (camera manager +0x40, observed thing +0x100).
extern "C" double __cdecl exp(double x);
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(exp, sqrt)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct CPlayer {
    bool IsMainPlayer() const;                  // 0x00487520
};
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char ThingType;                    // +0x10 (2: creature)
    const C3DVector& GetPosition() const;       // 0x004C73D0
    CPlayer* GetPlayer() const;                 // 0x004C7A10
};
struct CThingCreatureBase : CThing {
    float GetStretchRadius() const;             // 0x006625C0 (def +0x60)
    float GetStretchHeightScale() const;        // 0x006625E0 (def +0x5C)
    float GetCurrentMeshRadius() const;         // 0x00664BA0
};
struct CBaseIntelligentPointer {
    CThingCreatureBase* GetPItem();             // 0x00A01B50
};
struct CCameraManager {
    unsigned char           _pad[0x100];
    CBaseIntelligentPointer m_observedThing;    // +0x100
    CThingCreatureBase* GetObservedThing() { return m_observedThing.GetPItem(); }
};

namespace NCameraModes {
class CBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07();
    virtual bool UsesCameraStretchAroundHero(); // slot 8 (+0x20)
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual float GetStretchHeight();           // slot 15 (+0x3c)
    unsigned char   _pad04[0x3c];
    CCameraManager* m_pManager;                 // +0x40
    void UpdateCameraStretchAroundHero(C3DVector& position, C3DVector& lookAt);
};
}

void NCameraModes::CBase::UpdateCameraStretchAroundHero(C3DVector& position, C3DVector& lookAt)
{
    if (!UsesCameraStretchAroundHero())
        return;
    if (m_pManager->GetObservedThing()->ThingType != 2)
        return;
    if (!m_pManager->GetObservedThing()->GetPlayer()->IsMainPlayer())
        return;
    CThingCreatureBase* hero = m_pManager->GetObservedThing();
    C3DVector offset = lookAt - position;
    C3DVector heroPos = m_pManager->GetObservedThing()->GetPosition();
    C2DVector flat(position.X - heroPos.X, position.Y - heroPos.Y);
    float dz = position.Z - heroPos.Z;
    float stretchRadius = hero->GetStretchRadius();
    float radius = hero->GetCurrentMeshRadius();
    float distance = (float)sqrt(flat.Y * flat.Y + flat.X * flat.X) - radius;
    if (distance < 0.0f)
        distance = 0.0f;
    float blend = (float)exp(distance * distance * -0.01f);
    float heightScale = hero->GetStretchHeightScale();
    float height = ((GetStretchHeight() / heightScale - 1.0f) * blend + 1.0f) * dz;
    float scale = (radius / stretchRadius - 1.0f) * blend + 1.0f;
    position = heroPos + C3DVector(flat.X, flat.Y, 0.0f) * scale;
    position.Z += height;
    lookAt = offset + position;
}
