#ifndef FABLE_ENGINE_CRGBCOLOUR_H
#define FABLE_ENGINE_CRGBCOLOUR_H
#include "rebuild_abi.h"
#include "fable_compiler.h"
// Layout: original PDB class V, sizeof4; native B,G,R,A at0,1,2,3.
#ifdef FABLE_RETAIL_CRGBCOLOUR_INTERPOLATE
// (hand3c, CRGBColour::Interpolate below) its callees: FableWin.pdb ?GFGetInterpolatedColour@@YA?AVCRGBColour@@V1@0K@Z (0x00A44250, __cdecl)
// and the GFFloatToLongNear asm helper.
#include "engine/GFFloatToLongNear.h"
class CRGBColour;
CRGBColour GFGetInterpolatedColour(CRGBColour a, CRGBColour b, unsigned long t);   // 0x00A44250
#endif
#ifdef FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
// Both original TPI records,27 members. C13ctors36..39/Initialise41.
class CRGBALong;
class CRGBColour {
public:
    union {
        struct { unsigned char B,G,R,A; };
        unsigned long IntValue;
    };
    CRGBColour() { }
    CRGBColour(unsigned char r,unsigned char g,unsigned char b) { Initialise(r,g,b,255); }
    CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { Initialise(r,g,b,a); }
#ifdef FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COPY_FORCEINLINE_PASS26
    // Inline-budget: size-built 005BA48A expands the original colour copy and Initialise.
    FABLE_FORCEINLINE CRGBColour(const CRGBColour& colour) { Initialise(colour.R,colour.G,colour.B,colour.A); }
    FABLE_FORCEINLINE void Initialise(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r;G=g;B=b;A=a; }
#else
    CRGBColour(const CRGBColour& colour) { Initialise(colour.R,colour.G,colour.B,colour.A); }
    void Initialise(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r;G=g;B=b;A=a; }
#endif // FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COPY_FORCEINLINE_PASS26
    void SetR(unsigned char);
    void SetG(unsigned char);
    void SetB(unsigned char);
    void SetA(unsigned char);
    unsigned char GetR() const { return R; }
    unsigned char GetG() const { return G; }
    unsigned char GetB() const { return B; }
    unsigned char GetA() const { return A; }
    void SetFromInt(unsigned long);
    void SetD3DCOLOR(unsigned long);
    unsigned long GetAsInt() const;
    unsigned long GetD3DCOLOR() const;
    CRGBALong GetRGBALong() const;
    CRGBColour Interpolate(CRGBColour,float) const;
    void Roll(long,long,long,long);
    bool operator==(const CRGBColour&) const;
    bool operator!=(const CRGBColour&) const;
    // No generated copy assignment or destructor is redeclared.
};
#else
class CRGBColour {
public:
    union {
        struct { unsigned char B,G,R,A; };
        unsigned long IntValue;
    };
#ifdef FABLE_RETAIL_CRGBCOLOUR_DEFAULTCTOR_OUTOFLINE
    // Opt-in (wfE8, row 0x00B03CA0, PROBE): the default ctor not visible to the TU -- retail builds the row's two CRGBColour[3] locals with
    // `vector constructor iterator` 0x004038C0 over the folded empty ctor 0x004048E0 (pushes at 0x00B040EF / 0x00B042AC). Declaration only.
    CRGBColour();
#else
    CRGBColour() { }
#endif
#if defined(FABLE_RETAIL_CRGBCOLOUR) && defined(FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE)
    // Opt-in (chunk 4007, row 0x0061B610; first measured by hand882 on the PC twin 0x0061E6D0): FableWin ??0CRGBColour@@QAE@EEEE@Z is
    // 18 instructions / 44 bytes at /Od = the four-channel constructor forwarding to ?Initialise@CRGBColour@@QAEXEEEE@Z (19i/48b),
    // like the three-channel constructor below; the member-store body is 20i/51b. Same release bytes, one more inline layer.
    CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { Initialise(r,g,b,a); }
#elif defined(FABLE_RETAIL_CRGBCOLOUR_FORCEINLINE_CTOR4)
    // Opt-in (wfE3, 2026-11-15, row 0x00F1EED0 PlayWave): retail never calls ??0CRGBColour@@QAE@EEEE@Z out of line (no binding in
    // rebuild/oracles/reloc_bindings.tsv) and expands it at 0x00F21108 where c2's plain-inline score is 888 (f 0.0024): always inline.
    FABLE_FORCEINLINE CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; }
