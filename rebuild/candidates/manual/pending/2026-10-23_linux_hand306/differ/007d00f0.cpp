// Grade: DIFFER(1016v1016) -- size/flow/calls match; frame 0x28 v 0x2c: retail places the aim vector at F+4 and the wrapped angle at F+0x10 (ours angle F+4, vector F+8), guide/new-coin temporaries 4 higher
// CTCTavernGameShoveHaPenny::FrameUpdate() -- per frame while the game is active: show the aim line (while aiming, along the held coin's velocity) or the two coin guides (while placing), else hide them; while the coins are moving step the physics five times, rescore (with a sound on change) and once at rest either end the game (win/lose sound and text) or put a fresh coin on the board; then play the end sounds and refresh the meshes
// Leaf: ?FrameUpdate@CTCTavernGameShoveHaPenny
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D00F0, 1016 bytes (+ int3 padding), plain `ret`.
// Callees: CTCTavernGame::FrameUpdate 0x005EB9E0; CDefClassBase::GetSubDef<CTavernGameShoveHaPennyDef> 0x007D1260 (this TU) /
//   GetSubDef<CTavernGameDef> 0x005ED4A0 (manifest label "GetSubDef<CAICreatureWillPowerIndicatorDef>" is propagated);
//   HideMesh 0x005E9B10 / PositionMesh 0x005EABC0 (`__fastcall`); _CIfmod 0x00BFEB28; CTCTavernGameCoinBase::UpdateCoins
//   0x008EF520 / UpdateCoinSounds 0x008EF910 / AtRest 0x008EF1B0 / AddNewCoin 0x008EFA00 / UpdateMeshes 0x008EF2F0;
//   CalcScores 0x007CF860; CTCBetting::PlaySound 0x005EA950 / GetText 0x005E9E70; CTCTavernGame::SetStateAfterOK
//   0x005EB080 / SetState 0x005E9B20; __ftol2 0x00BFEA70; CCharString 0x0099EBF0 / ~ 0x0099EAE0; frame rate 0x01375550.
// Layout (retail): Active +0x20, Won +0x21, Score +0x2c, TargetScore +0x9c, GameMasterState +0xa8, State +0xc0, meshes
//   +0xf0/+0xf4/+0xf8, Held +0x108, BoardPos +0x10c, BoardRHSet +0x118, BoardSize +0x130, +0x13c / +0x14c 2D vectors,
//   CoinsPlayed +0x148 (descriptive names); Coin Pos +0x04, Velocity +0x0c.
#include <math.h>
class CThing;
struct CThingObject;
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CRightHandedSet { C3DVector Up, Forward; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
extern long g_FrameRate;                        // 0x01375550

#pragma pack(push, 1)
struct Coin {                                   // 0x3c bytes
    long      _pad00;
    C2DVector Pos;                              // +0x04
    C2DVector Velocity;                         // +0x0c
    unsigned char _pad14[0x3c - 0x14];
};
#pragma pack(pop)

void __fastcall HideMesh(CThingObject* piece);  // 0x005E9B10
void __fastcall PositionMesh(CThingObject* piece, const C3DVector& pos, const CRightHandedSet& rh,
                             const C3DVector& offset, long a, float angle);   // 0x005EABC0

struct CTavernGameShoveHaPennyDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
    unsigned char _pad08[0x1c8 - 0x08];
    float NewCoinY;                             // +0x1c8
    unsigned char _pad1cc[0x1d0 - 0x1cc];
    float AimLineScale;                         // +0x1d0
    unsigned char _pad1d4[0x1f8 - 0x1d4];
    float AtRestSpeed;                          // +0x1f8
    long CoinsPerGame;                          // +0x1fc
};
struct CShoveHaPennyDefPointer {
    CTavernGameShoveHaPennyDef* Object;
    CShoveHaPennyDefPointer() : Object(0) { }
    ~CShoveHaPennyDefPointer()
    {
        if (Object) {
            if (--Object->m_count == 0)
                Object->Destroy();
        }
    }
    CTavernGameShoveHaPennyDef* operator->() const { return Object; }
};
struct CTavernGameDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
    unsigned char _pad08[0xa4 - 0x08];
    long WinTitle;                              // +0xa4
    long LoseTitle;                             // +0xa8
    unsigned char _padac[0xf8 - 0xac];
    long EndText;                               // +0xf8
};
struct CTavernGameDefPointer {
    CTavernGameDef* Object;
    CTavernGameDefPointer() : Object(0) { }
    ~CTavernGameDefPointer()
    {
        if (Object) {
            if (--Object->m_count == 0)
                Object->Destroy();
        }
    }
    CTavernGameDef* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CShoveHaPennyDefPointer* out);   // 0x007D1260
    bool GetSubDef(CTavernGameDefPointer* out);     // 0x005ED4A0
};
CTavernGameShoveHaPennyDef* FindRef_GetSubDef(CDefClassBase* defClass);
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CShoveHaPennyDefPointer* out)
{
    CTavernGameShoveHaPennyDef* def = FindRef_GetSubDef(this);
    if (!def)
        return false;
    out->Object = def;
    return true;
}
CTavernGameDef* FindRef_GetGameDef(CDefClassBase* defClass);
bool CDefClassBase::GetSubDef(CTavernGameDefPointer* out)
{
    CTavernGameDef* def = FindRef_GetGameDef(this);
    if (!def)
        return false;
    out->Object = def;
    return true;
}
#pragma auto_inline(on)
struct CThingDefOwner {
    unsigned char _pad00[0x70];
    CDefClassBase* DefClass;                    // +0x70
};

