// CSubtitleRenderer::DrawPreparePrimitivesForEngine(const CDisplayInfo*) -- tick the skip-cutscene fade, drop the all-shown flag when the game is not paused and the skip prompt is up, then lay out the subtitle text box, size the button panel to the three button texts, draw the button background and button text (or, in a cutscene / paused game, just the button text a little higher); release the unused primitive handle sets
// Grade: DIFFER(1097v1094) -- frame 0x80 vs 0x64 (per-branch locals not shared), the dt select is fld [eax+0x10]/jmp not add eax,0x10, and the all-shown/in-cutscene test is not split into the retail goto-shaped retest
// Leaf: DrawPreparePrimitivesForEngine
// Retail: Fable.exe (TLC, VC7.1) @ 0x00490494, 1094 bytes, `ret 4`; size-optimised TU with an ebp frame. NOTE: retail offsets do not
//   follow the Ego_r.pdb CSubtitleRenderer layout (vectors are 12 bytes here, flags at +0x7c), so a local layout is used.
// Callees: CFadeInFadeOutBase::Update 0x00434870, GetGameState 0x0040D2A0, GetSubtitleDef 0x00438570, CalcLayout 0x004900F5,
//   GetResolution 0x009BEDC0, ScaleY 0x009E1E40, ScaleX 0x009E1E10, GetTextOffset 0x0048DBC3, GetExtents 0x009E1E70 (__fastcall),
//   GetFont 0x009E2C80 / handle dtor 0x004057A0, MeasureText 0x0048E062, DrawBackground 0x0048FAA0, DrawButtonText 0x0048FAB6,
//   DrawEmbedded 0x0048E78E, for_each(handles, mem_fun_ref(Release 0x0041BE70)) 0x0043F360 (__fastcall, hidden result).
// Literals: 640.0f 0x01238168, 24.0f 0x0123822C, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 10.0f 0x01237F0C; 65536.0 (double) 0x013961F0 / 0x013961E8.
#pragma optimize("s", on)
#pragma optimize("y", off)

