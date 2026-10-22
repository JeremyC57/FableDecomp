// NPlayerGui::CDrawScreenFade::Draw(CEnginePrimitivePrepareInfo*) -- draw the full-screen fade box: a black box over the virtual screen while the global force-black flag is set; otherwise, unless the hero's quest-completion UI overrides the fade, while the display engine fades draw its colour (alpha from the engine) over the virtual screen grown by 8 units, on layer 0x46 when the full fade-out was drawn and the region-load minimap shows (or the loading cutscene can be skipped), 0x4c when the hero's 0xcb component forces black or the GUI flags it, else 0x42 (remembering a fully opaque fade-out); when no fade runs, clear the remembered fade-out
// Grade: DIFFER(937v938) -- control flow and calls follow retail, but the frame differs (0x90 v 0xe4): retail keeps the black box (+0x38), the fade box (+0x48) and two sprite objects (-0x1c for black/0x46/0x4c, -0x70 for 0x42) in separate words, while VC shares one sprite and one box across the sibling blocks; retail also loads both virtual-screen doubles before building the black box. Tried a named box, a temporary sprite for 0x42, a hoisted fade box, an inline / __forceinline AddBox helper
// Leaf: Draw
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006495A8, length 938 bytes (biased ebp frame, `ret 4`).
// Callees: GetMainPlayer 0x00449970, GetControlledCreature 0x00487DD0, IS_THING_ALIVE 0x0043DBF0, bitset::test 0x00410DE0,
//   LowerBound 0x004365B0, CTCQuestCompletionUI::IsOverridingScreenFade 0x005E3E70, GFAXToVX 0x009E1DA0, display-engine fade alpha
//   0x004364C0 / IsScreenFullyFadedOut 0x004A9210, CWorld 0x0049E9F0 / 0x0048DB13, CSubtitleRenderer::IsCutsceneSkippable 0x0048DE44,
//   CEnginePrimitive2DSprite (box) 0x0041BEB0, renderer slot 23. Globals: force-black flag 0x013B89C0, virtual screen doubles
//   0x013961E8 / 0x013961F0 (65536.0); constants 8.0f 0x0125916C, -8.0f 0x0125A298.
#pragma optimize("s", on)
#include <stdlib.h>
struct CTCBase;
struct CPresenceBits {
    bool test(long bit);                           // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad0[0x20];
    mutable CPresenceBits m_present;               // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                    // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long type) const
    {
        if (m_present.test(type)) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *ppOut = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBF0
bool IsThingAliveImpl(const void* thing);
#pragma auto_inline(off)
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && IsThingAliveImpl(thing);
}
#pragma auto_inline(on)
struct CPlayer {
    CThing* GetControlledCreature();               // 0x00487DD0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                      // 0x00449970
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) {}
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
float __fastcall GFAXToVX(float x);                // 0x009E1DA0
extern bool   g_ForceBlackScreen;                  // 0x013B89C0
extern double g_VirtualScreenWidth;                // 0x013961E8
extern double g_VirtualScreenHeight;               // 0x013961F0
struct CEnginePrimitiveHandle { unsigned long Data[2]; };
class CEnginePrimitive2DSprite {
public:
    unsigned char _body[0x54];
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DBoxF& box, long layer, unsigned long graphic, unsigned long b, long blend,
                             long interpolation, float scaleX, float scaleY, unsigned char alpha, unsigned char red, unsigned char green,
                             unsigned char blue, unsigned long e, const C2DVector& uv0, const C2DVector& uv1);   // 0x0041BEB0
};
class CEnginePrimitiveRenderer {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DSprite& primitive, long flags);   // slot 23
};
class CEnginePrimitivePrepareInfo {
public:
    unsigned char _pad00[0x34];
    CEnginePrimitiveRenderer* Renderer;             // +0x34
};
struct CHandleVector { CEnginePrimitiveHandle *Start, *Finish, *End; };
struct CTCQuestCompletionUI { bool IsOverridingScreenFade(); };   // 0x005E3E70
struct CTCScreenFadeOverride { unsigned char _pad[0x26]; bool ForceBlack; };   // interface 0xcb, +0x26
class CDisplayEngine {
public:
    unsigned char _pad00[0xbc];
    bool          FadeActive;                       // +0xbc
    unsigned char _padbd[0xd4 - 0xbd];
    CRGBColour    FadeColour;                       // +0xd4
    unsigned char GetFadeAlpha();                   // 0x004364C0
    bool IsScreenFullyFadedOut();                   // 0x004A9210
};
struct CMainGameResources { unsigned char _pad[0x28]; CDisplayEngine* DisplayEngine; };   // +0x28
class CWorld {
public:
    unsigned char _pad[0xa4];
    bool          ShowingLoadScreen;                // +0xa4
    bool IsDisplayingMiniMapOnRegionLoad();         // 0x0049E9F0
    bool IsInMode();                                // 0x0048DB13
};
class CSubtitleRenderer { public: bool IsCutsceneSkippable(); };   // 0x0048DE44
struct CGameUI {
    unsigned char     _pad00[0x18];
    CSubtitleRenderer Subtitles;                    // +0x18
    unsigned char     _pad19[0x94 - 0x19];
    unsigned long     Flags;                        // +0x94
    unsigned char     _pad98[0xf6 - 0x98];
    bool              Bf6;                          // +0xf6
    unsigned char     _padf7[2];
    bool              Bf9;                          // +0xf9
    unsigned char     _padfa[3];
    bool              Bfd;                          // +0xfd
};

