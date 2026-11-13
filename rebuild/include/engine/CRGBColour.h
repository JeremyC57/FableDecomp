#ifndef FABLE_ENGINE_CRGBCOLOUR_H
#define FABLE_ENGINE_CRGBCOLOUR_H
#include "rebuild_abi.h"
#include "fable_compiler.h"
// Layout: original PDB class V, sizeof4; native B,G,R,A at0,1,2,3.
class CRGBColour {
public:
    union {
        struct { unsigned char B,G,R,A; };
        unsigned long IntValue;
    };
    CRGBColour() { }
#if defined(FABLE_RETAIL_CRGBCOLOUR) && defined(FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE)
    // Opt-in (chunk 4007, row 0x0061B610; first measured by hand882 on the PC twin 0x0061E6D0): FableWin ??0CRGBColour@@QAE@EEEE@Z is
    // 18 instructions / 44 bytes at /Od = the four-channel constructor forwarding to ?Initialise@CRGBColour@@QAEXEEEE@Z (19i/48b),
    // like the three-channel constructor below; the member-store body is 20i/51b. Same release bytes, one more inline layer.
    CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { Initialise(r,g,b,a); }
#else
    CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; }
#endif
#ifdef FABLE_RETAIL_CRGBCOLOUR
    // Retail opt-in (chunk 724): the PDB's Initialise(r, g, b, a) (?Initialise@CRGBColour@@QAEXEEEE@Z) and the copy constructor
    // built on it: all four channel bytes are read (as Initialise's arguments) before any is stored, the byte-by-byte copy
    // retail emits where a CRGBColour is passed by value (CPSCDecalRenderer::InterpolateTo 0x00B1BDA0 into
    // GFGetInterpolatedColour; GOTCHAS: the landed rows spell the setter `Set`, which neither PDB has).
    void Initialise(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; }
    CRGBColour(const CRGBColour& colour) { Initialise(colour.R,colour.G,colour.B,colour.A); }
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
    CRGBColour(const CRGBColour& colour) { R=colour.R; G=colour.G; B=colour.B; A=colour.A; }
#endif
#ifdef FABLE_RETAIL_CRGBCOLOUR_GETASINT
    // Opt-in (chunk 2032, row 0x00BA6FF0): FableWin.pdb ?GetAsInt@CRGBColour@@QBEKXZ (no Ego_r.pdb or Fable.exe copy): the packed dword
    // read in place (0x00BA7732 `mov edx,[esp+18h] ; push edx` hands the silhouette colour to DrawAlphaBoundingBox's unsigned long).
    unsigned long GetAsInt() const { return IntValue; }
#endif
#ifdef FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR_B
    // Retail opt-in (chunk 2018, row 0x00B2B180): Ego_r.pdb / FableWin.pdb ?GetD3DCOLOR@CRGBColour@@QBEKXZ, inline in retail (the B, G, R,
    // A bytes are a D3DCOLOR dword: 0x00B2B4BA `mov edx,[eax+8]` stores the blend's DiffuseColour into a vertex as one dword).
    unsigned long GetD3DCOLOR() const { return IntValue; }
#endif
    unsigned char GetR() const { return R; }
    unsigned char GetG() const { return G; }
    unsigned char GetB() const { return B; }
    unsigned char GetA() const { return A; }
#ifdef FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR
    // Retail opt-in (chunk Q12, row 0x009DD960 CRenderManager2D::Draw2DLine): Ego_r.pdb / FableWin.pdb ?GetD3DCOLOR@CRGBColour@@QBEKXZ,
    // inline in retail: the A,R,G,B word packed from the channel bytes (`movzx eax,[R] ; mov ah,[A] ; shl eax,8 ; or eax,G ; shl
    // eax,8 ; or eax,B` at 0x009DD96E). No layout change.
    unsigned long GetD3DCOLOR() const { return (((((unsigned long)A << 8) | R) << 8 | G) << 8) | B; }
#endif
};
FABLE_STATIC_ASSERT(sizeof(CRGBColour)==4);
#endif