struct CDisplayInfo { float Time; float _pad[3]; float PausedTime; };
struct C2DVector { float X, Y; };
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxF() { }
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};
struct CTextLayout { unsigned char _data[0x30]; CTextLayout() { } };
void __fastcall GetExtents(const CTextLayout& layout, C2DBoxF& extents);   // 0x009E1E70
struct CIntVec2 { long X, Y; CIntVec2() { } };
struct CDisplay { CIntVec2 GetResolution() const; };                    // 0x009BEDC0
extern CDisplay* g_Display;                                              // 0x013B8390
float ScaleX(float v);                                                   // 0x009E1E10
float ScaleY(float v);                                                   // 0x009E1E40
extern double g_VirtualWidth;                                            // 0x013961E8
extern double g_VirtualHeight;                                           // 0x013961F0
struct CFont;
struct CFontHandle { CFont* P; ~CFontHandle(); };                        // dtor 0x004057A0
struct CCharString { void* PStringData; };
struct CFontManager { CFontHandle GetFont(const CCharString& name); };   // 0x009E2C80
extern CFontManager* g_FontManager;                                      // 0x013B838C
struct CWideString { void* PStringData; };
struct CSubtitleDef {
    unsigned char _pad00[0x2b8];
    float MinButtonPanelHeight;                                          // +0x2b8
    float ButtonPanelBorderX;                                            // +0x2bc
    float ButtonPanelBorderY;                                            // +0x2c0
    unsigned char _pad2c4[0x5ac - 0x2c4];
    float TextBaseX;                                                     // +0x5ac
    unsigned char _pad5b0[0x5d0 - 0x5b0];
    float ButtonSpacing;                                                 // +0x5d0
    CCharString ButtonFontName;                                          // +0x5d4
};
CSubtitleDef* GetSubtitleDef();                                          // 0x00438570
struct CGameState {
    unsigned char _pad00[0xc];
    bool UseAlt;                                                         // +0x0c
    bool Paused;                                                         // +0x0d
    unsigned char _pad0e[0xcd - 0xe];
    bool AltPaused;                                                      // +0xcd
    bool IsPaused() const { return UseAlt ? AltPaused : Paused; }
};
CGameState* GetGameState();                                              // 0x0040D2A0
struct CEngineWorld { unsigned char _pad[0x38]; struct CSkipper* Skipper; unsigned char _pad3c[0xa4 - 0x3c]; bool PausedTime; };
struct CSkipper {
    virtual void s000();
    virtual void s004();
    virtual void s008();
    virtual void s00c();
    virtual void s010();
    virtual void s014();
    virtual void s018();
    virtual void s01c();
    virtual void s020();
    virtual void s024();
    virtual void s028();
    virtual void s02c();
    virtual void s030();
    virtual void s034();
    virtual void s038();
    virtual void s03c();
    virtual void s040();
    virtual void s044();
    virtual void s048();
    virtual void s04c();
    virtual void s050();
    virtual void s054();
    virtual void s058();
    virtual void s05c();
    virtual void s060();
    virtual void s064();
    virtual void s068();
    virtual void s06c();
    virtual void s070();
    virtual void s074();
    virtual void s078();
    virtual void s07c();
    virtual void s080();
    virtual void s084();
    virtual void s088();
    virtual void s08c();
    virtual void s090();
    virtual void s094();
    virtual void s098();
    virtual void s09c();
    virtual void s0a0();
    virtual void s0a4();
    virtual void s0a8();
    virtual void s0ac();
    virtual void s0b0();
    virtual void s0b4();
    virtual void s0b8();
    virtual void s0bc();
    virtual void s0c0();
    virtual void s0c4();
    virtual void s0c8();
    virtual void s0cc();
    virtual void s0d0();
    virtual void s0d4();
    virtual void s0d8();
    virtual void s0dc();
    virtual void s0e0();
    virtual void s0e4();
    virtual void s0e8();
    virtual void s0ec();
    virtual void s0f0();
    virtual void s0f4();
    virtual void s0f8();
    virtual void s0fc();
    virtual void s100();
    virtual void s104();
    virtual void s108();
    virtual void s10c();
    virtual void s110();
    virtual void s114();
    virtual void s118();
    virtual void s11c();
    virtual void s120();
    virtual void s124();
    virtual void s128();
    virtual void s12c();
    virtual void s130();
    virtual void s134();
    virtual void s138();
    virtual void s13c();
    virtual void s140();
    virtual void s144();
    virtual void s148();
    virtual void s14c();
    virtual void s150();
    virtual void s154();
    virtual void s158();
    virtual void s15c();
    virtual void s160();
    virtual void s164();
    virtual void s168();
    virtual void s16c();
    virtual void s170();
    virtual void s174();
    virtual void s178();
    virtual void s17c();
    virtual void s180();
    virtual void s184();
    virtual void s188();
    virtual void s18c();
    virtual void s190();
    virtual void s194();
    virtual void s198();
    virtual void s19c();
    virtual void s1a0();
    virtual void s1a4();
    virtual void s1a8();
    virtual void s1ac();
    virtual void s1b0();
    virtual void s1b4();
    virtual void s1b8();
    virtual void s1bc();
    virtual void s1c0();
    virtual void s1c4();
    virtual void s1c8();
    virtual void s1cc();
    virtual void s1d0();
    virtual void s1d4();
    virtual void s1d8();
    virtual void s1dc();
    virtual void s1e0();
    virtual void s1e4();
    virtual void s1e8();
    virtual void s1ec();
    virtual void s1f0();
    virtual void s1f4();
    virtual void s1f8();
    virtual void s1fc();
    virtual void s200();
    virtual void s204();
    virtual void s208();
    virtual void s20c();
    virtual void s210();
    virtual void s214();
    virtual void s218();
    virtual void s21c();
    virtual void s220();
    virtual void s224();
    virtual void s228();
    virtual void s22c();
    virtual void s230();
    virtual void s234();
    virtual void s238();
    virtual void s23c();
    virtual void s240();
    virtual void s244();
    virtual void s248();
    virtual void s24c();
    virtual void s250();
    virtual void s254();
    virtual void s258();
    virtual void s25c();
    virtual void s260();
    virtual void s264();
    virtual void s268();
    virtual void s26c();
    virtual void s270();
    virtual void s274();
    virtual void s278();
    virtual void s27c();
    virtual void s280();
    virtual void s284();
    virtual void s288();
    virtual void s28c();
    virtual void s290();
    virtual void s294();
    virtual void s298();
    virtual void s29c();
    virtual void s2a0();
    virtual void s2a4();
    virtual void s2a8();
    virtual void s2ac();
    virtual void s2b0();
    virtual void s2b4();
    virtual void s2b8();
    virtual void s2bc();
    virtual void s2c0();
    virtual void s2c4();
    virtual void s2c8();
    virtual void s2cc();
    virtual void s2d0();
    virtual void s2d4();
    virtual void s2d8();
    virtual void s2dc();
    virtual void s2e0();
    virtual void s2e4();
    virtual void s2e8();
    virtual void s2ec();
    virtual void s2f0();
    virtual void s2f4();
    virtual void s2f8();
    virtual void s2fc();
    virtual void s300();
    virtual void s304();
    virtual void s308();
    virtual void s30c();
    virtual void s310();
    virtual void s314();
    virtual void s318();
    virtual void s31c();
    virtual void s320();
    virtual void s324();
    virtual void s328();
    virtual void s32c();
    virtual void s330();
    virtual void s334();
    virtual void s338();
    virtual void s33c();
    virtual void s340();
    virtual void s344();
    virtual void s348();
    virtual void s34c();
    virtual void s350();
    virtual void s354();
    virtual void s358();
    virtual void s35c();
    virtual void s360();
    virtual void s364();
    virtual void s368();
    virtual void s36c();
    virtual void s370();
    virtual void s374();
    virtual void s378();
    virtual void s37c();
    virtual void s380();
    virtual void s384();
    virtual void s388();
    virtual void s38c();
    virtual void s390();
    virtual void s394();
    virtual void s398();
    virtual void s39c();
    virtual void s3a0();
    virtual void s3a4();
    virtual void s3a8();
    virtual void s3ac();
    virtual void s3b0();
    virtual void s3b4();
    virtual void s3b8();
    virtual void s3bc();
    virtual void s3c0();
    virtual void s3c4();
    virtual void s3c8();
    virtual void s3cc();
    virtual void s3d0();
    virtual void s3d4();
    virtual void s3d8();
    virtual void s3dc();
    virtual void s3e0();
    virtual void s3e4();
    virtual void s3e8();
    virtual void s3ec();
    virtual void s3f0();
    virtual void s3f4();
    virtual void s3f8();
    virtual void s3fc();
    virtual void s400();
    virtual void s404();
    virtual void s408();
    virtual void s40c();
    virtual void s410();
    virtual void s414();
    virtual void s418();
    virtual void s41c();
    virtual void s420();
    virtual void s424();
    virtual void s428();
    virtual void s42c();
    virtual void s430();
    virtual void s434();
    virtual void s438();
    virtual void s43c();
    virtual void s440();
    virtual void s444();
    virtual void s448();
    virtual void s44c();
    virtual void s450();
    virtual void s454();
    virtual void s458();
    virtual void s45c();
    virtual void s460();
    virtual void s464();
    virtual void s468();
    virtual void s46c();
    virtual void s470();
    virtual void s474();
    virtual void s478();
    virtual void s47c();
    virtual void s480();
    virtual void s484();
    virtual void s488();
    virtual void s48c();
    virtual void s490();
    virtual void s494();
    virtual void s498();
    virtual void s49c();
    virtual void s4a0();
    virtual void s4a4();
    virtual void s4a8();
    virtual void s4ac();
    virtual void s4b0();
    virtual void s4b4();
    virtual void s4b8();
    virtual void s4bc();
    virtual void s4c0();
    virtual void s4c4();
    virtual void s4c8();
    virtual void s4cc();
    virtual void s4d0();
    virtual void s4d4();
    virtual void s4d8();
    virtual void s4dc();
    virtual void s4e0();
    virtual void s4e4();
    virtual void s4e8();
    virtual void s4ec();
    virtual void s4f0();
    virtual void s4f4();
    virtual void s4f8();
    virtual void s4fc();
    virtual void s500();
    virtual void s504();
    virtual void s508();
    virtual void s50c();
    virtual void s510();
    virtual void s514();
    virtual void s518();
    virtual void s51c();
    virtual void s520();
    virtual void s524();
    virtual void s528();
    virtual void s52c();
    virtual void s530();
    virtual void s534();
    virtual void s538();
    virtual void s53c();
    virtual void s540();
    virtual void s544();
    virtual void s548();
    virtual void s54c();
    virtual void s550();
    virtual void s554();
    virtual void s558();
    virtual void s55c();
    virtual void s560();
    virtual void s564();
    virtual void s568();
    virtual void s56c();
    virtual void s570();
    virtual void s574();
    virtual void s578();
    virtual void s57c();
    virtual void s580();
    virtual void s584();
    virtual void s588();
    virtual void s58c();
    virtual void s590();
    virtual void s594();
    virtual void s598();
    virtual void s59c();
    virtual void s5a0();
    virtual void s5a4();
    virtual void s5a8();
    virtual void s5ac();
    virtual void s5b0();
    virtual void s5b4();
    virtual void s5b8();
    virtual void s5bc();
    virtual void s5c0();
    virtual void s5c4();
    virtual void s5c8();
    virtual void s5cc();
    virtual void s5d0();
    virtual void s5d4();
    virtual void s5d8();
    virtual void s5dc();
    virtual void s5e0();
    virtual void s5e4();
    virtual void s5e8();
    virtual void s5ec();
    virtual void s5f0();
    virtual void s5f4();
    virtual void s5f8();
    virtual void s5fc();
    virtual void s600();
    virtual void s604();
    virtual void s608();
    virtual void s60c();
    virtual void s610();
    virtual void s614();
    virtual void s618();
    virtual void s61c();
    virtual void s620();
    virtual void s624();
    virtual void s628();
    virtual bool IsSkipPromptShowing();                                  // vtable +0x62c
};
struct CEngineRoot { unsigned char _pad[0x24]; CEngineWorld* World; };
extern CEngineRoot* g_EngineRoot;                                        // 0x013B86A0
struct CEnginePrimitiveHandle { void Release(); };                       // 0x0041BE70
struct CReleaseFn {
    void (CEnginePrimitiveHandle::*F)();
    CReleaseFn(void (CEnginePrimitiveHandle::*f)()) : F(f) { }
};
CReleaseFn __fastcall ForEach(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last, CReleaseFn fn);   // 0x0043F360
struct CHandleVector { CEnginePrimitiveHandle* Start; CEnginePrimitiveHandle* Finish; CEnginePrimitiveHandle* End; };
struct CFadeInFadeOutBase { void Update(float dt); };                    // 0x00434870

