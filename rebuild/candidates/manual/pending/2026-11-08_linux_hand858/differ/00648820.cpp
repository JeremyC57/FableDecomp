// NPlayerGui::CDrawMiniMap::Draw(const CEnginePrimitivePrepareInfo& info, bool forceDraw) -- skip while a live hero's region display covers an active map in letterbox mode; otherwise mark the drawer drawing, step the screen-position fade, size the centred map box (0.4 of the render width) and blend it toward the anchored ScreenBox, clamp the fade alpha, draw the map; on a region change draw the six-tile frame and the two side borders, otherwise label every visible region exit
// Grade: DIFFER(3419v3464) -- 648 diff lines: frame 0x324 vs 0x334 (every ebp slot shifted) and the two side borders; the six tiles match after the named tile_height temporary
// Next idea: the borders are FableWin ??H@YI?AVC2DBoxF@@ABVC2DVector@@ABVC2DExtentsF@@@Z (operator+(pos, extents), inline: d26 with float locals reproduces the x87 sequence) followed by a copy of the GFToScreenScaled result into the box (retail movsd); every copy spelling tried (d28-d32, d37-d40, d49-d52) flips this/0xff register allocation; also port the local C2DVector/C2DBoxF/CThing models to the shared headers
// Leaf: ?Draw@CDrawMiniMap@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00648820, 3464 bytes, `ret 8`, size-optimised NPlayerGui TU (`lea ebp,[esp-70h]` frame).
// Identity: FableWin.pdb / Ego_r.pdb ?Draw@CDrawMiniMap@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@_N@Z (public, void).
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::PeekPControlledCreature 0x00487DD0, IS_THING_ALIVE 0x0043DBF0,
//   std::bitset<274>::test 0x00410DE0 and the interface LowerBound 0x004365B0 (PeekTC, interface 0x8e), CTCRegionDisplay::IsDrawing
//   0x00639B70, CFadeInFadeOutBase Update 0x00434870 / GetFadeInOutAlpha 0x004348D0 / IsOpen 0x0048DA30, CDisplayEngine::GetMainWindow
//   0x00434B50, CDisplayManager::GetRenderTargetDimensions 0x009BEDC0, CPlayerGui::PeekGuiDef 0x00438570, NPlayerGui::GFAnchorToTopRight
//   0x00643CA1, C2DBoxF(pos, size) 0x0041A7E0, CDrawBase::ScaleAlpha 0x00643D5D, CMiniMapDisplay DrawPreparePrimitivesForEngine
//   0x0082C060 (a `jmp` to DrawMap) / IsThingAsPosBeingDrawn 0x00829D20 / GetDrawPosition 0x0082A020, CEnginePrimitive2DSprite
//   0x0041BEB0, CIEngine slot 23, NPlayerGui::GFToScreenScaled 0x00643BFC, NGameText::CDataBank::GetTextBySymbol 0x009C95E0,
//   CCharString::ToWideString 0x0099E480, CFontBank::GetStringWidth 0x0048DA62 (result unused), GFAXToVX / GFAYToVY, CPlayerGui::
//   Draw2DText 0x00438010.
// Layout: CDrawBase (retail): PlayerGui +4, PlayerManager +8, World +0xc, Component +0x10, Bank +0x14, Drawing +0x18,
//   PrimitiveHandles +0x1c; CDrawMiniMap: MiniMap +0x28, Active +0xe9, RegionChange +0xeb, Alpha +0xec, ScreenBox +0xf0, FadeInOut
//   +0x100, ScreenPosFadeInOut +0x138, RegionExits +0x154 (16-byte pair<C3DVector, CCharString>). CPlayerGuiDef (retail = Ego_r
//   - 0x94 here): MiniMapScreenOrigin +0x964, MiniMapScreenSize +0x96c, RegionChangeTLOffset +0x9b8, the six RegionChange tile
//   graphics +0x9e8..+0x9fc, RegionChangeBorderLeft/RightOffset +0xa00 / +0xa08, Border graphics +0xa10 / +0xa14.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include <string.h>
#include <wchar.h>
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"

