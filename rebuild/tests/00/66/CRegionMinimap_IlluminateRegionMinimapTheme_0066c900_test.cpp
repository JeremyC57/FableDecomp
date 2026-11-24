#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION_SQUARE_PAREN_PASS26
#define FABLE_RETAIL_C3DVECTOR_DOT_ORIGINAL_SCOPE_PASS23
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#include "engine/C3DVector.h"
#include <string.h>
unsigned long GFastOneOverSqrtTable[128];
static void InitialiseRootSeeds()
{
    for (unsigned int i=0;i<128;++i) {
        const float value=(i<64?2.0f:1.0f)*(1.0f+(i&63)/64.0f);
        float reciprocal=(float)(1.0/sqrt((double)value));
        unsigned long bits;
        memcpy(&bits,&reciprocal,sizeof(bits));
        GFastOneOverSqrtTable[i]=(bits&0x00ffffffUL)>>15;
    }
}
struct CWorldMap{C3DVector normal;mutable C3DVector requested;mutable int calls;CWorldMap():normal(0,0,1),calls(0){}C3DVector GetGroundFaceNormalAt(const C3DVector&p)const{requested=p;++calls;return normal;}};
struct CRegionMinimap{C3DVector IlluminationDirection;CRegionMinimap():IlluminationDirection(0,0,1){}void GetRegionMinimapIlluminationDirection(C3DVector&dir)const{dir=IlluminationDirection;}float IlluminateRegionMinimapTheme(const CWorldMap&worldMap,const C3DVector&worldPos)const;};

float CRegionMinimap::IlluminateRegionMinimapTheme(const CWorldMap& worldMap,const C3DVector& worldPos) const
{
 C3DVector faceNormal=worldMap.GetGroundFaceNormalAt(worldPos);
 C3DVector lightDir(0.0f,0.0f,0.0f);
 GetRegionMinimapIlluminationDirection(lightDir);
 C3DVector faceNormalNorm=faceNormal.Normalise();
 C3DVector lightDirNorm=lightDir.Normalise();
 return (lightDirNorm.Dot(faceNormalNorm));
}

static void check(bool p,const char*s){if(!p){printf("FAIL %s\n",s);exit(1);}}
static bool close(float a,float b){return fabs(a-b)<0.001;}
int main(){InitialiseRootSeeds();CRegionMinimap m;CWorldMap w;C3DVector p(3,4,5);check(close(m.IlluminateRegionMinimapTheme(w,p),1),"aligned");check(w.calls==1&&w.requested.X==3&&w.requested.Y==4&&w.requested.Z==5,"world position forwarded once");m.IlluminationDirection=C3DVector(0,0,-3);check(close(m.IlluminateRegionMinimapTheme(w,p),-1),"opposing and no clamp");m.IlluminationDirection=C3DVector(2,0,0);check(close(m.IlluminateRegionMinimapTheme(w,p),0),"orthogonal");w.normal=C3DVector(0,3,4);m.IlluminationDirection=C3DVector(0,6,8);check(close(m.IlluminateRegionMinimapTheme(w,p),1),"independent scale normalisation");check(w.normal.Y==3&&m.IlluminationDirection.Y==6,"source vectors unchanged");m.IlluminationDirection=C3DVector(0,0,0);check(close(m.IlluminateRegionMinimapTheme(w,p),0),"zero illumination");w.normal=C3DVector(0,0,0);m.IlluminationDirection=C3DVector(1,0,0);check(close(m.IlluminateRegionMinimapTheme(w,p),0),"zero face normal");puts("0066c900_TEST PASS");return 0;}