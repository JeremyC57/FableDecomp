// Behaviour test for 007d7fd0: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCTavernGameSpotTheAddition::EnterGame() -- tavern-game EnterGame, camera onto the def's camera point, timer reset, then the pointer piece is created from the game def and placed at the game position raised by the pointer height
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CRightHandedSet { float Up[3], Forward[3]; };

struct CThing;
struct CThingObject;

struct CCharString {
    void* m_data;
    ~CCharString();                             // 0x0099EAE0
};
struct CDefStringTable {
    CCharString GetString(long id);             // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CTavernGameDef : CDefVirtual, CRefCounted {
    unsigned char _pad_0x08[0x88];
    long CameraName;                            // +0x90 CDefString
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {                            // CDefPointer<const CTavernGameDef>
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CTavernGameDef* operator->() const { return Object; }
};

struct CTavernGameSpotTheAdditionDef : CDefVirtual {
    long m_count;                               // +0x04
    unsigned char _pad_0x08[0x28];
    float CoverWidth;                           // +0x30
    float CoverLength;                          // +0x34
    unsigned char _pad_0x38[0x2c];
    long  PointerModel;                         // +0x64
    float PointerHeightOffset;                  // +0x68
    unsigned char _pad_0x6c[0x10];
    float LengthMovementScale;                  // +0x7c
    float HeightMovementScale;                  // +0x80
    void Release() { if (--m_count == 0) Destroy(); }
};
struct CSpotTheAdditionDefPointer {             // CDefPointer<const CTavernGameSpotTheAdditionDef>
    CTavernGameSpotTheAdditionDef* Object;
    CSpotTheAdditionDefPointer() : Object(0) {}
    ~CSpotTheAdditionDefPointer() { if (Object) Object->Release(); }
    CTavernGameSpotTheAdditionDef* operator->() const { return Object; }
};
struct CDefinitionManager {
    CTavernGameSpotTheAdditionDef* PeekDefFromGlobalIndex(long index);   // stand-in only
};
struct CDefClassBase {
    virtual long* GetSubDefIndex(const char* name);       // stand-in only (retail: slot 14, CCharString name)
    unsigned char       _pad_0x04[0x18];
    CDefinitionManager* m_pManager;             // +0x1c
    bool GetSubDef_007d9a80(CSpotTheAdditionDefPointer& out);   // 0x007D9A80 (GetSubDef<CTavernGameSpotTheAdditionDef>)
};

// Stand-in for the same-module GetSubDef<CTavernGameSpotTheAdditionDef> body (0x007D9A80; not graded here): compiled
// above the caller without inlining so VC7.1 sees, as retail's whole-program build did, that the def pointer it fills
// does not escape.
bool CDefClassBase::GetSubDef_007d9a80(CSpotTheAdditionDefPointer& out)
{
    long* index = GetSubDefIndex("CTavernGameSpotTheAdditionDef");
    if (index == 0)
        return false;
    CTavernGameSpotTheAdditionDef* def = m_pManager->PeekDefFromGlobalIndex(*index);
    if (out.Object != def) {
        if (out.Object)
            out.Object->Release();
        out.Object = def;
        if (def)
            ++def->m_count;
    }
    return true;
}

struct CThing {
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* PDef;                        // +0x70
};

struct CTCTavernGame {
    virtual void v00();
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0xe8];
    void EnterGame();                           // 0x005EC0F0
    void GetTavernGameDef(CDefPointer* out);    // 0x005EA000
    void CameraUseCameraPoint(const CCharString& name);   // 0x005EBFA0
    CThingObject* CreateGamePiece(long model);  // 0x005EA9B0
    static void __fastcall PositionMesh(CThing* thing, const C3DVector& position, const CRightHandedSet& orientation,
                                        const C3DVector& offset, float a, float b);   // 0x005EABC0
};
struct CThingObject : CThing { };

struct CTCTavernGameSpotTheAddition : CTCTavernGame {
    long          CurrentRound;                 // +0xf0
    float         CurrentTime;                  // +0xf4
    CThing*       Cover;                        // +0xf8
    float         CoverTimer;                   // +0xfc
    bool          UpdatedKnickKnacks;           // +0x100
    CThingObject* Pointer;                      // +0x104
    float         PointerScale;                 // +0x108
    float         DistFromObject;               // +0x10c
    unsigned char KnickKnacks[0xc];             // +0x110 vector<KnickKnack>
    C3DVector     BoardPos;                     // +0x11c
    CRightHandedSet BoardRHSet;                 // +0x128
    long          HeldTime;                     // +0x140
    long          Selected;                     // +0x144
    CThingObject* NewKnickKnack;                // +0x148
    bool          FirstRound;                   // +0x14c
    CRightHandedSet GamePositionRHSet;          // +0x14d
    C3DVector     GamePositionPos;              // +0x165
    void GetDef(CSpotTheAdditionDefPointer* out) { ParentThing->PDef->GetSubDef_007d9a80(*out); }
    void EnterGame();
};