class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF() {}
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) {}
    C2DBoxF(const C2DVector& top_left, const C2DVector& size);          // 0x0041A7E0
    float GetWidth() const { return BRX - TLX; }
    float GetHeight() const { return BRY - TLY; }
};
class C2DExtentsI { public: long Width, Height; };
class CDisplayManager { public: const C2DExtentsI GetRenderTargetDimensions(); };   // 0x009BEDC0
extern CDisplayManager* GPDisplayManager;                    // 0x013B8390
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
float __fastcall GFAXToVX(float x);                          // 0x009E1DA0
float __fastcall GFAYToVY(float y);                          // 0x009E1DD0

class CFadeInFadeOutBase { public:
    bool  Active;                                            // +0x00
    float FadeInTime;                                        // +0x04
    float FadeOutTime;                                       // +0x08
    bool  Closing;                                           // +0x0c
    bool  Opening;                                           // +0x0d
    float OpenTimer;                                         // +0x10
    float CloseTimer;                                        // +0x14
    unsigned char ToColour[4];                               // +0x18
    void Update(float elapsed);                              // 0x00434870
    float GetFadeInOutAlpha() const;                         // 0x004348D0
    bool IsActive() const { return Active; }
    bool IsOpen() const;                                     // 0x0048DA30
    bool IsOpening() const { return Opening; }
    bool IsClosing() const { return Closing; }
};

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;        // 0x004365B0
};
class CThing { public:
    unsigned char _pad0[0x20];
    mutable std::bitset<274> m_present;                      // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    template <class T> __forceinline bool PeekTC(long type, const T** ppOut) const
    {
        if (m_present.test(type)) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *ppOut = (const T*)((it == map->last || type < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBF0
bool IsThingAliveImpl(const void* thing);
#pragma auto_inline(off)
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && IsThingAliveImpl(thing);
}
#pragma auto_inline(on)
class CTCRegionDisplay { public:
    bool IsDrawing() const;                                  // 0x00639B70
};
class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
};
#include "engine/CPlayerManager.h"

class CDisplayEngine { public:
    unsigned char _pad00[0x70];
    bool          LetterBoxActive;                           // +0x70 (CLetterBoxModeInfo)
    unsigned char _pad71[0xbc - 0x71];
    CFadeInFadeOutBase ScreenFadeOutInfo;                    // +0xbc
    C2DBoxF GetMainWindow() const;                           // 0x00434B50
    bool IsLetterBoxMode() const { return LetterBoxActive; }
    bool IsScreenFadeOutClosing() const { return ScreenFadeOutInfo.IsClosing(); }
};
namespace NGameText { class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
}; }
class CFontBank { public:
    float GetStringWidth(const CWideString& text) const;     // 0x0048DA62
};
class CMainGameComponent { public:
    unsigned char _pad00[0x14];
    NGameText::CDataBank* PTextBank;                         // +0x14 (CCountedPointer object word)
    unsigned char _pad18[0x28 - 0x18];
    CDisplayEngine* PDisplayEngine;                          // +0x28 (scoped_ptr)
    unsigned char _pad2c[0x1614c - 0x2c];
    CFontBank* PCutSceneMainFont;                            // +0x1614C (CCountedPointer object word)
    const NGameText::CDataBank& GetTextBank() const { return *PTextBank; }
    const CFontBank& PeekCutSceneMainFont() const { return *PCutSceneMainFont; }
    CDisplayEngine& GetDisplayEngine() { return *PDisplayEngine; }
};
class CWorld { public:
    unsigned char _pad00[0xa4];
    bool Paused;                                             // +0xa4
};

class CGraphicDataBank { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual unsigned long GetGraphicWidth(long graphic) const;    // slot 23 (+0x5c)
    virtual unsigned long GetGraphicHeight(long graphic) const;   // slot 24 (+0x60)
};

class CPlayerGuiDef { public:
    unsigned char _pad000[0x964];
    C2DVector MiniMapScreenOrigin;                           // +0x964
    C2DVector MiniMapScreenSize;                             // +0x96c
    unsigned char _pad974[0x9b8 - 0x974];
    C2DVector RegionChangeTLOffset;                          // +0x9b8
    C2DVector RegionChangeTMOffset;                          // +0x9c0
    C2DVector RegionChangeTROffset;                          // +0x9c8
    C2DVector RegionChangeBLOffset;                          // +0x9d0
    C2DVector RegionChangeBMOffset;                          // +0x9d8
    C2DVector RegionChangeBROffset;                          // +0x9e0
    long RegionChangeTLGraphic;                              // +0x9e8
    long RegionChangeTMGraphic;                              // +0x9ec
    long RegionChangeTRGraphic;                              // +0x9f0
    long RegionChangeBLGraphic;                              // +0x9f4
    long RegionChangeBMGraphic;                              // +0x9f8
    long RegionChangeBRGraphic;                              // +0x9fc
    C2DVector RegionChangeBorderLeftOffset;                  // +0xa00
    C2DVector RegionChangeBorderRightOffset;                 // +0xa08
    long RegionChangeBorderLeftGraphic;                      // +0xa10
    long RegionChangeBorderRightGraphic;                     // +0xa14
};
enum EEngine2DLayer { };
class CPlayerGui { public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();     // 0x00438570
    void Draw2DText(const C2DVector& pos, const CWideString& text, const CFontBank& font, const CRGBColour& colour, float scale,
                    EEngine2DLayer layer, bool shadow);      // 0x00438010
};

class CEnginePrimitiveHandle { public: void* _vtable; void* m_pPrimitive; };
class CEnginePrimitive2DSprite { public:
    unsigned long Type;                                      // +0x00
    unsigned long InterpolationSet;                          // +0x04
    unsigned long ThemeChannelDesc;                          // +0x08
    C2DBoxF       ClipBox;                                   // +0x0c
    long          Layer;                                     // +0x1c
    unsigned long BankIndex;                                 // +0x20
    unsigned long Frame;                                     // +0x24
    float         StartAngle;                                // +0x28
    float         EndAngle;                                  // +0x2c
    long          BlendMode;                                 // +0x30
    unsigned char ColourB, ColourG, ColourR, ColourA;        // +0x34
    bool          ClipBoxIsPos;                              // +0x38
    unsigned long Flags;                                     // +0x3c
    void*         Texture;                                   // +0x40
    C2DVector     UVOffsetsStart;                            // +0x44
    C2DVector     UVOffsetsEnd;                              // +0x4c
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DBoxF& box, long layer, unsigned long graphic, unsigned long frame,
        long blend, long interpolation, float start_angle, float end_angle, unsigned char alpha, unsigned char red,
        unsigned char green, unsigned char blue, unsigned long flags, const C2DVector& uv0 = C2DVector(0.0f, 0.0f),
        const C2DVector& uv1 = C2DVector(0.0f, 0.0f));       // 0x0041BEB0
};
#pragma auto_inline(off)
inline CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64, const C2DBoxF& box, long layer, unsigned long graphic,
    unsigned long frame, long blend, long interpolation, float start_angle, float end_angle, unsigned char alpha, unsigned char red,
    unsigned char green, unsigned char blue, unsigned long flags, const C2DVector& uv0, const C2DVector& uv1)
    : Type(0x22), InterpolationSet(interpolation), ThemeChannelDesc(0), ClipBox(box), Layer(layer), BankIndex(graphic), Frame(frame),
      StartAngle(start_angle), EndAngle(end_angle), BlendMode(blend), ColourB(blue), ColourG(green), ColourR(red), ColourA(alpha),
      ClipBoxIsPos(false), Flags(flags), Texture(0), UVOffsetsStart(uv0), UVOffsetsEnd(uv1)
{
}
#pragma auto_inline(on)
class CIEngine { public:
    virtual void e00(); virtual void e01(); virtual void e02(); virtual void e03(); virtual void e04(); virtual void e05();
    virtual void e06(); virtual void e07(); virtual void e08(); virtual void e09(); virtual void e10(); virtual void e11();
    virtual void e12(); virtual void e13(); virtual void e14(); virtual void e15(); virtual void e16(); virtual void e17();
    virtual void e18(); virtual void e19(); virtual void e20(); virtual void e21(); virtual void e22();
    virtual void SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive2DSprite& primitive,
                                        unsigned long flags);   // slot 23 (+0x5C)
};
class CEnginePrimitivePrepareInfo { public:
    float GTPredictedTimeSinceLastRenderFrame;              // +0x00 (CInterpolationInfo Current)
    unsigned char _pad04[0x30];
    CIEngine* Engine;                                        // +0x34
};

