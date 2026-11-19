#ifndef FABLE_ENGINE_CPERLINNOISE_H
#define FABLE_ENGINE_CPERLINNOISE_H
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
// Layout: both original PDB type records and donor Update02F3F4C0;
// native B0DC20 member accesses use the original retail container sizes.
namespace NFractal {
class CPerlinNoise {
public:float GetGaussian2DNoise(const C2DVector&)const;
#ifdef FABLE_RETAIL_CPERLINNOISE_GETNOISE
// hand2c opt-in (rows 0x00F36010 / 0x00F361D0): Ego_r.pdb ?Get2DNoise@CPerlinNoise@NFractal@@QBEMABVC2DVector@@@Z (0x00BC77B0) and
// ?Get3DNoise@CPerlinNoise@NFractal@@QBEMABVC3DVector@@@Z (0x00BC79E0), donor order = retail 0x00F36010 / 0x00F361D0. Declarations only.
float Get2DNoise(const C2DVector& point)const;
float Get3DNoise(const C3DVector& point)const;
private:
// Ego_d.pdb CPerlinNoise LF_FIELDLIST 0x232DF: private inline helpers SCurve(float), Interp(float a, float b, float t),
// LookupIndex(unsigned long i), LookupGradient(unsigned long i) (C3DVector by value), LookupScalar(unsigned long i), all const.
// Bodies from their Ego_d /Od lengths (44 / 33 / 27 / 46 / 34 bytes) and retail's expansions (0x00F360D5 SCurve, 0x00F36095 the
// nested LookupIndex hash, 0x00F360BE LookupGradient's `& 0xff` index, 0x00F36630 LookupScalar). Bracketing of SCurve / Interp
// (same instructions every way; c1 leaves one no-code PAREN tuple per bracketed float sub-expression, and c2 schedules in regions
// of 81 tuples): measured with c2 DAG dumps on Get3DNoise 0x00F361D0, whose one block spans four regions -- SCurve(tx)'s fmuls
// (0x00F36303 / 0x00F3630D) need its PARENs after (3-2t) and on the whole product, not on 2t, and the region cut at 0x00F3640F needs
// one PAREN fewer per Interp; the outer-bracketed returns below are the spelling that matches all three users 0x00F36010 /
// 0x00F36540 / 0x00F361D0 (every tree change breaks the siblings). FableWin.pdb's later revision reads t * t * (3.0 - 2.0 * t).
float SCurve(float t)const{return (t*t*(3.0f-2.0f*t));}
float Interp(float a,float b,float t)const{return (a+(b-a)*t);}
unsigned long LookupIndex(unsigned long i)const{return IndexTable[i&0xff];}
C3DVector LookupGradient(unsigned long i)const{return VectorTable[i&0xff];}
float LookupScalar(unsigned long i)const{return VectorTable[i&0xff].X;}
public:
#endif
private:unsigned long IndexTable[256];C3DVector VectorTable[256];
};
}
FABLE_STATIC_ASSERT_AT(13, sizeof(NFractal::CPerlinNoise)==4096);
#endif