class CSubtitleRenderer {
public:
    void* __vftable;
    C2DBoxF VWindow;                                                     // +0x04
    C2DBoxF VTelevisionWindow;                                           // +0x14
    unsigned char _pad24[0x64 - 0x24];
    CWideString Button1Text;                                             // +0x64
    CWideString Button2Text;                                             // +0x68
    CWideString Button3Text;                                             // +0x6c
    unsigned char _pad70[0x7c - 0x70];
    unsigned long DisplayFlags;                                          // +0x7c
    unsigned char _pad80[0x8c - 0x80];
    float ButtonPulse;                                                   // +0x8c
    unsigned char _pad90[0x94 - 0x90];
    CHandleVector ButtonHandles;                                         // +0x94
    CHandleVector EmbeddedHandles;                                       // +0xa0
    CHandleVector HighlightHandles;                                      // +0xac
    unsigned char _padb8[0xde - 0xb8];
    bool InCutscene;                                                     // +0xde
    unsigned char _paddf[0xe4 - 0xdf];
    bool PausedGame;                                                     // +0xe4
    bool AllCharactersDisplayed;                                         // +0xe5
    unsigned char _pade6[0xe8 - 0xe6];
    CFadeInFadeOutBase SkipCutsceneTimer;                                // +0xe8