class CMiniMapDisplay { public:
    void DrawPreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info, const C2DBoxF& box, unsigned char alpha,
                                        bool front);         // 0x0082C060
    bool IsThingAsPosBeingDrawn(const C3DVector& pos) const;   // 0x00829D20
    C2DVector GetDrawPosition(const C3DVector& pos, const C2DBoxF& box) const;   // 0x0082A020
};
struct CRegionExit {                                         // pair<C3DVector, CCharString>
    C3DVector   first;
    CCharString second;
};
struct CRegionExitVector {                                   // retail STLport vector (12 bytes)
    CRegionExit* _M_start;
    CRegionExit* _M_finish;
    CRegionExit* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const CRegionExit& operator[](unsigned long n) const { return _M_start[n]; }
};
struct CHandleVector {
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
    CEnginePrimitiveHandle& operator[](unsigned long n) { return _M_start[n]; }
};

namespace NPlayerGui {
C2DVector __fastcall GFAnchorToTopRight(const C2DVector& pos);   // 0x00643CA1
C2DBoxF __fastcall GFToScreenScaled(const C2DBoxF& box);    // 0x00643BFC

class CDrawBase { public:
    virtual void ClearPrimitives();
    virtual void ClearPrimitivesIfNecessary();
    unsigned char ScaleAlpha(unsigned char alpha) const;     // 0x00643D5D
    void SetAsDrawing(bool drawing) { Drawing = drawing; }
protected:
    CPlayerGui*         PlayerGui;                           // +0x04
    CPlayerManager*     PlayerManager;                       // +0x08
    CWorld*             World;                               // +0x0c
    CMainGameComponent* Component;                           // +0x10
    CGraphicDataBank*   Bank;                                // +0x14
    bool                Drawing;                             // +0x18
    CHandleVector       PrimitiveHandles;                    // +0x1c
};

class CDrawMiniMap : public CDrawBase { public:
    CMiniMapDisplay    MiniMap;                              // +0x28
    unsigned char      _pad29[0xe8 - 0x29];
    bool               Enabled;                              // +0xe8
    bool               Active;                               // +0xe9
    bool               OldActive;                            // +0xea
    bool               RegionChange;                         // +0xeb
    unsigned char      Alpha;                                // +0xec
    C2DBoxF            ScreenBox;                            // +0xf0
    CFadeInFadeOutBase FadeInOut;                            // +0x100
    CFadeInFadeOutBase TextFadeInOut;                        // +0x11c
    CFadeInFadeOutBase ScreenPosFadeInOut;                   // +0x138
    CRegionExitVector  RegionExits;                          // +0x154
    void Draw(const CEnginePrimitivePrepareInfo& info, bool forceDraw);
};

void CDrawMiniMap::Draw(const CEnginePrimitivePrepareInfo& info, bool forceDraw)
{
    if (Active && !forceDraw && Component->GetDisplayEngine().IsLetterBoxMode()) {
        const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
        if (IS_THING_ALIVE(hero)) {
            const CTCRegionDisplay* region_display;
            hero->PeekTC(0x8e, &region_display);
            if (!region_display->IsDrawing())
                return;
        }
    }
    if (!Active && !RegionChange && !FadeInOut.IsActive())
        return;
    SetAsDrawing(true);
    if (!World->Paused && !Component->GetDisplayEngine().IsScreenFadeOutClosing())
        ScreenPosFadeInOut.Update(info.GTPredictedTimeSinceLastRenderFrame);

    C2DBoxF screen = Component->GetDisplayEngine().GetMainWindow();
    float size = GPDisplayManager->GetRenderTargetDimensions().Width * 0.4f;
    C2DVector centre_tl((screen.GetWidth() - size) * 0.5f, (screen.GetHeight() - size) * 0.5f);
    C2DVector centre_br(centre_tl.X + size, centre_tl.Y + size);
    C2DBoxF box(centre_tl.X, centre_tl.Y, centre_br.X, centre_br.Y);
    if (!RegionChange) {
        const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
        ScreenBox = C2DBoxF(GFAnchorToTopRight(def.MiniMapScreenOrigin), C2DVector(def.MiniMapScreenSize.X, def.MiniMapScreenSize.Y));
        box = ScreenBox;
        if (ScreenPosFadeInOut.IsOpen() || ScreenPosFadeInOut.IsOpening() || ScreenPosFadeInOut.IsClosing()) {
            box.TLX += ScreenPosFadeInOut.GetFadeInOutAlpha() * (centre_tl.X - ScreenBox.TLX);
            box.TLY += ScreenPosFadeInOut.GetFadeInOutAlpha() * (centre_tl.Y - ScreenBox.TLY);
            box.BRX += ScreenPosFadeInOut.GetFadeInOutAlpha() * (centre_br.X - ScreenBox.BRX);
            box.BRY += ScreenPosFadeInOut.GetFadeInOutAlpha() * (centre_br.Y - ScreenBox.BRY);
        }
    }

    float fade = FadeInOut.GetFadeInOutAlpha() * 255.0f;
    if (fade > 255.0f)
        fade = 255.0f;
    unsigned char alpha = (unsigned char)fade;
    if (alpha > Alpha)
        alpha = Alpha;
    unsigned char map_alpha = ScaleAlpha(alpha);
    if (RegionChange)
        map_alpha = alpha;
    MiniMap.DrawPreparePrimitivesForEngine(info, box, map_alpha, RegionChange);

    if (RegionChange) {
        const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
        C2DBoxF tile;
        float tile_height;
        tile.TLX = def.RegionChangeTLOffset.X;
        tile.TLY = def.RegionChangeTLOffset.Y;
        tile.BRX = tile.TLX + (float)(GPDisplayManager->GetRenderTargetDimensions().Width * Bank->GetGraphicWidth(def.RegionChangeTLGraphic)) * (1.0f / 640.0f);
        tile_height = (float)(Bank->GetGraphicHeight(def.RegionChangeTLGraphic) * GPDisplayManager->GetRenderTargetDimensions().Height) * (1.0f / 480.0f);
        tile.BRY = tile.TLY + tile_height;
        CEnginePrimitive2DSprite top_left(0, tile, 0x45, def.RegionChangeTLGraphic, 0, 2, 0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2);
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[0], top_left, 0xc0);

        float xTM = tile.TLX + tile.GetWidth();
        tile.TLX = xTM;
        tile.BRX = xTM + (float)(GPDisplayManager->GetRenderTargetDimensions().Width * Bank->GetGraphicWidth(def.RegionChangeTMGraphic)) * (1.0f / 640.0f);
        tile_height = (float)(Bank->GetGraphicHeight(def.RegionChangeTMGraphic) * GPDisplayManager->GetRenderTargetDimensions().Height) * (1.0f / 480.0f);
        tile.BRY = tile.TLY + tile_height;
        CEnginePrimitive2DSprite top_middle(0, tile, 0x45, def.RegionChangeTMGraphic, 0, 2, 0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2);
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[1], top_middle, 0xc0);

