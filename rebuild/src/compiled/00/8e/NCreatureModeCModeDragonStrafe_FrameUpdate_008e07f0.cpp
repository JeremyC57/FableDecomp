// NCreatureMode::CModeDragonStrafe::FrameUpdate() -- base update; at the end of the strafe timer set the "strafe out of" action, otherwise move the dragon one frame's worth of Speed along its physics facing
// Leaf: ?FrameUpdate@CModeDragonStrafe
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E07F0, 193 bytes (+ int3 padding), `ret`; vtable 0x012686C8 slot 3.
// Callees: CModeBase::FrameUpdate @ 0x0073AC50 (qualified), CCreatureAction_DragonStrafeOutOf(CThingCreatureBase*) @ 0x008E4AD0
//   (stores vtable 0x0128702C = CCreatureAction_DragonStrafeOutOf), CThingCreatureBase::SetCurrentAction @ 0x006644F0,
//   ~CCombatActionBase @ 0x0062E900, the physics body's (+0x60) virtual slot 70 GetFacing (+0x118, C3DVector by value through
//   the hidden pointer) and slot 30 SetPosition (+0x78), CThing::GetPosition @ 0x004C73D0.
// Globals: the frames-per-second long @ 0x01375550 (`fild`).
// Name: FableWin.pdb `?FrameUpdate@CModeDragonStrafe@NCreatureMode@@UAEXXZ` (slot 3).
// Layout: retail CModeDragonStrafe (the landed ctor 0x008DDFB0): Timer +0x44, Speed +0x54.
// Shape: the creature is a named local read before the timer decrement (edi), used for the action and the facing; the
//   position and the SetPosition receiver re-read ParentThing. The per-frame step is a named float (Speed / fps), the offset a
//   named C3DVector from an inline scaled copy (X, Y, Z stores in order) read straight through the facing temporary, and
//   the new position a NAMED local from an inline operator+ whose three-argument ctor is evaluated right to left (Z, Y, X
//   on the x87 stack, then three fstp). Unnamed, the sum shares the facing temporary's slot (frame 0x130, not 0x13c);
//   a named facing local reads it from the frame instead of through eax.
struct CCreatureActionBase;

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator*(float s) const
    {
        C3DVector r;
        r.X = X * s;
        r.Y = Y * s;
        r.Z = Z * s;
        return r;
    }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};

struct CPhysicsBody {                      // the creature's physics body at +0x60
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void SetPosition(const C3DVector& position);   // slot 30 (+0x78)
    virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036();
    virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042();
    virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066();
    virtual void v067(); virtual void v068(); virtual void v069();
    virtual C3DVector GetFacing();         // slot 70 (+0x118), by value through the hidden pointer
};

struct CThingCreatureBase {
    unsigned char _pad00[0x60];
    CPhysicsBody* PPhysics;                // +0x60
    const C3DVector& GetPosition() const;  // 0x004C73D0
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

struct CCreatureActionBase {               // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa4];
};
struct CCombatActionBase : CCreatureActionBase {   // retail sizeof 0x114
    unsigned char _combat[0x6c];
    ~CCombatActionBase();                  // 0x0062E900
};
struct CCreatureAction_DragonStrafeOutOf : CCombatActionBase {
    CCreatureAction_DragonStrafeOutOf(CThingCreatureBase* owner);   // 0x008E4AD0
};

extern long g_FramesPerSecond;             // 0x01375550

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThingCreatureBase* ParentThing;       // +0x04
    unsigned char       _body[0x14];
    void FrameUpdate();                    // 0x0073AC50
};

namespace NCreatureMode {
struct CModeDragonStrafe : CModeBase {     // vtable 0x012686C8
    unsigned char _flying[0x1c];           // +0x1c CModeDragonFlyingBase (spline, percentage, leg alpha)
    unsigned char Filter[0x0c];            // +0x38 CAnimFilterOfGroup
    long          Timer;                   // +0x44
    long          BurninateStartTime;      // +0x48
    long          BurninateDuration;       // +0x4c
    bool          Burninated;              // +0x50
    float         Speed;                   // +0x54
    void FrameUpdate();
};
}

void NCreatureMode::CModeDragonStrafe::FrameUpdate()
{
    CModeBase::FrameUpdate();
    CThingCreatureBase* creature = ParentThing;
    if (--Timer <= 0) {
        creature->SetCurrentAction(CCreatureAction_DragonStrafeOutOf(creature));
        return;
    }
    float step = Speed / g_FramesPerSecond;
    C3DVector offset = creature->PPhysics->GetFacing() * step;
    C3DVector position = ParentThing->GetPosition() + offset;
    ParentThing->PPhysics->SetPosition(position);
}