namespace NPlayerGui {
struct CDrawScreenFade {
    virtual void v00();
    CGameUI*            PlayerGui;                  // +0x04
    CPlayerManager*     PlayerManager;              // +0x08
    CWorld*             World;                      // +0x0c
    CMainGameResources* Resources;                  // +0x10
    void*               Bank;                       // +0x14
    bool                Drawing;                    // +0x18
    CHandleVector       Handles;                    // +0x1c
    bool                DrawnFullFadeOut;           // +0x28
    void Draw(CEnginePrimitivePrepareInfo* info);
};

void CDrawScreenFade::Draw(CEnginePrimitivePrepareInfo* info)
{
    if (g_ForceBlackScreen) {
        Drawing = true;
        CEnginePrimitive2DSprite black(0, C2DBoxF(0.0f, 0.0f, (float)g_VirtualScreenWidth, (float)g_VirtualScreenHeight), 0x4c, 0, 0, 2, 0,
                                       0.0f, 0.0f, 0xff, 0, 0, 0, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info->Renderer->AddPrimitive(&Handles.Start[0], black, 0xc0);
        return;
    }
    CThing* hero = PlayerManager->GetMainPlayer()->GetControlledCreature();
    CTCQuestCompletionUI* questUI;
    if (IS_THING_ALIVE(hero) && hero->GetInterface(&questUI, 0xfc) && questUI->IsOverridingScreenFade())
        return;
    CDisplayEngine* engine = Resources->DisplayEngine;
    if (engine->FadeActive) {
        Drawing = true;
        float bottom = GFAXToVX(8.0f) + g_VirtualScreenHeight;
        float right = GFAXToVX(8.0f) + g_VirtualScreenWidth;
        const CRGBColour& fade = engine->FadeColour;
        CRGBColour colour(fade.R, fade.G, fade.B, engine->GetFadeAlpha());
        CTCScreenFadeOverride* fadeOverride;
        if (DrawnFullFadeOut && engine->IsScreenFullyFadedOut() && World->IsDisplayingMiniMapOnRegionLoad() && World->IsInMode()) {
            CEnginePrimitive2DSprite box(0, C2DBoxF(-8.0f, -8.0f, right, bottom), 0x46, 0, 0, 2, 0, 0.0f, 0.0f, colour.A, colour.R, colour.G,
                                         colour.B, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info->Renderer->AddPrimitive(&Handles.Start[0], box, 0xc0);
        } else if ((IS_THING_ALIVE(hero) && hero->GetInterface(&fadeOverride, 0xcb) && fadeOverride->ForceBlack)
                   || (PlayerGui->Bfd && (PlayerGui->Flags & 8))) {
            CEnginePrimitive2DSprite box(0, C2DBoxF(-8.0f, -8.0f, right, bottom), 0x4c, 0, 0, 2, 0, 0.0f, 0.0f, colour.A, colour.R, colour.G,
                                         colour.B, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info->Renderer->AddPrimitive(&Handles.Start[0], box, 0xc0);
        } else if (World->ShowingLoadScreen && PlayerGui->Bf6 && PlayerGui->Subtitles.IsCutsceneSkippable() && PlayerGui->Bf9) {
            CEnginePrimitive2DSprite box(0, C2DBoxF(-8.0f, -8.0f, right, bottom), 0x46, 0, 0, 2, 0, 0.0f, 0.0f, colour.A, colour.R, colour.G,
                                         colour.B, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info->Renderer->AddPrimitive(&Handles.Start[0], box, 0xc0);
        } else {
            CEnginePrimitive2DSprite box(0, C2DBoxF(-8.0f, -8.0f, right, bottom), 0x42, 0, 0, 2, 0, 0.0f, 0.0f, colour.A, colour.R, colour.G,
                                         colour.B, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info->Renderer->AddPrimitive(&Handles.Start[0], box, 0xc0);
            if (colour.A == 0xff)
                DrawnFullFadeOut = true;
        }
    } else if (DrawnFullFadeOut)
        DrawnFullFadeOut = false;
}
}