struct CTCBetting {
    virtual void v00();
    CThingDefOwner* ParentThing;                // +0x04
    void PlaySound(const CCharString& name);    // 0x005EA950
    CCharString GetText(long id);               // 0x005E9E70
};
struct CTCTavernGame : CTCBetting {
    void FrameUpdate();                         // 0x005EB9E0
    void SetStateAfterOK(CCharString title, CCharString text, long state);   // 0x005EB080
    void SetState(long state);                  // 0x005E9B20
};
struct CTCTavernGameCoinBase : CTCTavernGame {
    void UpdateCoins(float dt);                 // 0x008EF520
    void UpdateCoinSounds(bool final);          // 0x008EF910
    bool AtRest(float speed);                   // 0x008EF1B0
    void AddNewCoin(const C3DVector& position); // 0x008EFA00
    void UpdateMeshes();                        // 0x008EF2F0
};
struct CTCTavernGameShoveHaPenny : CTCTavernGameCoinBase {
    unsigned char _pad08[0x20 - 0x08];
    bool Active;                                // +0x20
    bool Won;                                   // +0x21
    unsigned char _pad22[0x2c - 0x22];
    float Score;                                // +0x2c
    unsigned char _pad30[0x9c - 0x30];
    long TargetScore;                           // +0x9c
    unsigned char _pada0[0xa8 - 0xa0];
    long GameMasterState;                       // +0xa8
    unsigned char _padac[0xc0 - 0xac];
    long State;                                 // +0xc0
    unsigned char _padc4[0xf0 - 0xc4];
    CThingObject* AimMesh;                      // +0xf0
    CThingObject* LeftGuideMesh;                // +0xf4
    CThingObject* RightGuideMesh;               // +0xf8
    unsigned char _padfc[0x108 - 0xfc];
    Coin* Held;                                 // +0x108
    C3DVector BoardPos;                         // +0x10c
    CRightHandedSet BoardRHSet;                 // +0x118
    C3DVector BoardSize;                        // +0x130
    C2DVector Flick;                            // +0x13c
    unsigned char _pad144[0x148 - 0x144];
    long CoinsPlayed;                           // +0x148
    C2DVector FlickStart;                       // +0x14c
    void CalcScores();                          // 0x007CF860
    void FrameUpdate();
};

void CTCTavernGameShoveHaPenny::FrameUpdate()
{
    CTCTavernGame::FrameUpdate();
    if (Active) {
    CShoveHaPennyDefPointer def;
    ParentThing->DefClass->GetSubDef(&def);
    switch (State) {
    case 16:
        if (sqrt(Held->Velocity.X * Held->Velocity.X + Held->Velocity.Y * Held->Velocity.Y) > 0.0f) {
            float angle = (float)atan2(Held->Velocity.X, Held->Velocity.Y) * 0.15915494f;
            if (angle < 0.0f || angle >= 1.0f) {
                angle = (float)fmod((double)angle, 1.0);
                if (angle < 0.0f)
                    angle += 1.0f;
            }
            PositionMesh(AimMesh, BoardPos, BoardRHSet,
                         C3DVector(Held->Velocity.X * def->AimLineScale + Held->Pos.X,
                                   Held->Velocity.Y * def->AimLineScale + Held->Pos.Y, 0.001f),
                         0, angle + 0.5f);
        } else
            HideMesh(AimMesh);
        HideMesh(LeftGuideMesh);
        HideMesh(RightGuideMesh);
        break;
    case 15:
        HideMesh(AimMesh);
        PositionMesh(LeftGuideMesh, BoardPos, BoardRHSet, C3DVector(Held->Pos.X - 0.05f, Held->Pos.Y, 0.03f), 0, 0.25f);
        PositionMesh(RightGuideMesh, BoardPos, BoardRHSet, C3DVector(Held->Pos.X + 0.05f, Held->Pos.Y, 0.03f), 0, 0.75f);
        break;
    default:
        HideMesh(AimMesh);
        HideMesh(LeftGuideMesh);
        HideMesh(RightGuideMesh);
        break;
    }
    if (State == 17) {
        float dt = 0.2f * (1.0f / g_FrameRate);
        for (long i = 0; i < 5; ++i)
            UpdateCoins(dt);
        UpdateCoinSounds(false);
        long oldScore = (long)Score;
        CalcScores();
        if (Score != oldScore)
            PlaySound("TAVERN_GAME_OBJECT_SELECTION_CHANGE");
        if (AtRest(def->AtRestSpeed)) {
            if (CoinsPlayed + 1 == def->CoinsPerGame) {
                Won = TargetScore <= Score;
                CTavernGameDefPointer gameDef;
                ParentThing->DefClass->GetSubDef(&gameDef);
                PlaySound(Won ? "TAVERN_GAME_WIN_GAME" : "TAVERN_GAME_LOSE_GAME");
                SetStateAfterOK(GetText(Won ? gameDef->WinTitle : gameDef->LoseTitle), GetText(gameDef->EndText), 6);
            } else {
                ++CoinsPlayed;
                AddNewCoin(C3DVector(BoardSize.X * 0.5f, def->NewCoinY, 0.0f));
                FlickStart = C2DVector(0.0f, 0.0f);
                Flick = C2DVector(0.0f, 0.0f);
                SetState(15);
            }
        }
    }
    if (State == 45 && GameMasterState == 11)
        UpdateCoinSounds(true);
    UpdateMeshes();
    }
}
