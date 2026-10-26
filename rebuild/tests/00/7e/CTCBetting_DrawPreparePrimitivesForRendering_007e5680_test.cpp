// Behaviour test for 007e5680: src/007e5680.cpp copied verbatim; the display, the GUI def, the betting def lookup and
// DrawStatBoxWithText stubbed so the gate (input enabled + sub-def found), both boxes' screen positions, widths, texts
// and money flags, and the def release are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCBetting::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- while betting input is on, draw the bet box and the player's money box at their def positions (screen fractions of the render target)
// Retry 10b (getsw sweep, FIXES B-15): the base def is read through an inline getter `GetBaseDef()`; the plain member read gives the parked register/scheduling difference.
// Leaf: DrawPreparePrimitivesForRendering
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E5680, 320 bytes, `ret 4`.
// Callees: CDefClassBase::GetSubDef<CBettingDef> 0x007E5820, CDisplayManager::GetRenderTargetDimensions() 0x009BEDC0 on
//   g_pDisplayDevice 0x013B8390 (C2DExtentsI by hidden pointer, twice per position), CTCBetting::GetBetText 0x007E50C0
//   (descriptive; manifest "CTCSinglePersonOccupiable::IsOfType" is propagated), CTCBetting::GetPlayersMoney 0x007E5570,
//   PeekGuiDef 0x00438570 (__fastcall), DrawStatBoxWithText(const CEnginePrimitivePrepareInfo&, vector<
//   CEnginePrimitiveHandle>&, const C2DVector&, long, const CWideString&, long, unsigned char, bool) 0x00646A46
//   (__fastcall), ~CWideString 0x0099B510; the def's virtual Destroy (slot 1) on release.
// Globals: g_bBettingInputEnabled 0x013BAE1D.
// Layout = Ego_r.pdb CTCBetting with 12-byte retail vectors: BetHandles +0x10, MoneyHandles +0x1C; the betting def's bet
//   box position +0x28, money box position +0x30 (descriptive); the GUI def's stat-box width +0x2E8 (descriptive).
// Shape: the def pointer is a zeroed local released by its inline destructor (the null test folds on the path that read
//   it); each box is one DrawStatBoxWithText call with a C2DVector temporary built from `def x screen size` (Y first:
//   arguments evaluate right to left) and the text a by-value CWideString temporary. The def object is read once into a
//   named local; a same-TU stand-in GetSubDef body lets VC7.1 keep it in esi and reuse the pointer's frame word (the
//   retail 0x20 frame).
// Tried: the def through operator-> at each use (0x24 frame, def copied to two registers), the named def local alone
//   (0x24 frame), an out pointer plus a releasing holder (0x24 frame); the base def through the inline GetBaseDef()
//   accessor fixes the last register rotation.
#include <stddef.h>
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class C2DExtentsI { public:
    long X, Y;
    C2DExtentsI() {}
};
class CDisplayManager { public:
    const C2DExtentsI GetRenderTargetDimensions();   // 0x009BEDC0
};
extern CDisplayManager* g_pDisplayDevice;      // 0x013B8390
extern bool g_bBettingInputEnabled;             // 0x013BAE1D

class CWideString { public:
    void* PStringData;
    ~CWideString();                             // 0x0099B510
};
class CEnginePrimitiveHandle { public: void* _vtable; void* Data; };
struct CHandleVector {                          // STLport vector<CEnginePrimitiveHandle> (12 bytes)
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
};
class CEnginePrimitivePrepareInfo;
void __fastcall DrawStatBoxWithText(const CEnginePrimitivePrepareInfo& info, CHandleVector& handles, const C2DVector& pos,
                                    long width, const CWideString& text, long value, unsigned char alpha, bool money);   // 0x00646A46
class CPlayerGuiDef { public:
    unsigned char _pad00[0x2e8];
    long          StatBoxWidth;                 // +0x2e8
};
CPlayerGuiDef* __fastcall PeekGuiDef();         // 0x00438570

class CBettingDef { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long          RefCount;                     // +0x04
    unsigned char _pad08[0x28 - 0x08];
    float         BetBoxX;                      // +0x28
    float         BetBoxY;                      // +0x2c
    float         MoneyBoxX;                    // +0x30
    float         MoneyBoxY;                    // +0x34
    __forceinline void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CBettingDef>* out);   // 0x007E5820
};
CBettingDef* FindRef_GetSubDef_0();             // stand-in callee of the stand-in body below
// Same-TU stand-in for GetSubDef (the real one is out of line in retail; LTCG knew it): it only writes the out pointer,
// so VC7.1 knows the def pointer's address does not escape the call (the 0x007E4B60 / 0x00702B10 precedent).
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<CBettingDef>* out)
{
    CBettingDef* def = FindRef_GetSubDef_0();
    if (!def)
        return false;
    out->Object = def;
    return true;
}
#pragma auto_inline(on)
struct CThingWithDef {
    unsigned char  _pad00[0x70];
    CDefClassBase* PBaseDef;                    // +0x70
    CDefClassBase* GetBaseDef() const { return PBaseDef; }
};

