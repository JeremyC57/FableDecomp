// CTCStealth::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- stealth debug display: when the debug flag is set, show the thing's active stealth states ("V ", "H ", "S ", "Sn ") as grey text three units above it
// Grade: DIFFER(308v308) -- residue: frame packing only. Ours homes the ToWideString argument temporary in the dead `info` argument slot (frame 0x40); retail gives it its own frame word below the colour (frame 0x44), so every later esp offset is 4 off, and retail loads the engine vtable (ebx) before the pushes where ours loads it after the Z store
// Leaf: DrawPreparePrimitivesForRendering@CTCStealth
// Retail: Fable.exe (TLC, VC7.1) @ 0x007471F0, 308 bytes, `ret 4`. Name: CTCStealth vtable 0x012689EC slot 11 (rtti-vtables.tsv;
//   the manifest's "CTCStealth::vfunc_11") = Ego_r.pdb ?DrawPreparePrimitivesForRendering@CTCStealth@@UAEXABVCEnginePrimitivePrepareInfo@@@Z
//   (slot 11 is DrawPreparePrimitivesForRendering in the landed CTCInventoryMagic 0x0072DAC0 too).
// Callees (PDB spellings): CTCBase::GetThingManager 0x00686AE0 (IAE, result discarded), CCharString() 0x0099E4B0,
//   CCharString::operator+=(const char*) 0x0099F100 (QAEABV0@PBD@Z; "V " 0x012689E4, "H " 0x012689E0, "S " 0x0126895C, "Sn " 0x012689DC),
//   CCharString::operator!=(const char*) const 0x0099E960 ("" 0x0122D70E), CTCBase::PeekComponent 0x00686D60 (IBE, returns
//   const CMainGameComponent&), CThing::GetPos 0x004C73D0, CCharString::ToWideString() const 0x0099E480, CEnginePrimitiveText
//   ctor 0x004C3120, the prepare info's Engine (+0x34, CIEngine*) slot 20 (+0x50), ~CWideString 0x0099B510 (x2), ~CCharString 0x0099EAE0.
// Data: the debug flag 0x013B8A80 (no PDB name, unbound; named descriptively -- it neighbours CThingManager's statics 0x013B8A84/88);
//   3.0f 0x0122DED4 (bound as g_three among others: emitted here as a literal).
// Layout: Ego_r.pdb CTCStealth Viewable +0x0C, Smellable +0x0D, Hearable +0x0E, Sneaking +0x0F; CMainGameComponent PDebugFont
//   +0x16144 (CCountedPointer<CFontBank>, Data read directly: `*PDebugFont` keeps the pointer to the counted pointer, +5 bytes).
// Shape: the flag is read BEFORE the discarded GetThingManager call (a static-member read through GetThingManager() loads it
//   after the call); the position is a C3DVector argument temporary built from a named GetPos reference (gives retail's
//   fld/fadd/stores schedule; a named copy with Z += 3 stores late). Tried without effect on the frame word: early return vs
//   if-block, colour declared in the inner block, pointer vs reference parameter.
// No engine/CTCStealth.h exists and engine/CTCBase.h flattens the vptr into a `void* __vftable` field: local models.
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
    const CCharString& operator+=(const char* text);          // 0x0099F100
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
class CIEngine;
struct CEnginePrimitivePrepareInfo {
    unsigned char _pad00[0x34];
    CEnginePrimitiveSink* Engine;                       // +0x34 CIEngine*
};
#include "engine/CCountedPointer.h"
struct CMainGameComponent {
    unsigned char _pad00[0x16144];
    CCountedPointer<CFontBank> PDebugFont;              // +0x16144
};
class CThing {
public:
    const C3DVector& GetPos() const;                    // 0x004C73D0
};
class CThingManager { };
extern bool g_bShowStealthDebug;                        // 0x013B8A80
struct CTCStealth {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x4];
    bool Viewable;                                      // +0x0c
    bool Smellable;                                     // +0x0d
    bool Hearable;                                      // +0x0e
    bool Sneaking;                                      // +0x0f
protected:
    CThingManager& GetThingManager();                   // 0x00686AE0 (CTCBase)
    const CMainGameComponent& PeekComponent() const;    // 0x00686D60 (CTCBase)
public:
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

void CTCStealth::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
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
        CFontBank* font = PeekComponent().PDebugFont.Data;
        const C3DVector& pos = ParentThing->GetPos();
        info.Engine->AddPrimitive(CEnginePrimitiveText(0, C3DVector(pos.X, pos.Y, pos.Z + 3.0f), 1.0f, states.ToWideString(), *font, colour, false, 0));
    }
}
