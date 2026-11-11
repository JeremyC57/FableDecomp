#ifndef FABLE_ENGINE_CRGBCOLOUR_H
#define FABLE_ENGINE_CRGBCOLOUR_H
#include "rebuild_abi.h"
// Layout: original PDB class V, sizeof4; native B,G,R,A at0,1,2,3.
class CRGBColour {
public:
    union {
        struct { unsigned char B,G,R,A; };
        unsigned long IntValue;
    };
    CRGBColour() { }
    CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; }
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
#else
    CRGBColour(const CRGBColour& colour) { R=colour.R; G=colour.G; B=colour.B; A=colour.A; }
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
