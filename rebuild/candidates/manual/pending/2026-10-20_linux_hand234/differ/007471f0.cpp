// CTCStealth::DrawPreparePrimitivesForRendering(CViewport* viewport) -- stealth debug display: when the debug flag is set, show the thing's active stealth states ("V ", "H ", "S ", "Sn ") as grey text three units above it
// Grade: DIFFER(308v308) -- frame packing: ours homes the ToWideString argument temporary in the dead viewport argument slot (frame 40h), retail gives it a frame word below the colour (frame 44h) and schedules the position copy accordingly
// Leaf: DrawPreparePrimitivesForRendering
// Retail: Fable.exe (TLC, VC7.1) @ 0x007471F0, 307 bytes, `ret 4`. CTCStealth vtable 0x012689EC slot 11 (the manifest's
//   "CTCStealth::vfunc_11"; slot 11 is DrawPreparePrimitivesForRendering, as the landed CTCInventoryMagic 0x0072DAC0).
// Callees: CTCBase::GetThingManager @ 0x00686AE0 (called, result unused), CCharString() @ 0x0099E4B0, CCharString::
//   operator+=(const char*) @ 0x0099F100 ("V " 0x012689E4, "H " 0x012689E0, "S " 0x0126895C, "Sn " 0x012689DC),
//   CCharString::operator!=(const char*) @ 0x0099E960 ("" @ 0x0122D70E), CTCBase::GetGameSubsystem @ 0x00686D60,
//   CThing::GetPosition @ 0x004C73D0, CCharString::ToWideString() const @ 0x0099E480, CEnginePrimitiveText ctor @ 0x004C3120,
//   the viewport primitive sink's slot 20 (+0x50), ~CWideString @ 0x0099B510 (x2), ~CCharString @ 0x0099EAE0. Debug flag
//   @ 0x013B8A80 (named descriptively); 3.0f @ 0x0122DED4.
// Layout: Ego_r.pdb CTCStealth Viewable +0x0C, Smellable +0x0D, Hearable +0x0E, Sneaking +0x0F; the game subsystem's debug
//   font +0x16144 (named descriptively); CViewport primitive sink +0x34; CEnginePrimitiveText (landed 0x004C3120 model,
//   0x2C bytes, Text +0x20).
// Shape: the flag is read before the (discarded) GetThingManager call; the state string is a named local built by +=;
//   the colour is the inline CRGBColour::Set(0xA6, 0xA6, 0xA6, 0xFF); the position a named copy with Z raised; the text
//   primitive is an argument temporary whose inline destructor destroys only its CWideString.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() {}
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CWideString {
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
};
struct CCharString {
    void* m_pData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator+=(const char* text);          // 0x0099F100
    bool operator!=(const char* text) const;            // 0x0099E960
    CWideString ToWideString() const;                   // 0x0099E480
};
struct CFontBank;
struct CEnginePrimitive {
    long Type;
    long InterpolationSet;
    unsigned char ThemeChannelDesc[4];
};
struct CEnginePrimitiveText : CEnginePrimitive {
    C3DVector   Pos;                                    // +0x0c
    float       Scale;                                  // +0x18
    CRGBColour  Colour;                                 // +0x1c
    CWideString Text;                                   // +0x20
    CFontBank*  PFont;                                  // +0x24
    bool        ScaleIn3D;                              // +0x28
    CEnginePrimitiveText(unsigned __int64 handle, const C3DVector& pos, float scale, const CWideString& text,
                         const CFontBank& font, const CRGBColour& colour, bool scaleIn3D, long interpolationSet);   // 0x004C3120
};
struct CEnginePrimitiveSink {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19();
    virtual void AddPrimitive(const CEnginePrimitive& primitive);   // slot 20 (+0x50)
};
struct CViewport {
    unsigned char _pad00[0x34];
    CEnginePrimitiveSink* PPrimitives;                  // +0x34
};
struct CGameSubsystem {
    unsigned char _pad00[0x16144];
    CFontBank*    DebugFont;                            // +0x16144
};
class CThing {
public:
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
struct CThingManager;
extern bool g_bShowStealthDebug;                        // 0x013B8A80
struct CTCStealth {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x4];
    bool Viewable;                                      // +0x0c
    bool Smellable;                                     // +0x0d
    bool Hearable;                                      // +0x0e
    bool Sneaking;                                      // +0x0f
    CThingManager* GetThingManager();                   // 0x00686AE0 (CTCBase)
    CGameSubsystem* GetGameSubsystem();                 // 0x00686D60 (CTCBase)
    void DrawPreparePrimitivesForRendering(CViewport* viewport);
};

void CTCStealth::DrawPreparePrimitivesForRendering(CViewport* viewport)
{
    bool show = g_bShowStealthDebug;
    GetThingManager();
    if (!show)
        return;
    CCharString states;
    CRGBColour colour;
    if (Viewable)
        states += "V ";
    if (Hearable)
        states += "H ";
    if (Smellable)
        states += "S ";
    if (Sneaking)
        states += "Sn ";
    if (states != "") {
        colour.Set(0xa6, 0xa6, 0xa6, 0xff);
        CFontBank* font = GetGameSubsystem()->DebugFont;
        C3DVector textPos = ParentThing->GetPosition();
        textPos.Z += 3.0f;
        viewport->PPrimitives->AddPrimitive(CEnginePrimitiveText(0, textPos, 1.0f, states.ToWideString(), *font, colour, false, 0));
    }
}
