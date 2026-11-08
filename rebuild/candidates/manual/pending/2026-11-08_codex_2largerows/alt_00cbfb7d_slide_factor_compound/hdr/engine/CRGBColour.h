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
#ifdef FABLE_RETAIL_CRGBCOLOUR_CTOR_FORCEINLINE
    // Per-TU opt-in (single-row worker 876, row 0x00CBFB7D RunCutsceneMacro): ??0CRGBColour@@QAE@EEEE@Z has no out-of-line copy in
    // Fable.exe (rebuild/oracles/reloc_bindings.tsv) or the Ego_r.pdb name set (FableWin 0x018BCBE0 is the debug COMDAT); retail
    // stores R, G, B, A in place at all six sites of that 74 KB function, where the plain inline body is called out of line.
    __forceinline
#endif
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
#else
#ifdef FABLE_RETAIL_CRGBCOLOUR_CTOR_FORCEINLINE
    // (same opt-in) the copy constructor: retail copies a by-value CRGBColour byte by byte in place, R G B then A
    // (0x00CD0C57..0x00CD0C6D, FadeScreenOut's argument), where the plain inline body is called out of line.
    __forceinline
#endif
    CRGBColour(const CRGBColour& colour) { R=colour.R; G=colour.G; B=colour.B; A=colour.A; }
#endif
    unsigned char GetR() const { return R; }
    unsigned char GetG() const { return G; }
    unsigned char GetB() const { return B; }
    unsigned char GetA() const { return A; }
};
FABLE_STATIC_ASSERT(sizeof(CRGBColour)==4);
#endif
