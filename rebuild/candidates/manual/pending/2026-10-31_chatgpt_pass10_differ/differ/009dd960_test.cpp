// Fixture: selected target pasted unchanged; coordinate conversion and renderer submission are checked boundaries.
#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#endif
// 009dd960 CRenderManager2D::Draw2DLine
// Leaf: ?Draw2DLine@CRenderManager2D@@QAE_NABVC2DVector@@0ABVCRGBColour@@K@Z
// Flags:
// Layout: original CTVertexRHWColSpecTex1Base public fields, GenericVertex base,32bytes.
class C2DVector {public: float X, Y; };
class CRGBColour {public: unsigned char B, G, R, A; };
class CTexture;

class CTVertexRHWColSpecTex1Base {public:                 // 0x20 bytes
    float         X, Y, Z;                      // +0x00
    float         RHW;                          // +0x0c
    unsigned long DiffuseColour;                       // +0x10 D3DCOLOR
    unsigned long SpecularColour;                     // +0x14
    float         U1, V1;                         // +0x18
};

template<class T>class CGenericVertex:public T{public:void SetRHW(float value){this->RHW=value;}float GetRHW()const{return this->RHW;}};
class CTVertexRHWColSpecTex1:public CGenericVertex<CTVertexRHWColSpecTex1Base>{};
float __fastcall GFVXToAX(float x);              // 0x009E1E10
float __fastcall GFVYToAY(float y);              // 0x009E1E40
extern const float BIGGEST_RHW_VERTEX_VALUE;                     // 0x0139652C

class CRenderManager2D {private:
    void Add2DLine(const CTVertexRHWColSpecTex1& start, const CTVertexRHWColSpecTex1& end, long mode, CTexture* texture);   // 0x009DC2C0
public:bool Draw2DLine(const C2DVector& start, const C2DVector& end, const CRGBColour& colour, unsigned long flags);
};

bool CRenderManager2D::Draw2DLine(const C2DVector& start, const C2DVector& end, const CRGBColour& colour, unsigned long flags)
{
    unsigned long argb = ((unsigned long)colour.A << 24) | ((unsigned long)colour.R << 16) | ((unsigned long)colour.G << 8) | colour.B;
    long mode = 1;
    CTVertexRHWColSpecTex1 v0;
    CTVertexRHWColSpecTex1 v1;
    v0.X = GFVXToAX(start.X);
    v0.Y = GFVYToAY(start.Y);
    v1.X = GFVXToAX(end.X);
    v1.Y = GFVYToAY(end.Y);
    v1.RHW = BIGGEST_RHW_VERTEX_VALUE;
    v0.DiffuseColour = argb;
    v1.DiffuseColour = argb;
    v0.SpecularColour = 0;
    v1.SpecularColour = 0;
    v0.Z = 0.0f;
    v1.Z = 0.0f;
    v0.RHW = v1.RHW;
    v1.U1 = 0.0f;
    v0.U1 = 0.0f;
    v1.V1 = 0.0f;
    v0.V1 = 0.0f;
    if (flags & 1) {
        if (colour.A != 0xff)
            mode = 3;
    }
    else
        mode = (flags & 2) ? 2 : 1;
    Add2DLine(v0, v1, mode, 0);
    return true;
}

#ifdef _MSC_VER
typedef char CheckVertexSize[sizeof(CTVertexRHWColSpecTex1)==32?1:-1];
typedef char CheckColourSize[sizeof(CRGBColour)==4?1:-1];
#endif
#define CHECK(x)do{if(!(x)){fprintf(stderr,"CHECK line %d\n",__LINE__);abort();}}while(0)
const float BIGGEST_RHW_VERTEX_VALUE=100.0f;
static int conversionCalls,submissions;
static float expectedArguments[4];
static CRenderManager2D*expectedManager;
static C2DVector expectedStart,expectedEnd;
static unsigned long expectedColour;
static long expectedMode;
float __fastcall GFVXToAX(float x){CHECK(conversionCalls==0||conversionCalls==2);CHECK(x==expectedArguments[conversionCalls]);++conversionCalls;return x*2.0f+1.0f;}
float __fastcall GFVYToAY(float y){CHECK(conversionCalls==1||conversionCalls==3);CHECK(y==expectedArguments[conversionCalls]);++conversionCalls;return y*4.0f-2.0f;}
void CRenderManager2D::Add2DLine(const CTVertexRHWColSpecTex1&start,const CTVertexRHWColSpecTex1&end,long mode,CTexture*texture){
 CHECK(this==expectedManager&&conversionCalls==4&&texture==0&&mode==expectedMode);++submissions;
 CHECK(start.X==expectedStart.X*2.0f+1.0f&&start.Y==expectedStart.Y*4.0f-2.0f);
 CHECK(end.X==expectedEnd.X*2.0f+1.0f&&end.Y==expectedEnd.Y*4.0f-2.0f);
 CHECK(start.RHW==100.0f&&end.RHW==100.0f&&start.Z==0.0f&&end.Z==0.0f);
 CHECK(start.DiffuseColour==expectedColour&&end.DiffuseColour==expectedColour);
 CHECK(start.SpecularColour==0&&end.SpecularColour==0&&start.U1==0&&start.V1==0&&end.U1==0&&end.V1==0);
}
int main(){
 const unsigned long flagsList[]={0,1,2,3,4,5,0x80000000UL,0xffffffffUL};
 const float coordinates[4][4]={{0,0,0,0},{-2,4,8,-16},{0.25f,-0.5f,1.5f,2.75f},{128,-64,-32,16}};
 const unsigned char colours[4][3]={{0,0,0},{255,255,255},{17,34,51},{128,1,254}};
 long cases=0;
 for(int alpha=0;alpha<256;++alpha)for(int flag=0;flag<8;++flag)for(int col=0;col<4;++col)for(int coordinate=0;coordinate<4;++coordinate){
  CRenderManager2D manager;C2DVector start,end;CRGBColour colour;
  start.X=coordinates[coordinate][0];start.Y=coordinates[coordinate][1];end.X=coordinates[coordinate][2];end.Y=coordinates[coordinate][3];
  colour.B=colours[col][0];colour.G=colours[col][1];colour.R=colours[col][2];colour.A=(unsigned char)alpha;
  for(int k=0;k<4;++k)expectedArguments[k]=coordinates[coordinate][k];expectedStart=start;expectedEnd=end;expectedManager=&manager;
  expectedColour=(unsigned long)colour.B+256UL*colour.G+65536UL*colour.R+16777216UL*alpha;
  const unsigned long flags=flagsList[flag];expectedMode=1;if(flags&1){if(alpha<255)expectedMode=3;}else if(flags&2)expectedMode=2;
  conversionCalls=submissions=0;CHECK(manager.Draw2DLine(start,end,colour,flags));CHECK(conversionCalls==4&&submissions==1);
  CHECK(start.X==coordinates[coordinate][0]&&start.Y==coordinates[coordinate][1]&&end.X==coordinates[coordinate][2]&&end.Y==coordinates[coordinate][3]);
  CHECK(colour.A==alpha&&colour.B==colours[col][0]&&colour.G==colours[col][1]&&colour.R==colours[col][2]);++cases;
 }
 CHECK(cases==32768);puts("009dd960_TEST PASS");return 0;
}
