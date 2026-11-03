#ifndef FABLE_ENGINE_GFMATHS_H
#define FABLE_ENGINE_GFMATHS_H
// New shared helper header (chunk 724b): the Lionhead GF maths inlines that retail expands in place (no call in
// NCameraModes::DrawSquare 0x00875FB0 / the camera rows). Spellings: Ego_r + FableWin name sets ?GFSqr@@YIMM@Z,
// ?GFSin@@YINM@Z, ?GFCos@@YINM@Z, ?GFTanh@@YINN@Z, ?GFExp@@YINN@Z (global __fastcall; GFSin/GFCos take turns and return double:
// `sin(angle * TWO_PI)`, the `fmul qword [TWO_PI 0x0124F2B8]` then fsin/fcos in retail). TWO_PI is the established external
// `const double` ?TWO_PI@@3NB (reloc_bindings). Plain C++: no asm, identical on every compiler.
#include <math.h>
extern const double TWO_PI;
inline float __fastcall GFSqr(float n) { return n * n; }
inline double __fastcall GFSin(float angle) { return sin(angle * TWO_PI); }
inline double __fastcall GFCos(float angle) { return cos(angle * TWO_PI); }
inline double __fastcall GFTanh(double value) { return tanh(value); }
inline double __fastcall GFExp(double value) { return exp(value); }
#endif