class CTCBetting {
public:
    void*          __vftable;                   // +0x00 (CTCBase)
    CThingWithDef* ParentThing;                 // +0x04
    unsigned char  _pad08[0x10 - 0x08];
    CHandleVector  BetHandles;                  // +0x10
    CHandleVector  MoneyHandles;                // +0x1c
    CWideString GetBetText();                   // 0x007E50C0
    CWideString GetPlayersMoney();              // 0x007E5570
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

void CTCBetting::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    CDefPointer<CBettingDef> def;
    if (g_bBettingInputEnabled) {
        if (ParentThing->GetBaseDef()->GetSubDef(&def)) {
            CBettingDef* bettingDef = def.Object;
            DrawStatBoxWithText(info, BetHandles,
                                C2DVector(bettingDef->BetBoxX * g_pDisplayDevice->GetRenderTargetDimensions().X,
                                          bettingDef->BetBoxY * g_pDisplayDevice->GetRenderTargetDimensions().Y),
                                PeekGuiDef()->StatBoxWidth, GetBetText(), 0, 0xff, false);
            DrawStatBoxWithText(info, MoneyHandles,
                                C2DVector(bettingDef->MoneyBoxX * g_pDisplayDevice->GetRenderTargetDimensions().X,
                                          bettingDef->MoneyBoxY * g_pDisplayDevice->GetRenderTargetDimensions().Y),
                                PeekGuiDef()->StatBoxWidth, GetPlayersMoney(), 0, 0xff, true);
        }
    }
}
// ---- end source ----
bool g_bBettingInputEnabled = false;
static CDisplayManager g_display; CDisplayManager* g_pDisplayDevice = &g_display;
const C2DExtentsI CDisplayManager::GetRenderTargetDimensions() { C2DExtentsI e; e.X = 800; e.Y = 600; return e; }
static int g_wsDtors = 0;
CWideString::~CWideString() { ++g_wsDtors; }
static CPlayerGuiDef g_gui;
CPlayerGuiDef* __fastcall PeekGuiDef() { return &g_gui; }
static int g_destroys = 0;
void CBettingDef::Destroy() { ++g_destroys; }
void CBettingDef::v00() {}
static CBettingDef g_def; static bool g_found = true;
CBettingDef* FindRef_GetSubDef_0() { if (!g_found) return 0; ++g_def.RefCount; return &g_def; }
struct SBox { const CHandleVector* handles; float x, y; long width; void* text; long value; unsigned char alpha; bool money; };
static SBox g_boxes[4]; static int g_nBoxes = 0;
void __fastcall DrawStatBoxWithText(const CEnginePrimitivePrepareInfo&, CHandleVector& handles, const C2DVector& pos,
                                    long width, const CWideString& text, long value, unsigned char alpha, bool money)
{ SBox b = { &handles, pos.X, pos.Y, width, text.PStringData, value, alpha, money }; g_boxes[g_nBoxes++] = b; }
CWideString CTCBetting::GetBetText() { CWideString s; s.PStringData = (void*)"bet"; return s; }
CWideString CTCBetting::GetPlayersMoney() { CWideString s; s.PStringData = (void*)"money"; return s; }
int main()
{
    g_gui.StatBoxWidth = 120;
    g_def.RefCount = 1; g_def.BetBoxX = 0.5f; g_def.BetBoxY = 0.25f; g_def.MoneyBoxX = 0.1f; g_def.MoneyBoxY = 0.5f;
    static CDefClassBase base; static CThingWithDef thing; thing.PBaseDef = &base;
    static unsigned char raw[sizeof(CTCBetting)]; memset(raw, 0, sizeof(raw));
    CTCBetting* b = (CTCBetting*)raw; b->ParentThing = &thing;
    const CEnginePrimitivePrepareInfo& info = *(const CEnginePrimitivePrepareInfo*)&g_gui;
    b->DrawPreparePrimitivesForRendering(info); CHECK(g_nBoxes == 0);           // input disabled
    g_bBettingInputEnabled = true;
    b->DrawPreparePrimitivesForRendering(info);
    CHECK(g_nBoxes == 2 && g_boxes[0].handles == &b->BetHandles && g_boxes[0].x == 400.0f && g_boxes[0].y == 150.0f);
    CHECK(g_boxes[0].width == 120 && g_boxes[0].value == 0 && g_boxes[0].alpha == 0xff && !g_boxes[0].money);
    CHECK(g_boxes[1].handles == &b->MoneyHandles && g_boxes[1].x == 80.0f && g_boxes[1].y == 300.0f && g_boxes[1].money);
    CHECK(g_def.RefCount == 1 && g_destroys == 0 && g_wsDtors >= 2);
    g_found = false;
    b->DrawPreparePrimitivesForRendering(info); CHECK(g_nBoxes == 2);
    if (fails == 0) printf("007e5680_TEST PASS\n"); else printf("007e5680_TEST FAIL\n");
    return fails;
}