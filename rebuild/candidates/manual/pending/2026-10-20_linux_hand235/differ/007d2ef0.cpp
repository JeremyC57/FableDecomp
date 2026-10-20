// CTCTavernGameCoinGolf::SetupForNewRound() -- clears the board, finds the obstacles, enables the current course, adds a held coin at the course start, places its mesh, resets the joystick average and mouse effect and turns the camera to face along the board
// Grade: DIFFER(ours v retail) -- 242v242: scheduling/registers only: retail computes `lea ecx,[eax+25h]` before the RHSet push (so the RHSet address goes through eax) and the zero-copy temporaries rotate ecx/edx/eax/ecx (ours edx/eax/ecx/edx)
// Leaf: SetupForNewRound
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D2EF0, 242 bytes (plain `ret`; Ego_r.pdb
// `?SetupForNewRound@CTCTavernGameCoinGolf@@AAEXXZ`).
// Callees: CTCTavernGameCoinBase::ClearBoard() 0x008EF950; CTCTavernGameCoinBase::FindObstacles() 0x008EFB00;
//          CTCTavernGameCoinGolf::EnableCourse(long) 0x007D2CC0; CTCTavernGameCoinBase::AddNewCoin(const C3DVector&)
//          0x008EFA00; CBaseIntelligentPointer::GetPItem() 0x00A01B50; PositionMesh(CThingObject*, const C3DVector&, const
//          CRightHandedSet&, const C3DVector&, long, float) 0x005EABC0 (__fastcall global); atan2 as the `fpatan` intrinsic.
// Constants: 1/(2*pi) 0x01237F08, 0.5f 0x0122F59C.
// Layout (retail): CTCTavernGameCoinBase Held +0x108, BoardPos +0x10c, BoardRHSet +0x118 (two C3DVectors), MousePosEffect
//          +0x13c (PDB +0x158); CTCTavernGameCoinGolf ZAngle +0x148, AverageJoystickPos +0x160, CoinStartPos +0x168,
//          CurrentCourse +0x17c (PDB - 0x18); Coin (packed) Pos +0x04, Thing +0x25.
// Shape: the coin goes on at BoardPos and is then moved to the start position (dword copies); the mesh offset is a
//        C3DVector temporary; the two C2DVector(0, 0) resets are temporaries through a user ctor, copied member-wise;
//        ZAngle = atan2(Up.X, Up.Y) / (2*pi) + 0.5 with the atan2 operands loaded first.
class CThing;
struct CThingObject;
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CRightHandedSet { C3DVector Forward; C3DVector Up; };
#include <math.h>
#pragma intrinsic(atan2)

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    void* _vfptr;                               // +0x00
    void* m_pItem;                              // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct Coin {                                   // 0x3c bytes
    long      _pad_0x00;
    C2DVector Pos;                              // +0x04
    C2DVector Velocity;                         // +0x0c
    float     _pad_0x14;
    float     Angle;                            // +0x18
    unsigned char _pad_0x1c[0x8];
    bool      Owned;                            // +0x24
    CBaseIntelligentPointer Thing;              // +0x25
    unsigned char _pad_0x2d[0xf];
};
#pragma pack(pop)

void __fastcall PositionMesh(CThingObject* piece, const C3DVector& pos, const CRightHandedSet& rh,
                             const C3DVector& offset, long a, float angle);   // 0x005EABC0

struct CTCTavernGameCoinBase {
    unsigned char   _pad_0x00[0x108];
    Coin*           Held;                       // +0x108
    C3DVector       BoardPos;                   // +0x10c
    CRightHandedSet BoardRHSet;                 // +0x118
    C3DVector       BoardSize;                  // +0x130
    C2DVector       MousePosEffect;             // +0x13c
    unsigned char   _pad_0x144[4];
    void ClearBoard();                          // 0x008EF950
    void FindObstacles();                       // 0x008EFB00
    void AddNewCoin(const C3DVector& position); // 0x008EFA00
};
struct CTCTavernGameCoinGolf : CTCTavernGameCoinBase {
    float         ZAngle;                       // +0x148
    float         XAngle;                       // +0x14c
    float         CameraDistance;               // +0x150
    C3DVector     CameraFocus;                  // +0x154
    C2DVector     AverageJoystickPos;           // +0x160
    C2DVector     CoinStartPos;                 // +0x168
    C2DVector     GoalPos;                      // +0x170
    float         GoalRadius;                   // +0x178
    long          CurrentCourse;                // +0x17c
    void EnableCourse(long course);             // 0x007D2CC0
    void SetupForNewRound();
};

void CTCTavernGameCoinGolf::SetupForNewRound()
{
    ClearBoard();
    FindObstacles();
    EnableCourse(CurrentCourse);
    AddNewCoin(BoardPos);
    Held->Pos = CoinStartPos;
    PositionMesh((CThingObject*)Held->Thing.GetPItem(), BoardPos, BoardRHSet, C3DVector(Held->Pos.X, Held->Pos.Y, 0.0f), 0, 0.0f);
    AverageJoystickPos = C2DVector(0.0f, 0.0f);
    MousePosEffect = C2DVector(0.0f, 0.0f);
    ZAngle = (float)atan2(BoardRHSet.Up.X, BoardRHSet.Up.Y) * 0.15915494f + 0.5f;
}