    CTextLayout CalcLayout(const CDisplayInfo* info, const C2DBoxF& window, const C2DBoxF& tvWindow);   // 0x004900F5
    float GetTextOffset() const;                                         // 0x0048DBC3
    float MeasureText(const CWideString& text, CFont* font);             // 0x0048E062
    void DrawBackground(const CDisplayInfo* info, CHandleVector& handles, const C2DBoxF& box);   // 0x0048FAA0
    bool DrawButtonText(C2DVector* pos, bool withPanel);                 // 0x0048FAB6
    void DrawEmbedded(const CDisplayInfo* info, CHandleVector& handles, C2DVector* pos, bool withPanel);   // 0x0048E78E
    void DrawPreparePrimitivesForEngine(const CDisplayInfo* info);
};

void CSubtitleRenderer::DrawPreparePrimitivesForEngine(const CDisplayInfo* info)
{
    SkipCutsceneTimer.Update(g_EngineRoot->World->PausedTime ? info->PausedTime : info->Time);
    if (AllCharactersDisplayed && !(DisplayFlags & 0x2c) && (DisplayFlags & 2)
        && !GetGameState()->IsPaused() && g_EngineRoot->World->Skipper->IsSkipPromptShowing())
        AllCharactersDisplayed = false;
    bool drawn = false;
    if (AllCharactersDisplayed && (!InCutscene || (DisplayFlags & 0x20))) {
        CSubtitleDef* def = GetSubtitleDef();
        CTextLayout layout = CalcLayout(info, VWindow, VTelevisionWindow);
        float resX = (float)g_Display->GetResolution().X;
        float height = ScaleY((float)g_VirtualHeight);
        C2DVector pos;
        pos.Y = height - GetTextOffset();
        pos.X = resX + def->TextBaseX - 640.0f;
        bool withPanel = false;
        if (!(DisplayFlags & 2) || (DisplayFlags & 0x28)) {
            C2DBoxF extents;
            GetExtents(layout, extents);
            float width = extents.BRX - extents.TLX + (def->ButtonPanelBorderX + def->ButtonPanelBorderX);
            float panelHeight = extents.BRY - extents.TLY + (def->ButtonPanelBorderY + def->ButtonPanelBorderY);
            if (panelHeight < def->MinButtonPanelHeight)
                panelHeight = def->MinButtonPanelHeight;
            CFont* font = g_FontManager->GetFont(def->ButtonFontName).P;
            float buttons = ScaleX(MeasureText(Button1Text, font)) + 24.0f;
            buttons = ScaleX(MeasureText(Button2Text, font)) + buttons;
            buttons = ScaleX(MeasureText(Button3Text, font)) + buttons + def->ButtonSpacing * 3.0f;
            if (buttons > width)
                width = buttons;
            float x = (ScaleX((float)g_VirtualWidth) - width) * 0.5f;
            float y = ScaleY((float)g_VirtualHeight) - panelHeight - GetTextOffset();
            if (DisplayFlags & 8)
                y = (ScaleY((float)g_VirtualHeight) - panelHeight) * 0.5f;
            C2DBoxF box;
            box = C2DBoxF(x, y, x + width, y + panelHeight);
            DrawBackground(info, ButtonHandles, box);
            withPanel = true;
            pos.X = box.BRX - def->ButtonSpacing;
            if (DisplayFlags & 8)
                pos.Y = box.BRY;
        }
        drawn = DrawButtonText(&pos, withPanel);
        if (drawn)
            DrawEmbedded(info, EmbeddedHandles, &pos, withPanel);
    } else if (InCutscene || PausedGame) {
        CSubtitleDef* def = GetSubtitleDef();
        float resX = (float)g_Display->GetResolution().X;
        float height = ScaleY((float)g_VirtualHeight);
        C2DVector pos;
        pos.Y = height - GetTextOffset();
        pos.X = resX + def->TextBaseX - 640.0f - 10.0f;
        drawn = DrawButtonText(&pos, false);
        if (drawn)
            DrawEmbedded(info, EmbeddedHandles, &pos, false);
    } else if (!GetGameState()->IsPaused() && (DisplayFlags & 2)) {
        CSubtitleDef* def = GetSubtitleDef();
        float resX = (float)g_Display->GetResolution().X;
        float height = ScaleY((float)g_VirtualHeight);
        C2DVector pos;
        pos.Y = height - GetTextOffset();
        pos.X = resX + def->TextBaseX - 640.0f;
        drawn = DrawButtonText(&pos, false);
        ButtonPulse = 0.25f;
        if (drawn)
            DrawEmbedded(info, EmbeddedHandles, &pos, false);
    } else {
        ForEach(ButtonHandles.Start, ButtonHandles.Finish, CReleaseFn(&CEnginePrimitiveHandle::Release));
        ForEach(HighlightHandles.Start, HighlightHandles.Finish, CReleaseFn(&CEnginePrimitiveHandle::Release));
    }
    if (!drawn)
        ForEach(EmbeddedHandles.Start, EmbeddedHandles.Finish, CReleaseFn(&CEnginePrimitiveHandle::Release));
}
