// Grade: DIFFER(958v930) -- the gating, alpha blend, TC tests and colour stores match; retail keeps the text position in the frame ([esp+24h], also in the info-display branch where ours scalar-replaces Y into edi), homes the GFAXToVX result in the dead `info` argument slot, and tail-merges the info-display draw with the extra-line draw (ours keeps three draw copies, +28 bytes)
// CTCRegionDisplay::GlobalDraw(const CEnginePrimitivePrepareInfo&) -- fade the region name in/out (blending last/current alpha by the frame fraction) and draw it, right-aligned at the info position or centred near the bottom of the screen with the extra line below, unless the GUI or the owner's TCs hide it
// Leaf: GlobalDraw
// Retail: Fable.exe (TLC, VC7.1) @ 0x00639E40, 944 bytes, `ret 4`.
// Callees: CWideString::operator!=(const wchar_t*) 0x0099B150 (L"" 0x0122D70C), __ftol2 0x00BFEA70, the player GUI 0x013B8790
//   state getter 0x00437CD0, CTCBase display-engine getter 0x00686A80 / engine getter 0x00686D60 (font +0x1614C), const
//   LowerBound 0x004365B0, PeekGuiDef 0x00438570, GFAXToVX 0x009E1DA0 / GFAYToVY 0x009E1DD0, CFontBank::GetVStringWidth
//   0x0043E4D0 / height vslot +0x08, Draw2DText 0x005BCAFE, CDisplayManager::GetRenderTargetDimensions 0x009BEDC0.
//   Floats 0.5f 0x0122F59C, 480.0f 0x01238874.
// Layout: Ego_r.pdb CTCRegionDisplay (as the landed 0x00639A90 / 0x00639BA0); GUI flags +0xf6 / +0xfd / +0x290; gui def
//   region-name Y +0x83c, info position +0x840; TC 0xfc flag +0xd, TC 0xc6 flag +0xc; CRGBColour is B,G,R,A in memory
//   with the constructor assigning R,G,B,A (retail stores +2,+1,+0,+3).
#include <stddef.h>
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct C2DExtentsI {
    long X, Y;
    C2DExtentsI() { }
};
struct CDisplayManager {
    const C2DExtentsI GetRenderTargetDimensions();          // 0x009BEDC0
};
extern CDisplayManager* g_pDisplayDevice;                   // 0x013B8390
struct CRGBColour {
    unsigned char B, G, R, A;                               // D3D order
    CRGBColour() { }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CWideString {
    void* m_pData;
    bool operator!=(const wchar_t* text) const;             // 0x0099B150
};
struct CFontBank {
    virtual void v00(); virtual void v01();
    virtual float GetVHeight();                             // slot 2 (+0x08)
    float GetVStringWidth(const CWideString& text);         // 0x0043E4D0
};
float __fastcall GFAXToVX(float x);                         // 0x009E1DA0
float __fastcall GFAYToVY(float y);                         // 0x009E1DD0
void __fastcall Draw2DText(C2DVector* position, const CWideString* text, CFontBank* font, CRGBColour* colour,
                           float scale, bool shadow);       // 0x005BCAFE
struct CPlayerGuiDef {
    unsigned char _pad00[0x83c];
    float         RegionNameY;                              // +0x83c
    C2DVector     InfoRegionNamePos;                        // +0x840
};
CPlayerGuiDef* __fastcall PeekGuiDef();                     // 0x00438570
struct CGuiState { unsigned char _pad[0x34]; bool Flag34; };
struct CPlayerGui {
    unsigned char _pad00[0xf6];
    bool          HideAll;                                  // +0xf6
    unsigned char _padf7[0xfd - 0xf7];
    bool          HideRegionName;                           // +0xfd
    unsigned char _padfe[0x290 - 0xfe];
    bool          RegionDisplayEnabled;                     // +0x290
    CGuiState* GetState();                                  // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                            // 0x013B8790
struct CGameState { unsigned char _pad[0xbc]; bool Paused; };
struct CMainGameComponent { unsigned char _pad[0x28]; CGameState* State; };
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0
struct CDisplayEngine { unsigned char _pad[0x104]; long Active; };
struct CEngineHolder { unsigned char _pad[0x1614c]; CFontBank* RegionFont; };

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;             // 0x004365B0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long wanted) const
    {
        long type = wanted;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
struct CTCFC { unsigned char _pad[0xd]; bool HideRegion; };
struct CTCC6 { unsigned char _pad[0xc]; bool HideRegion; };
struct CPrepareInfo { long _pad0; float Blend; };

__forceinline void DrawRegionText(const C2DVector& position, const CWideString& text, CFontBank* font, long alpha, bool centred)
{
    float x;
    if (centred)
        x = GFAXToVX(position.X) - font->GetVStringWidth(text) * 0.5f;
    else
        x = GFAXToVX(position.X) - font->GetVStringWidth(text);
    float height = GFAYToVY(font->GetVHeight());
    CRGBColour colour(0xff, 0xff, 0xff, (unsigned char)alpha);
    C2DVector textPos(x, GFAYToVY(position.Y) - height * 0.5f);
    Draw2DText(&textPos, &text, font, &colour, 1.0f, true);
}

struct CTCRegionDisplay {
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04
    unsigned char _pad08[4];
    CWideString   CurrentAreaName;                          // +0x0c
    CWideString   CurrentAreaNameExtra;                     // +0x10
    long          FrameNameChanged;                         // +0x14
    long          CurrentAlpha;                             // +0x18
    long          LastAlpha;                                // +0x1c
    float         DisplayTime;                              // +0x20
    bool          TimeOut;                                  // +0x24
    bool          InfoDisplay;                              // +0x25
    CDisplayEngine* GetDisplayEngine();                     // 0x00686A80
    CEngineHolder* GetEngine();                             // 0x00686D60
    void GlobalDraw(const CPrepareInfo& info);
};

void CTCRegionDisplay::GlobalDraw(const CPrepareInfo& info)
{
    if (CurrentAlpha <= 0)
        return;
    if (!(CurrentAreaName != L""))
        return;
    if (!g_pPlayerGui->RegionDisplayEnabled)
        return;
    long alpha = (long)((float)LastAlpha + ((float)CurrentAlpha - (float)LastAlpha) * info.Blend);
    if (!(CurrentAreaName != L"") && !(CurrentAreaNameExtra != L""))
        return;
    if (alpha <= 0)
        return;
    if (g_pPlayerGui->GetState()->Flag34 && !GetDisplayEngine()->Active)
        return;
    if (g_pPlayerGui->HideAll)
        return;
    CThing* parent = ParentThing;
    if (parent->HasTC(0xfc)) {
        CTCFC* fc = parent->GetTC<CTCFC>(0xfc);
        if (fc && fc->HideRegion)
            return;
    }
    if (parent->HasTC(0xc6)) {
        CTCC6* c6 = parent->GetTC<CTCC6>(0xc6);
        if (c6 && c6->HideRegion)
            return;
    }
    C2DVector pos;
    if (InfoDisplay) {
        if (g_pMainGameComponent->State->Paused)
            return;
        CPlayerGuiDef* gui = PeekGuiDef();
        CFontBank* font = GetEngine()->RegionFont;
        pos = gui->InfoRegionNamePos;
        DrawRegionText(pos, CurrentAreaName, font, alpha, false);
    } else {
        if (g_pPlayerGui->HideRegionName)
            return;
        CPlayerGuiDef* gui = PeekGuiDef();
        CFontBank* font = GetEngine()->RegionFont;
        float y = gui->RegionNameY;
        pos.X = g_pDisplayDevice->GetRenderTargetDimensions().X * 0.5f;
        pos.Y = g_pDisplayDevice->GetRenderTargetDimensions().Y - (480.0f - y);
        DrawRegionText(pos, CurrentAreaName, font, alpha, true);
        if (CurrentAreaNameExtra != L"") {
            pos.Y += font->GetVHeight();
            DrawRegionText(pos, CurrentAreaNameExtra, font, alpha, true);
        }
    }
}