void CTCTavernGameSpotTheAddition::EnterGame()
{
    CTCTavernGame::EnterGame();
    CDefPointer gameDef;
    GetTavernGameDef(&gameDef);
    {
        CCharString camera = g_DefStringTable.GetString(gameDef->CameraName);
        CameraUseCameraPoint(camera);
    }
    CSpotTheAdditionDefPointer def;
    GetDef(&def);
    CurrentTime = 0;
    Pointer = CreateGamePiece(def->PointerModel);
    PositionMesh(Pointer, GamePositionPos, GamePositionRHSet, C3DVector(0.0f, 0.0f, def->PointerHeightOffset), 0.0f, 0.0f);
}

// ---- stubs and driver ----
void CDefVirtual::v00() {}
static int g_destroyed;
void CDefVirtual::Destroy() { ++g_destroyed; }
static CTavernGameSpotTheAdditionDef g_def;
static CTavernGameDef g_gameDef;
static long g_index = 4;
long* CDefClassBase::GetSubDefIndex(const char* name) { return std::strcmp(name, "CTavernGameSpotTheAdditionDef") == 0 ? &g_index : 0; }
CTavernGameSpotTheAdditionDef* CDefinitionManager::PeekDefFromGlobalIndex(long index) { return index == 4 ? &g_def : 0; }
void CTCTavernGame::v00() {}
static char g_log[16]; static int g_n;
static void Log(char c) { if (g_n < 15) g_log[g_n++] = c; g_log[g_n] = 0; }
void CTCTavernGame::EnterGame() { Log('E'); }
void CTCTavernGame::GetTavernGameDef(CDefPointer* out) { Log('G'); out->Object = &g_gameDef; ++g_gameDef.m_count; }
CCharString::~CCharString() {}
CDefStringTable g_DefStringTable;
static long g_nameId = -1;
CCharString CDefStringTable::GetString(long id) { g_nameId = id; CCharString s; s.m_data = (void*)"CAM"; return s; }
static const char* g_camera;
void CTCTavernGame::CameraUseCameraPoint(const CCharString& name) { Log('C'); g_camera = (const char*)name.m_data; }
static CThingObject g_piece; static long g_model = -1;
CThingObject* CTCTavernGame::CreateGamePiece(long model) { Log('P'); g_model = model; return &g_piece; }
static CThing* g_thing; static const C3DVector* g_pos; static const CRightHandedSet* g_rh; static float g_off[3], g_a = 1, g_b = 1;
void __fastcall CTCTavernGame::PositionMesh(CThing* thing, const C3DVector& position, const CRightHandedSet& orientation, const C3DVector& offset, float a, float b)
{ Log('M'); g_thing = thing; g_pos = &position; g_rh = &orientation; g_off[0] = offset.X; g_off[1] = offset.Y; g_off[2] = offset.Z; g_a = a; g_b = b; }

int main()
{
    static CDefinitionManager manager;
    static CDefClassBase thingDef;
    static CThing parent;
    thingDef.m_pManager = &manager; parent.PDef = &thingDef;
    static CTCTavernGameSpotTheAddition game;
    game.ParentThing = &parent; game.CurrentTime = 9.0f;
    g_def.m_count = 1; g_def.PointerModel = 77; g_def.PointerHeightOffset = 0.25f;
    g_gameDef.m_count = 0; g_gameDef.CameraName = 12;
    game.EnterGame();
    if (std::strcmp(g_log, "EGCPM") != 0 || g_nameId != 12 || std::strcmp(g_camera, "CAM") != 0 || g_model != 77
        || game.Pointer != &g_piece || game.CurrentTime != 0.0f) { std::printf("007d7fd0_TEST FAIL (%s)\n", g_log); return 1; }
    if (g_thing != &g_piece || g_pos != &game.GamePositionPos || g_rh != &game.GamePositionRHSet || g_off[0] != 0.0f || g_off[1] != 0.0f
        || g_off[2] != 0.25f || g_a != 0.0f || g_b != 0.0f) { std::printf("007d7fd0_TEST FAIL (mesh)\n"); return 1; }
    if (g_def.m_count != 1 || g_gameDef.m_count != 0 || g_destroyed != 1) { std::printf("007d7fd0_TEST FAIL (release)\n"); return 1; }
    std::printf("007d7fd0_TEST PASS\n");
    return 0;
}
