// CTCCrateStack::FillStack(long count, const C3DVector& position, const C3DVector& direction) -- create the bottom crate
// Grade: DIFFER(615v615) -- param-home slot reuse only: retail homes the crate in position's arg slot and the second twist in direction's; 46 diff lines
// at the position facing along the direction (with a small random twist); for a stack of two, create the top crate and
// lift it onto the bottom one; both are remembered through intelligent pointers
// Leaf: FillStack
//
// Retail 0x007B6810, 616 bytes (`ret 0xc`).
// Callees: CTCBase::GetPlayerManager const @ 0x00686D50, CPlayerManager::GetMainPlayerNumber @ 0x00449700, CCharString
// @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CreateThingObject @ 0x00703210 (__fastcall), CTCBase::GetPRandomSeed
// @ 0x00686DA0, GFFloatRandom @ 0x004978A0 (__fastcall), _CIfmod @ 0x00BFEB28, physics slot 66 (+0x108, set the facing
// angle) / slot 30 (+0x78, set position), CTCPhysicsBase::SetAsMoving @ 0x006B0130, CBaseIntelligentPointer ctor
// @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10 (CIntelligentPointer<CThing> vtable
// 0x01238C6C), CThing::GetPosition @ 0x004C73D0.
// Constants: 0.125f, 0.0625f @ 0x0122DC98, 1/(2pi) @ 0x01237F08, 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8, 1.0 (double)
// @ 0x01237F00, 0.5f @ 0x0122F59C; the crate height global @ 0x0138366C.
// Layout (local model): ParentThing +0x04, BottomCrate +0x0c, TopCrate +0x14 (CIntelligentPointer<CThing>), CrateDefIndex +0x1c.
#include <math.h>
#pragma intrinsic(atan2, fmod)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
#define VF(n) virtual void v##n();
struct CTCPhysicsBase {
    VF(0) VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18)
    VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29)
    virtual void SetPosition(const C3DVector& pos);          // slot 30 (+0x78)
    VF(31) VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39) VF(40) VF(41) VF(42) VF(43) VF(44) VF(45) VF(46) VF(47)
    VF(48) VF(49) VF(50) VF(51) VF(52) VF(53) VF(54) VF(55) VF(56) VF(57) VF(58) VF(59) VF(60) VF(61) VF(62) VF(63) VF(64)
    VF(65)
    virtual void SetFacingAngleXY(float angle);              // slot 66 (+0x108)
    void SetAsMoving(bool moving);                           // 0x006B0130
};
#undef VF
struct CThing {
    unsigned char   _pad0[0x60];
    CTCPhysicsBase* PhysicsTC;                               // +0x60
    const C3DVector& GetPosition() const;                    // 0x004C73D0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                                           // +0x04
    CBaseIntelligentPointer();                               // 0x00A01B10
    ~CBaseIntelligentPointer();                              // 0x00A01C10
    CThing* GetPItem() const;                                // 0x00A01B50
    void SetPItem(CThing* item);                             // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CRightHandedSet;
CThing* __fastcall CreateThingObject(long defIndex, const C3DVector* position, long player, const CRightHandedSet* set,
                                     unsigned long flags, const CCharString& name);   // 0x00703210
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
extern float g_crateHeight;                                  // 0x0138366C
struct CPlayerManager { long GetMainPlayerNumber() const; }; // 0x00449700
__forceinline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}
struct CTCCrateStack {
    virtual void v00();
    CThing* ParentThing;                                     // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CIntelligentPointerThing BottomCrate;                    // +0x0c
    CIntelligentPointerThing TopCrate;                       // +0x14
    long CrateDefIndex;                                      // +0x1c
    const CPlayerManager* GetPlayerManager() const;          // 0x00686D50
    unsigned long* GetPRandomSeed();                         // 0x00686DA0
    void FillStack(long count, const C3DVector& position, const C3DVector& direction);
};

void CTCCrateStack::FillStack(long count, const C3DVector& position, const C3DVector& direction)
{
    if (!count)
        return;
    long player = GetPlayerManager()->GetMainPlayerNumber();
    CThing* bottom = CreateThingObject(CrateDefIndex, &position, player, 0, 0, "");
    CTCPhysicsBase* physics = bottom->PhysicsTC;
    float twist = GFFloatRandom(0.125f, GetPRandomSeed()) - 0.0625f;
    physics->SetFacingAngleXY(WrapAngle((float)atan2(direction.X, direction.Y) * 0.15915494f) + twist + 0.5f);
    BottomCrate = CIntelligentPointerThing(bottom);
    if (count == 2) {
        CThing* top = CreateThingObject(CrateDefIndex, &ParentThing->GetPosition(), player, 0, 0, "");
        CTCPhysicsBase* topPhysics = top->PhysicsTC;
        topPhysics->SetPosition(C3DVector(position.X, position.Y, g_crateHeight + position.Z));
        float topTwist = GFFloatRandom(0.125f, GetPRandomSeed()) - 0.0625f;
        topPhysics->SetFacingAngleXY(WrapAngle((float)atan2(direction.X, direction.Y) * 0.15915494f) + topTwist + 0.5f);
        topPhysics->SetAsMoving(false);
        TopCrate = CIntelligentPointerThing(top);
    }
}
