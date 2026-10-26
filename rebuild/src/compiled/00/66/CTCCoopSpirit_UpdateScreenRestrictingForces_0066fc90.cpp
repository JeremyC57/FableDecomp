// CTCCoopSpirit::UpdateScreenRestrictingForces() -- project the spirit (1.1 above its position) to the screen; within 25
//   and the force-ctor load order).
// pixels of an edge push the owning hero's creature back towards the middle (rotated into its movement frame, scaled and
// clamped); count the frames the spirit spends off screen and after five seconds' worth make it return to the hero
// Leaf: UpdateScreenRestrictingForces
//
// Retail 0x0066FC90, 655 bytes, plain `ret`.
// Callees: the display's GetScreenArea @ 0x00434B50 (C2DBoxF by hidden pointer), CThing::GetPosition @ 0x004C73D0, the
// camera's slot 13 (+0x34, world to screen), CTCBase::GetPlayerManager @ 0x00686D40, CPlayerManager::GetPlayer @ 0x004498C0,
// CPlayer::GetHeroCreature @ 0x00487DC0, CThingPlayerCreature::GetCurrentRelativeMovementAngleXY @ 0x006A9640,
// C3DVector::RotateAboutZ @ 0x00A14040 / GetLength @ 0x00A14480, CTCPhysicsBase::ApplyAcceleration @ 0x006B0210, __ftol2
// @ 0x00BFEA70, CCreatureAction_SpiritReturnToHero @ 0x00854660, CThingCreatureBase::SetCurrentAction @ 0x006644F0, the
// action's base dtor @ 0x00693EF0.
// Constants: 1.1f, 25.0f, 0.04f, -0.04f, 0.8f, 1e-4f, 3.0f; globals: the main game component @ 0x013B86A0, the frame rate
// @ 0x01375550, the spirit's off-screen seconds (5.0f) @ 0x0137EA24.
// Layout (local model): ParentThing +0x04 (player number byte +0x90), OffScreenFrames +0x14; the game component's display
// +0x28, the display's camera +0x2c.
#include <math.h>
#pragma intrinsic(fabs)
class C2DVector { public: float X, Y; };
class C2DBoxF { public: C2DVector TopLeft, BottomRight; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void RotateAboutZ(float angle);                          // 0x00A14040
    float GetLength() const;                                 // 0x00A14480
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
};
#define VF(n) virtual void v##n();
class CCamera { public:
    VF(0) VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9) VF(10) VF(11) VF(12)
    virtual bool WorldToScreen(const C3DVector& world, C2DVector* screen);   // slot 13 (+0x34)
};
#undef VF
struct CDisplay {
    unsigned char _pad00[0x2c];
    CCamera* PCamera;                                        // +0x2c
    C2DBoxF GetScreenArea() const;                           // 0x00434B50
};
class CMainGameComponent { public:
    unsigned char _pad00[0x28];
    CDisplay* PDisplay;                                      // +0x28
};
extern CMainGameComponent* g_pMainGameComponent;             // 0x013B86A0
extern long g_framesPerSecond;                               // 0x01375550
extern float g_spiritOffScreenSeconds;                       // 0x0137EA24
class CTCPhysicsBase { public: void ApplyAcceleration(const C3DVector& acceleration); };   // 0x006B0210
class CThing { public:
    unsigned char   _pad0[0x60];
    CTCPhysicsBase* PhysicsTC;                               // +0x60
    unsigned char   _pad64[0x90 - 0x64];
    char            PlayerNumber;                            // +0x90
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    float GetCurrentRelativeMovementAngleXY() const;         // 0x006A9640 (CThingPlayerCreature)
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    ~CCreatureActionBase();                                  // 0x00693EF0
};
class CThingCreatureBase;
class CCreatureAction_SpiritReturnToHero : public CCreatureActionBase { public:
    CCreatureAction_SpiritReturnToHero(CThingCreatureBase* owner);   // 0x00854660
};
class CThingCreatureBase : public CThing { public:
    void SetCurrentAction(const CCreatureActionBase& action);        // 0x006644F0
};
class CPlayer { public: CThing* GetHeroCreature(); };               // 0x00487DC0
class CPlayerManager { public: CPlayer* GetPlayer(long number); };  // 0x004498C0
class CTCCoopSpirit { public:
    virtual void v00();
    CThingCreatureBase* ParentThing;                         // +0x04
    unsigned char _pad08[0x14 - 0x08];
    long          OffScreenFrames;                           // +0x14
    CPlayerManager* GetPlayerManager();                      // 0x00686D40
    void UpdateScreenRestrictingForces();
};

void CTCCoopSpirit::UpdateScreenRestrictingForces()
{
    CDisplay* display = g_pMainGameComponent->PDisplay;
    CCamera* camera = display->PCamera;
    C2DBoxF area = display->GetScreenArea();
    const C3DVector& position = ParentThing->GetPosition();
    C3DVector spiritPos;
    spiritPos.X = position.X;
    spiritPos.Y = position.Y;
    spiritPos.Z = position.Z + 1.1f;
    C2DVector screenPos;
    camera->WorldToScreen(spiritPos, &screenPos);
    float forceX = 0.0f;
    float left = area.TopLeft.X + 25.0f;
    if (screenPos.X < left) {
        forceX = (left - screenPos.X) * 0.04f;
    } else {
        float right = area.BottomRight.X - 25.0f;
        if (screenPos.X > right)
            forceX = (right - screenPos.X) * 0.04f;
    }
    float forceY = 0.0f;
    float top = area.TopLeft.Y + 25.0f;
    if (screenPos.Y < top) {
        forceY = (top - screenPos.Y) * -0.04f;
    } else {
        float bottom = area.BottomRight.Y - 25.0f;
        if (screenPos.Y > bottom)
            forceY = (bottom - screenPos.Y) * -0.04f;
    }
    if ((float)fabs(forceX) > 0.0001f || (float)fabs(forceY) > 0.0001f) {
        long player = ParentThing->PlayerNumber;
        CThing* hero = GetPlayerManager()->GetPlayer(player)->GetHeroCreature();
        float angle = hero->GetCurrentRelativeMovementAngleXY();
        C3DVector force(forceX, forceY, 0.0f);
        force.RotateAboutZ(angle);
        force *= 0.8f;
        float length = force.GetLength();
        if (length > 0.0001f) {
            if (length > 3.0f)
                length = 3.0f;
            force *= length;
            hero->PhysicsTC->ApplyAcceleration(force);
        }
    }
    if (screenPos.X < area.TopLeft.X || screenPos.X > area.BottomRight.X || screenPos.Y < area.TopLeft.Y
        || screenPos.Y > area.BottomRight.Y)
        ++OffScreenFrames;
    else
        OffScreenFrames = 0;
    if (OffScreenFrames > (long)((float)g_framesPerSecond * g_spiritOffScreenSeconds)) {
        OffScreenFrames = 0;
        CThingCreatureBase* parent = ParentThing;
        parent->SetCurrentAction(CCreatureAction_SpiritReturnToHero(parent));
    }
}