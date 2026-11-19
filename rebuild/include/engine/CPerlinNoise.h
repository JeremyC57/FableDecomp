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
// nested LookupIndex hash, 0x00F360BE LookupGradient's `& 0xff` index, 0x00F36630 LookupScalar). Grouping of SCurve / Interp (no /Od
// evidence: same instructions; SCurve is left-associative t*t*(...): its Ego_d /Od 44 bytes = 23 bytes of x87 arithmetic) measured on the three users 0x00F36010 / 0x00F36540 / 0x00F361D0 (VC7.1 scheduling-region tuples).
float SCurve(float t)const{return t*t*(3.0f-(2.0f*t));}
float Interp(float a,float b,float t)const{return a+((b-a)*t);}
unsigned long LookupIndex(unsigned long i)const{return IndexTable[i&0xff];}
C3DVector LookupGradient(unsigned long i)const{return VectorTable[i&0xff];}
float LookupScalar(unsigned long i)const{return VectorTable[i&0xff].X;}
public:
#endif
private:unsigned long IndexTable[256];C3DVector VectorTable[256];
};
}
FABLE_STATIC_ASSERT(sizeof(NFractal::CPerlinNoise)==4096);
#endif
