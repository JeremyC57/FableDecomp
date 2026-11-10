#ifndef FABLE_NFRACTAL_RECONSTRUCTION_H
#define FABLE_NFRACTAL_RECONSTRUCTION_H
#include "engine/C2DVector.h"
namespace NFractal {
class CNoise {
private:
 long P[512];C2DVector G2[512];unsigned long Seed;
 void InitPerlinNoise();float SCurve(float)const;float Lerp(float,float,float)const;float At2(const C2DVector&,float,float)const;
public:
 CNoise();CNoise(long);CNoise(const CNoise&);~CNoise();void SetSeed(long);float PerlinNoise(const C2DVector&)const;
};
class CFractal {
protected:
 double ExponentArray[128];double Lacunarity;double Octaves;CNoise& Noise;
public:
 CFractal(CNoise&,double,double,double);CFractal(CNoise&);CFractal(const CFractal&);
 void NewFractalData(double,double,double);
 virtual double GetFractalHeight(const C2DVector&)const=0;
};
class CHybridMultifractal:public CFractal {public:CHybridMultifractal(CNoise&);CHybridMultifractal(const CHybridMultifractal&);virtual double GetFractalHeight(const C2DVector&)const;};
class CHeteroTerrainFractal:public CFractal {public:CHeteroTerrainFractal(CNoise&);CHeteroTerrainFractal(const CHeteroTerrainFractal&);virtual double GetFractalHeight(const C2DVector&)const;};
}
#endif