#else
#ifdef FABLE_RETAIL_CRGBCOLOUR_CTOR_FORCEINLINE
    // Per-TU opt-in (row 0x00CBFB7D RunCutsceneMacro; hand885 text): ??0CRGBColour@@QAE@EEEE@Z has no out-of-line copy in Fable.exe
    // or the Ego_r.pdb name set; retail stores R, G, B, A in place at all six sites of that 74 KB function.
    FABLE_FORCEINLINE
#endif
    CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; }
#ifdef FABLE_RETAIL_CRGBCOLOUR_SETCHANNEL
    // Opt-in (chunk 4010, row 0x00BD3070 RenderMeshPrimitive): FableWin.pdb ?SetR / ?SetG / ?SetB / ?SetA@CRGBColour@@QAEXE@Z (debug COMDATs
    // of 10 instructions = one member store; no Ego_r.pdb or Fable.exe copy). The row sets the tint's alpha from the sub-primitive before
    // the float conversion (0x00BD4DE5 `mov al,[ecx+8]` .. 0x00BD4DFF `movzx eax,al`).
    void SetR(unsigned char r) { R = r; }
    void SetG(unsigned char g) { G = g; }
    void SetB(unsigned char b) { B = b; }
    void SetA(unsigned char a) { A = a; }
#endif
#endif
#ifdef FABLE_RETAIL_CRGBCOLOUR
    // Retail opt-in (chunk 724): the PDB's Initialise(r, g, b, a) (?Initialise@CRGBColour@@QAEXEEEE@Z) and the copy constructor
    // built on it: all four channel bytes are read (as Initialise's arguments) before any is stored, the byte-by-byte copy
    // retail emits where a CRGBColour is passed by value (CPSCDecalRenderer::InterpolateTo 0x00B1BDA0 into
    // GFGetInterpolatedColour; GOTCHAS: the landed rows spell the setter `Set`, which neither PDB has).
    void Initialise(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; }
#ifdef FABLE_RETAIL_CRGBCOLOUR_CURSOR_COPY_INLINE_PASS22
    // Native0064BFBA expands both fade-colour copies; ordinary size budget also expands unrelated native calls.
    FABLE_FORCEINLINE CRGBColour(const CRGBColour& colour) { Initialise(colour.R,colour.G,colour.B,colour.A); }
#else
    CRGBColour(const CRGBColour& colour) { Initialise(colour.R,colour.G,colour.B,colour.A); }
#endif
    // Codex pass 13 (rows 0x00EEDC20 / 0x00E8BEF0): FableWin ??0CRGBColour@@QAE@EEE@Z, inline in retail (R, G, B then A = 0xFF stored
    // in order through Initialise).
    CRGBColour(unsigned char r,unsigned char g,unsigned char b) { Initialise(r,g,b,255); }
#elif defined(FABLE_RETAIL_CRGBCOLOUR_DWORD_COPY)
    // Retail opt-in (chunk 1018, row 0x00405220 TransferIMEColour): the PDB copy constructor ??0CRGBColour@@QAE@ABV0@@Z copies the
    // colour as one dword in that TU (`mov ecx,[esp+18h] ; mov [esp+10h],ecx` at 0x00405251 / 0x0040525A).
    CRGBColour(const CRGBColour& colour) : IntValue(colour.IntValue) { }
#elif defined(FABLE_RETAIL_CRGBCOLOUR_FORCEINLINE_COPY)
    // Opt-in (chunk 2051, row 0x0042EC7C CNewFrontendGameComponent::Run): the default copy body, forced. In a size TU c2 refuses this
    // plain inline with no score (the by-value CXMVPlayer::SetClearColour argument was a call) where retail copies the four bytes in
    // R, G, B, A order in place (0x0042ED68); GOTCHAS chunk 268: "a by-value CRGBColour argument built in its slot needs a
    // __forceinline user copy ctor".
    FABLE_FORCEINLINE CRGBColour(const CRGBColour& colour) { R=colour.R; G=colour.G; B=colour.B; A=colour.A; }
