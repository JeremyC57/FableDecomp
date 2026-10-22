#include <stdio.h>
#include <string.h>
#include <math.h>
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
struct CIntelligentPointerThingObject : CBaseIntelligentPointer {
    operator CThingObject*() const { return (CThingObject*)GetPItem(); }
};
struct Coin {                                   // 0x3c bytes
    long      _pad_0x00;
    C2DVector Pos;                              // +0x04
    C2DVector Velocity;                         // +0x0c
    float     _pad_0x14;
    float     Angle;                            // +0x18
    unsigned char _pad_0x1c[0x8];
    bool      Owned;                            // +0x24
    CIntelligentPointerThingObject Thing;       // +0x25
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
    PositionMesh(Held->Thing, BoardPos, BoardRHSet, C3DVector(Held->Pos.X, Held->Pos.Y, 0.0f), 0, 0.0f);
    AverageJoystickPos = C2DVector(0.0f, 0.0f);
    MousePosEffect = C2DVector(0.0f, 0.0f);
    ZAngle = (float)atan2(BoardRHSet.Up.X, BoardRHSet.Up.Y) * 0.15915494f + 0.5f;
}

// ---- stubs ----
static int g_calls[8]; static int g_order[16]; static int g_n;
static long g_course; static C3DVector g_added(0,0,0);
static CThing* g_piece = (CThing*)0x1234;
static void* g_meshPiece; static const C3DVector* g_meshPos; static const CRightHandedSet* g_meshRH;
static C3DVector g_meshOff(0,0,0); static long g_meshA = -1; static float g_meshAngle = -1.0f;
static double g_coinBuf[16];
#define g_coin (*(Coin*)g_coinBuf)
void CTCTavernGameCoinBase::ClearBoard() { g_order[g_n++] = 1; g_calls[0]++; Held = 0; }
void CTCTavernGameCoinBase::FindObstacles() { g_order[g_n++] = 2; g_calls[1]++; }
void CTCTavernGameCoinGolf::EnableCourse(long course) { g_order[g_n++] = 3; g_calls[2]++; g_course = course; }
void CTCTavernGameCoinBase::AddNewCoin(const C3DVector& position) { g_order[g_n++] = 4; g_calls[3]++; g_added = position; g_coin.Pos.X = position.X; g_coin.Pos.Y = position.Y; Held = &g_coin; }
CThing* CBaseIntelligentPointer::GetPItem() const { g_calls[4]++; return this == &g_coin.Thing ? g_piece : 0; }
void __fastcall PositionMesh(CThingObject* piece, const C3DVector& pos, const CRightHandedSet& rh, const C3DVector& offset, long a, float angle)
{ g_order[g_n++] = 5; g_calls[5]++; g_meshPiece = piece; g_meshPos = &pos; g_meshRH = &rh; g_meshOff = offset; g_meshA = a; g_meshAngle = angle; }

static int fails;
static void check(int ok, const char* what) { if (!ok) { char b[200]; _snprintf(b, sizeof b, "FAIL %s", what); puts(b); fails++; } }
static int near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    static double gBuf[64];
    CTCTavernGameCoinGolf& g = *(CTCTavernGameCoinGolf*)gBuf;
    memset(gBuf, 0, sizeof gBuf);
    memset(&g_coin, 0, sizeof g_coin);
    g.CurrentCourse = 3;
    g.BoardPos = C3DVector(10.0f, 20.0f, 30.0f);
    g.BoardRHSet.Up = C3DVector(1.0f, 1.0f, 0.0f);
    g.CoinStartPos = C2DVector(4.0f, 5.0f);
    g.AverageJoystickPos = C2DVector(7.0f, 8.0f);
    g.MousePosEffect = C2DVector(9.0f, 11.0f);
    g.ZAngle = 99.0f;
    g.SetupForNewRound();
    check(g_n == 5 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 3 && g_order[3] == 4 && g_order[4] == 5, "call order");
    check(g_course == 3, "course");
    check(near(g_added.X, 10.0f) && near(g_added.Y, 20.0f) && near(g_added.Z, 30.0f), "added at board pos");
    check(g.Held == &g_coin, "held");
    check(near(g_coin.Pos.X, 4.0f) && near(g_coin.Pos.Y, 5.0f), "coin moved to start");
    check(g_meshPiece == (void*)g_piece && g_calls[4] == 1, "mesh piece via Thing");
    check(g_meshPos == &g.BoardPos && g_meshRH == &g.BoardRHSet, "mesh pos/rh refs");
    check(near(g_meshOff.X, 4.0f) && near(g_meshOff.Y, 5.0f) && near(g_meshOff.Z, 0.0f), "mesh offset");
    check(g_meshA == 0 && near(g_meshAngle, 0.0f), "mesh trailing args");
    check(near(g.AverageJoystickPos.X, 0.0f) && near(g.AverageJoystickPos.Y, 0.0f), "joystick reset");
    check(near(g.MousePosEffect.X, 0.0f) && near(g.MousePosEffect.Y, 0.0f), "mouse reset");
    check(near(g.ZAngle, 0.625f), "zangle atan2(1,1)/(2pi)+0.5");
    // second round: different up vector (pointing down-negative)
    g_n = 0;
    g.BoardRHSet.Up = C3DVector(0.0f, -1.0f, 0.0f);
    g.CoinStartPos = C2DVector(-2.0f, 6.5f);
    g.CurrentCourse = 1;
    g.SetupForNewRound();
    check(g_course == 1 && near(g_meshOff.X, -2.0f) && near(g_meshOff.Y, 6.5f), "round 2 offset");
    check(near(g.ZAngle, 1.0f), "zangle atan2(0,-1)");
    if (fails) { puts("007d2ef0_TEST FAIL"); return 1; }
    puts("007d2ef0_TEST PASS");
    return 0;
}