        float xTR = tile.TLX + tile.GetWidth();
        tile.TLX = xTR;
        tile.BRX = xTR + (float)(GPDisplayManager->GetRenderTargetDimensions().Width * Bank->GetGraphicWidth(def.RegionChangeTRGraphic)) * (1.0f / 640.0f);
        tile_height = (float)(Bank->GetGraphicHeight(def.RegionChangeTRGraphic) * GPDisplayManager->GetRenderTargetDimensions().Height) * (1.0f / 480.0f);
        tile.BRY = tile.TLY + tile_height;
        CEnginePrimitive2DSprite top_right(0, tile, 0x45, def.RegionChangeTRGraphic, 0, 2, 0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2);
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[2], top_right, 0xc0);

        float xBL = def.RegionChangeTLOffset.X;
        tile.TLX = xBL;
        tile.TLY = tile.BRY;
        tile.BRX = xBL + (float)(GPDisplayManager->GetRenderTargetDimensions().Width * Bank->GetGraphicWidth(def.RegionChangeBLGraphic)) * (1.0f / 640.0f);
        tile_height = (float)(Bank->GetGraphicHeight(def.RegionChangeBLGraphic) * GPDisplayManager->GetRenderTargetDimensions().Height) * (1.0f / 480.0f);
        tile.BRY = tile.TLY + tile_height;
        CEnginePrimitive2DSprite bottom_left(0, tile, 0x45, def.RegionChangeBLGraphic, 0, 2, 0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2);
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[3], bottom_left, 0xc0);

        float xBM = tile.TLX + tile.GetWidth();
        tile.TLX = xBM;
        tile.BRX = xBM + (float)(GPDisplayManager->GetRenderTargetDimensions().Width * Bank->GetGraphicWidth(def.RegionChangeBMGraphic)) * (1.0f / 640.0f);
        tile_height = (float)(Bank->GetGraphicHeight(def.RegionChangeBMGraphic) * GPDisplayManager->GetRenderTargetDimensions().Height) * (1.0f / 480.0f);
        tile.BRY = tile.TLY + tile_height;
        CEnginePrimitive2DSprite bottom_middle(0, tile, 0x45, def.RegionChangeBMGraphic, 0, 2, 0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2);
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[4], bottom_middle, 0xc0);

        float xBR = tile.TLX + tile.GetWidth();
        tile.TLX = xBR;
        tile.BRX = xBR + (float)(GPDisplayManager->GetRenderTargetDimensions().Width * Bank->GetGraphicWidth(def.RegionChangeBRGraphic)) * (1.0f / 640.0f);
        tile_height = (float)(Bank->GetGraphicHeight(def.RegionChangeBRGraphic) * GPDisplayManager->GetRenderTargetDimensions().Height) * (1.0f / 480.0f);
        tile.BRY = tile.TLY + tile_height;
        CEnginePrimitive2DSprite bottom_right(0, tile, 0x45, def.RegionChangeBRGraphic, 0, 2, 0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2);
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[5], bottom_right, 0xc0);

        const C2DVector& left_pos = def.RegionChangeBorderLeftOffset;
        C2DBoxF left = GFToScreenScaled(C2DBoxF(left_pos.X, left_pos.Y, left_pos.X + (float)Bank->GetGraphicWidth(def.RegionChangeBorderLeftGraphic),
                                                left_pos.Y + (float)Bank->GetGraphicWidth(def.RegionChangeBorderRightGraphic)));
        float height = GPDisplayManager->GetRenderTargetDimensions().Height * (1.0f / 480.0f) * (float)Bank->GetGraphicHeight(def.RegionChangeBorderLeftGraphic);
        left.TLY = (GPDisplayManager->GetRenderTargetDimensions().Height - height) * 0.5f;
        left.BRY = left.TLY + height;
        CEnginePrimitive2DSprite left_border(0, left, 0x44, def.RegionChangeBorderLeftGraphic, 0, 2, 0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2);
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[6], left_border, 0xc0);

        const C2DVector& right_pos = def.RegionChangeBorderRightOffset;
        C2DBoxF right = GFToScreenScaled(C2DBoxF(right_pos.X, right_pos.Y, right_pos.X + (float)Bank->GetGraphicWidth(def.RegionChangeBorderRightGraphic),
                                                 right_pos.Y + (float)Bank->GetGraphicHeight(def.RegionChangeBorderRightGraphic)));
        right.TLY = left.TLY;
        right.BRY = left.BRY;
        CEnginePrimitive2DSprite right_border(0, right, 0x44, def.RegionChangeBorderRightGraphic, 0, 2, 0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2);
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[7], right_border, 0xc0);
    }

    if (!RegionChange && ScreenPosFadeInOut.IsActive()) {
        const CFontBank& font = Component->PeekCutSceneMainFont();
        for (unsigned long i = 0; i < RegionExits.size(); ++i) {
            const CCharString& symbol = RegionExits[i].second;
            CWideString text = Component->GetTextBank().GetTextBySymbol(symbol);
            if (text == L"")
                text = symbol.ToWideString();
            const CCharString& name = RegionExits[i].second;
            if (name == "TXT_REGION_PRISON_BLOCK_B" || name == "TXT_REGION_PRISON_BLOCK_C")
                text = L"";
            if (text != L"" && MiniMap.IsThingAsPosBeingDrawn(RegionExits[i].first)) {
                float scale = ScreenPosFadeInOut.GetFadeInOutAlpha() * 0.5f;
                scale += 0.5f;
                unsigned char text_alpha = (unsigned char)(ScreenPosFadeInOut.GetFadeInOutAlpha() * 255.0f);
                font.GetStringWidth(text);
                C2DVector pos = MiniMap.GetDrawPosition(RegionExits[i].first, box);
                PlayerGui->Draw2DText(C2DVector(GFAXToVX(pos.X), GFAYToVY(pos.Y)), text, font,
                    CRGBColour(0xff, 0xff, 0xff, ScaleAlpha(text_alpha)), scale, (EEngine2DLayer)0x43, true);
            }
        }
    }
}

}
