// CTCCameraPointDefinitionBase::IsAngleValid() const -- for a point that tests its angle before activation: true only when the point's active camera position lies on the same side of the hero as the current game camera (positive 2D dot product)
// Grade: RELOCATION_MATCH -- the 2D dot product is accumulated `d = toActive.X * toCamera.X; d += toActive.Y * toCamera.Y;` (retail multiplies the X term first); the single expression multiplies Y first
// Leaf: IsAngleValid
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075C530, length 192 bytes
// (?IsAngleValid@CTCCameraPointDefinitionBase@@ABE_NXZ).
// Callees: CTCBase::PeekWorld() const @ 0x00686A80, CWorld::GetCurrentPlayer @ 0x0049D9D0, CPlayer::PeekPControlledCreature
// @ 0x00487DD0, the camera manager's (world +0x34) virtual slot 11 (+0x2C, C3DVector by hidden pointer), CThing::GetPos
// @ 0x004C73D0, the point's own virtual GetActiveCameraPosition (slot 35, +0x8C, C3DVector by hidden pointer); 0.0f @ 0x0122DEDC.
// Layout = Ego_r.pdb CTCCameraPointDefinitionBase: TestAngleBeforeActivation +0x0E.
// Shape: the camera direction from the hero is a 3D local with only x and y filled (its unused z keeps a frame word) built
// before the virtual call; the dot product of the two
// hero-relative directions is compared `> 0.0f`; the early exits return byte constants, the comparison an int-sized 1/0.
class C3DVector { public:
    float X, Y, Z;
};
class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
};
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    const C3DVector& GetPos() const;       // 0x004C73D0
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;            // 0x00487DD0
};
class CGameCameraManager { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual C3DVector PeekCurrentPos() const;      // slot 11 (+0x2C) 0x0069E1F0
};
class CWorld { public:
    unsigned char _pad00[0x34];
    CGameCameraManager* PCameraManager;         // +0x34
    CGameCameraManager* GetCameraManager() const { return PCameraManager; }
    CPlayer* GetCurrentPlayer() const;          // 0x0049D9D0
};
inline C2DVector DeltaXY(const C3DVector& a, const C3DVector& b)
{
    C2DVector d;
    d.X = a.X - b.X;
    d.Y = a.Y - b.Y;
    return d;
}
class CTCBase { public:
    virtual ~CTCBase();                                           // slot 0 (0x00686830)
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80 (Ego_r.pdb CTCBase, protected)
};
class CTCCameraPointDefinitionBase : public CTCBase {
public:
#define VF(n) virtual void v##n();
    VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15)
    VF(16) VF(17) VF(18) VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29) VF(30) VF(31)
    VF(32) VF(33) VF(34)
    virtual C3DVector GetActiveCameraPosition() const;   // slot 35 (+0x8C)
    void*   ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char _pad0a[2];
    bool    CutInto;                            // +0x0c
    bool    CutOutOf;                           // +0x0d
    bool    TestAngleBeforeActivation;          // +0x0e
    bool IsAngleValid() const;
};

bool CTCCameraPointDefinitionBase::IsAngleValid() const
{
    if (TestAngleBeforeActivation) {
        const CThing* hero = PeekWorld().GetCurrentPlayer()->PeekPControlledCreature();
        if (hero && !hero->IsDead()) {
            C3DVector camera = PeekWorld().GetCameraManager()->PeekCurrentPos();
            const C3DVector& heroPosition = hero->GetPos();
            C3DVector toCamera;
            toCamera.X = camera.X - heroPosition.X;
            toCamera.Y = camera.Y - heroPosition.Y;
            C2DVector toActive = DeltaXY(GetActiveCameraPosition(), heroPosition);
            float d = toActive.X * toCamera.X;
            d += toActive.Y * toCamera.Y;
            return d > 0.0f;
        }
        return false;
    }
    return true;
}