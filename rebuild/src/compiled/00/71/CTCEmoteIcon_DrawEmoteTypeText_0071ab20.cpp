// CTCEmoteIcon::DrawEmoteTypeText(const CEnginePrimitivePrepareInfo& info) -- within 24 units of the camera, draw the emote text in white half a unit above the thing's static focal point, scaled 1.4x the emote scale
// Retry 10b (combined getter sweep, FIXES B-15): the graphic appearance and the emote font are read through inline getters (`ParentThing->GetGraphicAppearance()`, `GetGameSubsystem()->GetEmoteFont()`); plain member reads give the parked register difference.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071AB20, 244 bytes (+ int3 padding), `ret 4`.
// Callees: CTCBase::GetCamera @ 0x00686A80 (`[g_pMainGameComponent]+0x24`, the world's current camera; manifest
//   "CViewBase::GetDisplayEngine" is propagated), CThing::GetPosition @ 0x004C73D0, the camera's virtual slot 8 (+0x20,
//   position by value through a hidden pointer), CTCGraphicAppearance::GetStaticFocalPosIfLoaded(C3DVector*) @
//   0x004C11F0, CTCBase::GetGameSubsystem @ 0x00686D60 (its +0x16144 font), CEnginePrimitiveText(unsigned __int64,
//   const C3DVector&, float, const CWideString&, const CFontBank&, const CRGBColour&, bool, EEngineInterpolationSet) @
//   0x004C3120, the prepare info's primitive renderer (+0x34) virtual slot 20 (+0x50, add the primitive),
//   ~CWideString @ 0x0099B510 (the text primitive's only non-trivial member).
// Globals: 576.0f @ 0x01265AB0 (24 squared), 1.4f @ 0x01265AAC, the emote text height 0.5f @ 0x01381044 (an extern
//   float variable, named descriptively).
// Layout = Ego_r.pdb CTCEmoteIcon (no retail shift): EmoteText +0x10, EmoteScale +0x18; CThing 64-bit UID +0x18, the
//   appearance TC +0x64.
// Shape: camera and position are named locals fetched in that order (the landed CThingAICreature::
//   WorldUpdateBulletTime 0x0082FD90); `(camPos - pos).GetLengthSquared() < 576` as one expression (the Z*Z term first);
//   the focal point reuses the camera temporary's frame words; the font is a named local fetched first; the text
//   primitive is a named local built from the white CRGBColour temporary and handed to the renderer.
extern float g_EmoteTextHeight;                 // 0x01381044 (0.5f)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLengthSquared() const { float s = Z * Z; s += Y * Y; s += X * X; return s; }
};
struct CRGBColour {                             // D3DCOLOR byte order
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { Set(r, g, b, a); }
};
struct CWideString {
    void* PStringData;
    ~CWideString();                             // 0x0099B510
};
struct CFontBank;
struct CEnginePrimitiveText {                   // 0x2c bytes
    unsigned char _base[0x20];
    CWideString   Text;                         // +0x20
    CFontBank*    PFont;                        // +0x24
    bool          ScaleIn3D;                    // +0x28
    CEnginePrimitiveText(unsigned __int64 handle, const C3DVector& pos, float scale, const CWideString& text,
                         const CFontBank& font, const CRGBColour& colour, bool scaleIn3D, long interpolationSet);   // 0x004C3120
};
struct CEnginePrimitiveRenderer {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void AddPrimitive(const CEnginePrimitiveText& primitive);   // slot 20 (+0x50)
};
struct CEnginePrimitivePrepareInfo {
    unsigned char _pad00[0x34];
    CEnginePrimitiveRenderer* PRenderer;        // +0x34
};
struct CCamera {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual C3DVector GetPosition() const;      // slot 8 (+0x20)
};
struct CTCGraphicAppearance {
    bool GetStaticFocalPosIfLoaded(C3DVector* pos);   // 0x004C11F0
};
struct CThing {
    unsigned char _pad00[0x18];
    unsigned __int64 UID;                       // +0x18
    unsigned char _pad20[0x64 - 0x20];
    CTCGraphicAppearance* PGraphicAppearance;   // +0x64
    CTCGraphicAppearance* GetGraphicAppearance() const { return PGraphicAppearance; }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CMainGameComponent {
    unsigned char _pad00[0x16144];
    CFontBank* EmoteFont;                       // +0x16144
    CFontBank* GetEmoteFont() const { return EmoteFont; }
};
struct CTCEmoteIcon {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    long        ActiveTimer;                    // +0x0c
    CWideString EmoteText;                      // +0x10
    long        EmoteIcon;                      // +0x14
    float       EmoteScale;                     // +0x18
    const CCamera* GetCamera() const;           // 0x00686A80 (CTCBase)
    CMainGameComponent* GetGameSubsystem();     // 0x00686D60 (CTCBase)
    void DrawEmoteTypeText(const CEnginePrimitivePrepareInfo& info);
};

void CTCEmoteIcon::DrawEmoteTypeText(const CEnginePrimitivePrepareInfo& info)
{
    const CCamera* camera = GetCamera();
    const C3DVector& pos = ParentThing->GetPosition();
    if ((camera->GetPosition() - pos).GetLengthSquared() < 576.0f) {
        C3DVector focal;
        if (ParentThing->GetGraphicAppearance()->GetStaticFocalPosIfLoaded(&focal)) {
            focal.Z += g_EmoteTextHeight;
            CFontBank* font = GetGameSubsystem()->GetEmoteFont();
            CEnginePrimitiveText text(ParentThing->UID, focal, EmoteScale * 1.4f, EmoteText, *font,
                                      CRGBColour(255, 255, 255, 255), true, 0);
            info.PRenderer->AddPrimitive(text);
        }
    }
}