#else
#ifdef FABLE_RETAIL_CRGBCOLOUR_CTOR_FORCEINLINE
    // (same opt-in) the copy constructor: retail copies a by-value CRGBColour byte by byte in place, R G B then A
    // (0x00CD0C57..0x00CD0C6D, FadeScreenOut's argument).
    FABLE_FORCEINLINE
#endif
    CRGBColour(const CRGBColour& colour) { R=colour.R; G=colour.G; B=colour.B; A=colour.A; }
#endif
#ifdef FABLE_RETAIL_CRGBCOLOUR_GETASINT
    // Opt-in (chunk 2032, row 0x00BA6FF0): FableWin.pdb ?GetAsInt@CRGBColour@@QBEKXZ (no Ego_r.pdb or Fable.exe copy): the packed dword
    // read in place (0x00BA7732 `mov edx,[esp+18h] ; push edx` hands the silhouette colour to DrawAlphaBoundingBox's unsigned long).
    unsigned long GetAsInt() const { return IntValue; }
#endif
#ifdef FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR
    // Retail opt-in (chunk 2018, row 0x00B2B180): Ego_r.pdb / FableWin.pdb ?GetD3DCOLOR@CRGBColour@@QBEKXZ, inline in retail (the B, G, R,
    // A bytes are a D3DCOLOR dword: 0x00B2B4BA `mov edx,[eax+8]` stores the blend's DiffuseColour into a vertex as one dword).
    // The dword read is the body: FableWin.pdb's debug COMDAT 0x02CCFDA0 fills one 0x20-byte slot, like GetAsInt 0x02284290 and GetR
    // 0x01D73670 (a four-channel packing is over 50 bytes at /Od). The channel packing in CRenderManager2D::Draw2DLine 0x009DD96E /
    // Draw2DTexture 0x009DC6ED is those rows' own expression over GetA / GetR / GetG / GetB (landing fz, 2026-11-13).
    unsigned long GetD3DCOLOR() const { return IntValue; }
#endif
#ifdef FABLE_RETAIL_CRGBCOLOUR_INTERPOLATE
    // hand3c opt-in (row 0x0066CD50 CRegionMinimap::BuildRegionMinimapTGA): Ego_r.pdb / FableWin.pdb ?Interpolate@CRGBColour@@QBE?AV1@V1@M@Z
    // (FableWin 0x02188AA0: GFGetInterpolatedColour(*this, col, GFFloatToLongNear(interp * 256)); Ego_d.pdb names col / interp; FableWin's literal is a double, retail's a
    // float 0x0125B240); inline in retail (0x0066CE21..0x0066CE67, no out-of-line copy called). Needs FABLE_RETAIL_CRGBCOLOUR (the
    // Initialise copy constructor), the callee declarations above the class.
#ifdef FABLE_RETAIL_CRGBCOLOUR_INTERPOLATE_FORCEINLINE
    // Nested opt-in (hand3c3, row 0x0078D250 CTCBerserk::FrameUpdate): retail expands Interpolate at both tint sites there
    // (0x0078D680..0x0078D6F8 and 0x0078D869..0x0078D8D9: `this` pushed first, `fmul [256.0f]`, the bare fistp, the result built in
    // a frame slot and passed by `lea`); VC7.1 calls the plain inline out of line (1678 bytes for the 1729-byte row) and there is no
    // /d2inlT window (T76..T160 expand the list destructor retail calls first, 1705 bytes; T200 1728, T300 1751).
    FABLE_FORCEINLINE
#endif
    CRGBColour Interpolate(CRGBColour col, float interp) const { return GFGetInterpolatedColour(*this, col, GFFloatToLongNear(interp * 256.0f)); }
#endif
    unsigned char GetR() const { return R; }
    unsigned char GetG() const { return G; }
    unsigned char GetB() const { return B; }
    unsigned char GetA() const { return A; }
};
#endif
FABLE_STATIC_ASSERT_AT(65, sizeof(CRGBColour)==4);
#endif
