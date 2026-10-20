// Behaviour test for 005ec990: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCTavernGame::DrawBox(const CEnginePrimitivePrepareInfo&) -- draws the main-game score box: nine box handles, the box placed and sized from the tavern-game def in render-target units, on layer 0x47
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
struct CEnginePrimitiveHandle;

struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C2DBoxF {
    C2DVector TopLeft;
    C2DVector BottomRight;
    C2DBoxF(float left, float top, float right, float bottom) : TopLeft(left, top), BottomRight(right, bottom) {}
};
struct C2DExtentsI {
    long X, Y;
    C2DExtentsI() {}
};
struct CDisplayManager {
    const C2DExtentsI GetRenderTargetDimensions();   // 0x009BEDC0
};
extern CDisplayManager* g_pDisplayDevice;      // 0x013B8390

struct CRetailHandleVector {                    // STLport vector<CEnginePrimitiveHandle> (12 bytes)
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    void Resize(long count);                    // 0x00443F90
};
struct CEnginePrimitivePrepareInfo;
enum EEngine2DLayer { ENGINE_2D_LAYER_TAVERN_SCORE_BOX = 0x47 };
void __fastcall DrawBox(CEnginePrimitivePrepareInfo* info, CRetailHandleVector* handles, const C2DBoxF& box,
                        EEngine2DLayer layer);   // 0x0048E0A2

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CTavernGameDef : CDefVirtual {
    long m_count;                               // +0x04
    unsigned char _pad_0x08[0x180];
    float MainGameScoreBoxX;                    // +0x188
    float MainGameScoreBoxY;                    // +0x18c
    float MainGameScoreBoxWidthXbox;            // +0x190
    float MainGameScoreBoxWidthPC;              // +0x194
    float MainGameScoreBoxHeight;               // +0x198
    void Release() { if (--m_count == 0) Destroy(); }
};
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->Release(); }
    CTavernGameDef* operator->() const { return Object; }
};
struct CDefinitionManager {
    CTavernGameDef* PeekDefFromGlobalIndex(long index);   // stand-in only
};
struct CDefClassBase {
    virtual long* GetSubDefIndex(const char* name);       // stand-in only (retail: slot 14, CCharString name)
    unsigned char       _pad_0x04[0x18];
    CDefinitionManager* m_pManager;             // +0x1c
    bool GetSubDef_005ed4a0(CDefPointer& out);  // 0x005ED4A0 (GetSubDef<CTavernGameDef>)
};

// Stand-in for the same-module GetSubDef<CTavernGameDef> body (0x005ED4A0, landed separately; not graded here): compiled
// above the caller without inlining so VC7.1 sees, as retail's whole-program build did, that the def pointer it fills
// does not escape.
bool CDefClassBase::GetSubDef_005ed4a0(CDefPointer& out)
{
    long* index = GetSubDefIndex("CTavernGameDef");
    if (index == 0)
        return false;
    CTavernGameDef* def = m_pManager->PeekDefFromGlobalIndex(*index);
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
    unsigned char _pad_0x08[0x48];
    CRetailHandleVector BoxHandles;             // +0x50
    void GetDef(CDefPointer* out) { ParentThing->PDef->GetSubDef_005ed4a0(*out); }   // out of line 0x005EA000
    void DrawBox(const CEnginePrimitivePrepareInfo& info);
};

void CTCTavernGame::DrawBox(const CEnginePrimitivePrepareInfo& info)
{
    CDefPointer def;
    GetDef(&def);
    BoxHandles.Resize(9);
    C2DExtentsI size = g_pDisplayDevice->GetRenderTargetDimensions();
    ::DrawBox(const_cast<CEnginePrimitivePrepareInfo*>(&info), &BoxHandles,
              C2DBoxF(def->MainGameScoreBoxX * (float)size.Y, def->MainGameScoreBoxY * (float)size.Y,
                      (def->MainGameScoreBoxX + def->MainGameScoreBoxWidthPC) * size.X,
                      (def->MainGameScoreBoxY + def->MainGameScoreBoxHeight) * (float)size.Y),
              ENGINE_2D_LAYER_TAVERN_SCORE_BOX);
}

// ---- stubs and driver ----
void CDefVirtual::v00() {}
static int g_destroyed;
void CDefVirtual::Destroy() { ++g_destroyed; }
static CTavernGameDef g_def;
static long g_index = 9;
long* CDefClassBase::GetSubDefIndex(const char* name) { return std::strcmp(name, "CTavernGameDef") == 0 ? &g_index : 0; }
CTavernGameDef* CDefinitionManager::PeekDefFromGlobalIndex(long index) { return index == 9 ? &g_def : 0; }
const C2DExtentsI CDisplayManager::GetRenderTargetDimensions() { C2DExtentsI e; e.X = 800; e.Y = 600; return e; }
static CDisplayManager g_display;
CDisplayManager* g_pDisplayDevice = &g_display;
static long g_resize = -1; static CRetailHandleVector* g_resized;
void CRetailHandleVector::Resize(long count) { g_resize = count; g_resized = this; }
static CEnginePrimitivePrepareInfo* g_info; static CRetailHandleVector* g_handles; static float g_box[4]; static long g_layer; static long g_countAtDraw;
void __fastcall DrawBox(CEnginePrimitivePrepareInfo* info, CRetailHandleVector* handles, const C2DBoxF& box, EEngine2DLayer layer)
{ g_info = info; g_handles = handles; g_box[0] = box.TopLeft.X; g_box[1] = box.TopLeft.Y; g_box[2] = box.BottomRight.X; g_box[3] = box.BottomRight.Y; g_layer = layer; g_countAtDraw = g_def.m_count; }
void CTCTavernGame::v00() {}

int main()
{
    static CDefinitionManager manager;
    static CDefClassBase thingDef;
    static CThing thing;
    thingDef.m_pManager = &manager;
    thing.PDef = &thingDef;
    static CTCTavernGame game;
    game.ParentThing = &thing;
    g_def.m_count = 1;
    g_def.MainGameScoreBoxX = 0.125f; g_def.MainGameScoreBoxY = 0.25f; g_def.MainGameScoreBoxWidthPC = 0.5f; g_def.MainGameScoreBoxHeight = 0.0625f;
    CEnginePrimitivePrepareInfo* info = (CEnginePrimitivePrepareInfo*)&manager;
    game.DrawBox(*info);
    if (g_resize != 9 || g_resized != &game.BoxHandles || g_info != info || g_handles != &game.BoxHandles || g_layer != 0x47)
        { std::printf("005ec990_TEST FAIL (calls)\n"); return 1; }
    if (g_box[0] != 0.125f * 600 || g_box[1] != 0.25f * 600 || g_box[2] != (0.125f + 0.5f) * 800 || g_box[3] != (0.25f + 0.0625f) * 600)
        { std::printf("005ec990_TEST FAIL (box)\n"); return 1; }
    if (g_countAtDraw != 2 || g_def.m_count != 1 || g_destroyed != 0) { std::printf("005ec990_TEST FAIL (def)\n"); return 1; }
    g_def.m_count = 0;
    game.DrawBox(*info);
    if (g_def.m_count != 0 || g_destroyed != 1) { std::printf("005ec990_TEST FAIL (release)\n"); return 1; }
    std::printf("005ec990_TEST PASS\n");
    return 0;
}