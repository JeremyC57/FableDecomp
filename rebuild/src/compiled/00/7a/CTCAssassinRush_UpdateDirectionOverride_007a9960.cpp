// CTCAssassinRush::UpdateDirectionOverride() -- while the player pushes the analogue stick (input event 0x11) past 0.05, take the stick as the rush's override direction (Y flipped), normalised and rotated by the hero's relative movement angle
// Leaf: UpdateDirectionOverride
// Retail 0x007A9960, 336 bytes, `ret`. Called once, from 0x007AA806 (twin of CTCBattleCharge::UpdateDirectionOverride 0x007A7000).
// Callees: CInputEvent ctor 0x00A04410 / dtor 0x00A03B30, GFGetSystemManager 0x009A4EC0 (input manager +0x58), CThing::GetPlayer
// 0x004C7A30, CPlayer's player-number accessor 0x00487510 (+0x228), CInputManager::IsJoystickEventInQueue(EInputEventType,
// long, CInputEvent*, long) 0x009F4C60, CInputEvent's analogue position 0x00A043A0 (C2DVector by hidden pointer),
// CThingPlayerCreature::GetCurrentRelativeMovementAngleXY 0x006A9640, C3DVector's rotate-about-Z 0x00A14040.
// Globals: 0.05f 0x0126CEF4, the reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C.
// Layout = Ego_r.pdb CTCAssassinRush (packed: DirectionOverride +0x48, DirectionOverrideVector +0x49), ParentThing +4.
// Shape: CInputEvent is a 0x34-byte frame object (it fills the whole frame top); the input manager is a named local read
// before the player argument; the stick length is the inline `sqrt(Y*Y + X*X)` (retail squares Y first); the override
// vector's members are stored one by one (X, Y, the flag, then Z) and normalised with C3DVector's inline fast Normalise
// (landed CTCAssassinRush::SetupFacingDirection 0x007A6E20 model); the angle is a named float local (`fstp ; mov ; push`).
#include <math.h>
#pragma intrinsic(sqrt)
extern unsigned long g_InvSqrtMantissaTable[128];          // 0x013CE558

class C2DVector { public:
    float X, Y;
    C2DVector() {}
    float GetLength() const { return (float)sqrt(Y * Y + X * X); }
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
public:
    void RotateXY(float);   // 0x00A14040
};
class CInputEvent { public:
    unsigned char _body[0x34];
    CInputEvent();                                          // 0x00A04410
    ~CInputEvent();                                         // 0x00A03B30
    C2DVector GetAnaloguePosition() const;                  // 0x00A043A0
};
class CInputManager { public:
    bool IsJoystickEventInQueue(long type, long a, CInputEvent* event, long player);   // 0x009F4C60
};
class CSystemManager { public:
    unsigned char  _pad00[0x58];
    CInputManager* InputManager;                            // +0x58
};
CSystemManager* __fastcall GFGetSystemManager();            // 0x009A4EC0
class CPlayer { public:
    long GetPlayerNumber() const;                           // 0x00487510
};
class CThing { public:
    CPlayer* GetPlayer() const;                             // 0x004C7A30
};
class CThingPlayerCreature : public CThing { public:
    float GetCurrentRelativeMovementAngleXY() const;        // 0x006A9640
};

#pragma pack(push, 1)
class CTCAssassinRush {
public:
    void*                 __vftable;                        // +0x00
    CThingPlayerCreature* ParentThing;                      // +0x04
    unsigned char         _pad08[0x48 - 0x08];
    bool                  DirectionOverride;                // +0x48
    C3DVector             DirectionOverrideVector;          // +0x49
    void UpdateDirectionOverride();
};
#pragma pack(pop)

void CTCAssassinRush::UpdateDirectionOverride()
{
    CInputEvent event;
    CThingPlayerCreature* thing = ParentThing;
    CInputManager* input = GFGetSystemManager()->InputManager;
    if (input->IsJoystickEventInQueue(0x11, 0, &event, thing->GetPlayer()->GetPlayerNumber())) {
        C2DVector stick = event.GetAnaloguePosition();
        if (stick.GetLength() > 0.05f) {
            DirectionOverrideVector.X = stick.X;
            DirectionOverrideVector.Y = -stick.Y;
            DirectionOverride = true;
            DirectionOverrideVector.Z = 0.0f;
            DirectionOverrideVector.Normalise();
            float angle = ParentThing->GetCurrentRelativeMovementAngleXY();
            DirectionOverrideVector.RotateXY(angle);
        }
    }
}