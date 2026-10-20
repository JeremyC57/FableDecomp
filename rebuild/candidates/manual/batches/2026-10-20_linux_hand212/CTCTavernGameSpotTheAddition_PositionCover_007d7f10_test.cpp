// Behaviour test for 007d7f10: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCTavernGameSpotTheAddition::PositionCover(float fraction) -- swings the cover over the board along an eased arc: the eased fraction picks the angle, the cover sits (1 - cos) along and sin up from its board position
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
#include <math.h>

const float TWO_PI = 6.2831855f;

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CRightHandedSet { float Up[3], Forward[3]; };
struct CThing;

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CTavernGameSpotTheAdditionDef : CDefVirtual {
    long m_count;                               // +0x04
    unsigned char _pad_0x08[0x28];
    float CoverWidth;                           // +0x30
    float CoverLength;                          // +0x34
    unsigned char _pad_0x38[0x44];
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
    bool GetSubDef_007d9a80(CSpotTheAdditionDefPointer& out);  // 0x007D9A80 (GetSubDef<CTavernGameSpotTheAdditionDef>)
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
    static void __fastcall PositionMesh(CThing* thing, const C3DVector& position, const CRightHandedSet& orientation,
                                        const C3DVector& offset, float a, float b);   // 0x005EABC0
};
struct CTCTavernGameSpotTheAddition : CTCTavernGame {
    long          CurrentRound;                 // +0xf0
    float         CurrentTime;                  // +0xf4
    CThing*       Cover;                        // +0xf8
    float         CoverTimer;                   // +0xfc
    bool          UpdatedKnickKnacks;           // +0x100
    void*         Pointer;                      // +0x104 CThingObject*
    float         PointerScale;                 // +0x108
    float         DistFromObject;               // +0x10c
    unsigned char KnickKnacks[0xc];             // +0x110 vector<KnickKnack>
    C3DVector     BoardPos;                     // +0x11c
    CRightHandedSet BoardRHSet;                 // +0x128
    void GetDef(CSpotTheAdditionDefPointer* out) { ParentThing->PDef->GetSubDef_007d9a80(*out); }
    void PositionCover(float fraction);
};

void CTCTavernGameSpotTheAddition::PositionCover(float fraction)
{
    if (Cover) {
        CSpotTheAdditionDefPointer def;
        GetDef(&def);
        float eased = (float)((1.0 - cos(fraction * 0.5f * (double)TWO_PI)) * 0.5);
        float angle = (float)(eased * 0.5f * (double)TWO_PI);
        float along = (float)((1.0 - cos(angle)) * def->LengthMovementScale * def->CoverLength);
        float up = (float)(sin(angle) * def->HeightMovementScale * def->CoverLength);
        PositionMesh(Cover, BoardPos, BoardRHSet, C3DVector(0.0f, along, up), 0.0f, 0.0f);
    }
}

// ---- stubs and driver ----
void CDefVirtual::v00() {}
static int g_destroyed;
void CDefVirtual::Destroy() { ++g_destroyed; }
static CTavernGameSpotTheAdditionDef g_def;
static long g_index = 4;
long* CDefClassBase::GetSubDefIndex(const char* name) { return std::strcmp(name, "CTavernGameSpotTheAdditionDef") == 0 ? &g_index : 0; }
CTavernGameSpotTheAdditionDef* CDefinitionManager::PeekDefFromGlobalIndex(long index) { return index == 4 ? &g_def : 0; }
void CTCTavernGame::v00() {}
static int g_calls; static CThing* g_thing; static const C3DVector* g_pos; static const CRightHandedSet* g_rh; static float g_off[3], g_a = 1, g_b = 1;
void __fastcall CTCTavernGame::PositionMesh(CThing* thing, const C3DVector& position, const CRightHandedSet& orientation, const C3DVector& offset, float a, float b)
{ ++g_calls; g_thing = thing; g_pos = &position; g_rh = &orientation; g_off[0] = offset.X; g_off[1] = offset.Y; g_off[2] = offset.Z; g_a = a; g_b = b; }
static bool Near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }

int main()
{
    static CDefinitionManager manager;
    static CDefClassBase thingDef;
    static CThing parent, cover;
    thingDef.m_pManager = &manager; parent.PDef = &thingDef;
    static CTCTavernGameSpotTheAddition game;
    game.ParentThing = &parent;
    g_def.m_count = 1; g_def.CoverLength = 2.0f; g_def.LengthMovementScale = 3.0f; g_def.HeightMovementScale = 5.0f;
    // no cover: nothing happens
    game.Cover = 0;
    game.PositionCover(0.5f);
    if (g_calls != 0 || g_def.m_count != 1) { std::printf("007d7f10_TEST FAIL (no cover)\n"); return 1; }
    game.Cover = &cover;
    // fraction 1: eased 1, angle pi: offset (0, 2*3*2, 0)
    game.PositionCover(1.0f);
    if (g_calls != 1 || g_thing != &cover || g_pos != &game.BoardPos || g_rh != &game.BoardRHSet || g_a != 0.0f || g_b != 0.0f
        || g_off[0] != 0.0f || !Near(g_off[1], 12.0f) || !Near(g_off[2], 0.0f)) { std::printf("007d7f10_TEST FAIL (1)\n"); return 1; }
    // fraction 0.5: eased 0.5, angle pi/2: offset (0, 1*3*2, 1*5*2)
    game.PositionCover(0.5f);
    if (g_off[0] != 0.0f || !Near(g_off[1], 6.0f) || !Near(g_off[2], 10.0f)) { std::printf("007d7f10_TEST FAIL (0.5)\n"); return 1; }
    // fraction 0: at rest
    game.PositionCover(0.0f);
    if (!Near(g_off[1], 0.0f) || !Near(g_off[2], 0.0f) || g_def.m_count != 1 || g_destroyed != 0) { std::printf("007d7f10_TEST FAIL (0)\n"); return 1; }
    std::printf("007d7f10_TEST PASS\n");
    return 0;
}
